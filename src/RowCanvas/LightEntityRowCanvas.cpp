#include "LightEntityRowCanvas.hpp"
#include "LovelaceConfig/LightCardConfig.hpp"

using namespace std::placeholders;

LightEntityRowCanvas::LightEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    ToggleEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    this->entityCallback = [](BaseCardConfig*)->bool{return false;};
    this->entity = hass->getEntity<Light>(rowConfig->getEntityId());
    defaultStateColor = 1;
    if (entity) {
        this->brightness = 0;
        this->tmpBrightness = 0;
        onTouch(std::bind(&LightEntityRowCanvas::onTouchEventCallback, this, _2, _3));
        this->stateCanvas->onStateChange([this](SwitchCanvas* canvas, bool state)->bool{
                entity->setState(state);
                if (entity->isFeatureSupported(Light::SUPPORT_BRIGHTNESS)) {
                    if (brightness == 0 && state) {
                        brightness = 0xFF;
                    }
                    entity->setBrightness(brightness);
                }
                this->hass->callStateService(entity);
                return true;
            });
        auto touchCallback = [this](Canvas*, TouchEvent event, TouchEventData)->bool{
            if (isEvent(event, TouchActionTapped)) {
                return entityCallback(new LightCardConfig(entity->getEntityId(), this->rowConfig->getName(),
                    ICON_CHEVRON_LEFT, this->rowConfig->getIcon(), this->rowConfig->getStateColor()));
            }
            return false;
        };
        this->nameCanvas->onTouch(touchCallback);
        this->iconCanvas->onTouch(touchCallback);
    }
}

bool LightEntityRowCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (!entity->isFeatureSupported(Light::SUPPORT_BRIGHTNESS)) {
        return false;
    }
    if (isEvent(event, TouchActionLongPressedAndDragged)) {
        int16_t deltaBrightness = (eventData.endX -eventData.startX)/SWIPE_THRESHOLD;
        uint16_t currBrightness = brightness;
        if (this->stateCanvas->getState()) {
            currBrightness = brightness;
            if (brightness > 192) {
                currBrightness = 255;
            } else if (brightness > 128) {
                currBrightness = 191;
            } else if (brightness > 64) {
                currBrightness = 127;
            } else if (brightness > 0) {
                currBrightness = 63;
            } else {
                currBrightness = 0;
            }
        } else {
            currBrightness = 0;
        }
        int16_t nextBrightness = currBrightness + ((deltaBrightness)<<6);
        if (nextBrightness > 192) {
            nextBrightness = 255;
        } else if (nextBrightness > 128) {
            nextBrightness = 191;
        } else if (nextBrightness > 64) {
            nextBrightness = 127;
        } else if (nextBrightness > 0) {
            nextBrightness = 63;
        } else {
            nextBrightness = 0;
        }
        tmpBrightness = nextBrightness;
        this->setBrightnessIconPath();
        return true;
    } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
        if (tmpBrightness != this->brightness) {
            this->brightness = tmpBrightness;
            this->stateCanvas->resetIcon();
            entity->setBrightness(brightness);
            if (brightness == 0) {
                this->stateCanvas->setState(false);
            } else {
                this->stateCanvas->setState(true);
            }
            entity->setState(this->stateCanvas->getState());
            this->hass->callStateService(entity);
            this->stateCanvas->invalidate();
        } else {
            this->stateCanvas->resetIcon();
        }
        return true;
    } else if (isEvent(event, TouchActionLongPressed)) {
        if (this->stateCanvas->getState()) {
            tmpBrightness = brightness;
        } else {
            tmpBrightness = 0;
        }
        this->setBrightnessIconPath();
        return true;
    } else if (isEvent(event, TouchActionLongPressReleased)) {
        if (tmpBrightness != this->brightness) {
            this->brightness = tmpBrightness;
            this->stateCanvas->resetIcon();
            if (brightness == 0) {
                this->stateCanvas->setState(false);
            } else {
                this->stateCanvas->setState(true);
            }
            entity->setBrightness(brightness);
            entity->setState(this->stateCanvas->getState());
            this->hass->callStateService(entity);
        } else {
            this->stateCanvas->resetIcon();
        }
        return true;
    }
    return false;
}

void LightEntityRowCanvas::setBrightnessIconPath() {
    if (tmpBrightness == 0) {
        this->stateCanvas->setPath(ICON_LIGHT_OFF);
    } else if (tmpBrightness < 0x40) {
        this->stateCanvas->setPath(ICON_LIGHT_1);
    } else if (tmpBrightness < 0x80) {
        this->stateCanvas->setPath(ICON_LIGHT_2);
    } else if (tmpBrightness < 0xC0) {
        this->stateCanvas->setPath(ICON_LIGHT_3);
    } else if (tmpBrightness <= 0xFF) {
        this->stateCanvas->setPath(ICON_LIGHT_4);
    } else {
        this->stateCanvas->setPath(ICON_LIGHT_OFF);
    }
    this->stateCanvas->setCustomState(true);
    if (tmpBrightness == 0) {
        this->stateCanvas->setFgColor(this->stateCanvas->getSurfaceColor());
    } else {
        this->stateCanvas->setFgColor(this->stateCanvas->getSecondaryColor());
    }
}

void LightEntityRowCanvas::update() {
    ToggleEntityRowCanvas::update();
    if (entity) {
        this->brightness = entity->getBrightness();
        if (this->stateCanvas->getState() == 0) {
            brightness = 0;
        }
    }
}