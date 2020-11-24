#include "FanEntityRowCanvas.hpp"
#include "LovelaceConfig/FanCardConfig.hpp"

using namespace std::placeholders;

FanEntityRowCanvas::FanEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    ToggleEntityRowCanvas(canvas, id, hass, rowConfig, theme){
    this->entityCallback = [](BaseCardConfig*)->bool{return false;};
    this->entity = hass->getEntity<Fan>(rowConfig->getEntityId());
    if (entity) {
        this->speed = 0;
        this->tmpSpeed = 0;
        swipeThreshold = 20;
        onTouch(std::bind(&FanEntityRowCanvas::onTouchEventCallback, this, _2, _3));
        this->stateCanvas->onStateChange([this](SwitchCanvas* canvas, bool state)->bool{
                entity->setState(state);
                if (entity->isFeatureSupported(Fan::SUPPORT_SET_SPEED)) {
                    entity->setSpeed(speed);
                }
                this->hass->callStateService(entity);
                return true;
            });
        auto touchCallback = [this](Canvas*, TouchEvent event, TouchEventData)->bool{
            if (isEvent(event, TouchActionTapped)) {
                return entityCallback(new FanCardConfig(entity->getEntityId(), this->rowConfig->getName(),
                    ICON_CHEVRON_LEFT, this->rowConfig->getIcon(), this->rowConfig->getStateColor()));
            }
            return false;
        };
        this->nameCanvas->onTouch(touchCallback);
        this->iconCanvas->onTouch(touchCallback);
    } else {
        TextCanvas * errorCanvas = new TextCanvas(this, 0);
        errorCanvas->setText("Entity is not 'fan'");
    }
}

void FanEntityRowCanvas::setFanIconPath() {
    if (tmpSpeed == 0){
        this->stateCanvas->setPath(ICON_FAN_OFF);
    } else if (tmpSpeed == 1) {
        this->stateCanvas->setPath(ICON_FAN_SPEED_1);
    } else if (tmpSpeed == 2) {
        this->stateCanvas->setPath(ICON_FAN_SPEED_2);
    } else if (tmpSpeed == 3) {
        this->stateCanvas->setPath(ICON_FAN_SPEED_3);
    } else {
        this->stateCanvas->setPath(ICON_FAN_OFF);
    }
    this->stateCanvas->setCustomState(true);
    if (tmpSpeed == 0) {
        this->stateCanvas->setFgColor(this->stateCanvas->getSurfaceColor());
    } else {
        this->stateCanvas->setFgColor(this->stateCanvas->getSecondaryColor());
    }
}

bool FanEntityRowCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (isEvent(event, TouchActionLongPressedAndDragged)) {
        int16_t deltaSpeed = (eventData.endX -eventData.startX)/swipeThreshold;
        uint16_t currSpeed = speed;
        if (this->stateCanvas->getState()) {
            currSpeed = speed;
        } else {
            currSpeed = 0;
        }
        int16_t nextSpeed = currSpeed + deltaSpeed;
        if (nextSpeed < 0) {
            nextSpeed = 0;
        } else if (nextSpeed >= 3) {
            nextSpeed = 3;
        }
        tmpSpeed = nextSpeed;
        this->setFanIconPath();
        return true;
    } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
        this->speed = tmpSpeed;
        this->stateCanvas->resetIcon();
        if (entity->isFeatureSupported(Fan::SUPPORT_SET_SPEED)) {
            entity->setSpeed(speed);
        }
        entity->setState(this->stateCanvas->getState());
        this->hass->callStateService(entity);
        return true;
    } else if (isEvent(event, TouchActionLongPressed)) {
        if (this->stateCanvas->getState()) {
            tmpSpeed = speed;
        } else {
            tmpSpeed = 0;
        }
        this->setFanIconPath();
        return true;
    } else if (isEvent(event, TouchActionLongPressReleased)) {
        if (tmpSpeed != this->speed) {
            this->speed = tmpSpeed;
            this->stateCanvas->resetIcon();
            if (entity->isFeatureSupported(Fan::SUPPORT_SET_SPEED)) {
                entity->setSpeed(speed);
            }
            entity->setState(this->stateCanvas->getState());
            this->hass->callStateService(entity);
        } else {
            this->stateCanvas->resetIcon();
        }
        return true;
    }
    return false;
}

void FanEntityRowCanvas::update() {
    ToggleEntityRowCanvas::update();
    if (entity) {
        this->speed = entity->getSpeedIndex();
    }
}