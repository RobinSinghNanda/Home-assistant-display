#include "ClimateEntityRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"
#include "LovelaceConfig/ThermostatCardConfig.hpp"

ClimateEntityRowCanvas::ClimateEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    this->entity = hass->getEntity<Climate>(rowConfig->getEntityId());
    this->rowConfig = rowConfig;
    if (entity) {
        modeSelectCanvas = new TextSelectCanvas(this, 2);
        modeSelectCanvas->setHAlign(ALIGN_RIGHT);
        modeSelectCanvas->setWidth(140);
        modeSelectCanvas->hAlignRight();
        modeSelectCanvas->setFont(FONT_REGULAR_24);
        modeSelectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                uint8_t index = entity->getHvacModeIndex();
                if (event == CHANGE_NEXT) {    
                    index++;
                    if (index == entity->getNumHvacModes()) {
                        index = 0;
                    }
                } else {
                    if (index == 0) {
                        index = entity->getNumHvacModes() - 1;
                    } else {
                        index--;
                    }
                }
                entity->setHvacMode(index);
                entity->setState(entity->getHvacMode());
                value = entity->getHvacMode();
                this->hass->callService(entity, Climate::SERVICE_SET_HVAC_MODE);
                return true;
            });

        textCanvas = new TextCanvas(this, 5);
        textCanvas->setPaddingRight(10);
        textCanvas->setWidth(80);
        textCanvas->setFont(FONT_REGULAR_24);
        textCanvas->setHAlign(ALIGN_RIGHT);
        textCanvas->setVAlign(ALIGN_MIDDLE);
        textCanvas->setVisible(false);
        textCanvas->hAlignLeft(modeSelectCanvas);
        auto touchCallback = [this](Canvas*, TouchEvent event, TouchEventData)->bool{
            if (isEvent(event, TouchActionTapped)) {
                return entityCallback(new ThermostatCardConfig(entity->getEntityId(), this->rowConfig->getName(), this->rowConfig->getName(),
                    ICON_CHEVRON_LEFT, this->rowConfig->getIcon()));
            }
            return false;
        };
        this->nameCanvas->onTouch(touchCallback);
        this->iconCanvas->onTouch(touchCallback);
        this->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool {
                if (isEvent(event, TouchActionLongPressedAndDragged)) {
                    if (!entity->isFeatureSupported(Climate::SUPPORT_TARGET_TEMPERATURE) || entity->isOff()) {
                        return false;
                    }
                    touched = true;
                    int16_t deltaTemp = (eventData.endX -eventData.startX)/SWIPE_THRESHOLD;
                    float currTemp = entity->getTemperature();
                    float nextTemp = currTemp + deltaTemp*entity->getTemperatureStep();
                    if (nextTemp < entity->getMinTemperature()) {
                        nextTemp = entity->getMinTemperature();
                    } else if (nextTemp > entity->getMaxTemperature()) {
                        nextTemp = entity->getMaxTemperature();
                    }
                    temperature = nextTemp;
                    setTemperatureState();
                    return true;
                } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
                    if (!entity->isFeatureSupported(Climate::SUPPORT_TARGET_TEMPERATURE) || entity->isOff()) {
                        return false;
                    }
                    touched = false;
                    if (temperature != entity->getTemperature()) {
                        this->entity->setTemperature(temperature);
                        this->hass->callService(entity, Climate::SERVICE_SET_TEMPERATURE);
                    }
                    this->resetState();
                    return true;
                } else if (isEvent(event, TouchActionLongPressed)) {
                    if (!entity->isFeatureSupported(Climate::SUPPORT_TARGET_TEMPERATURE) || entity->isOff()) {
                        return false;
                    }
                    touched = true;
                    temperature = entity->getTemperature();
                    setTemperatureState();
                    return true;
                } else if (isEvent(event, TouchActionLongPressReleased)) {
                    if (!entity->isFeatureSupported(Climate::SUPPORT_TARGET_TEMPERATURE) || entity->isOff()) {
                        return false;
                    }
                    touched = false;
                    if (temperature != entity->getTemperature()) {
                        this->entity->setTemperature(temperature);
                        this->resetState();
                        this->hass->callService(entity, Climate::SERVICE_SET_TEMPERATURE);
                    } else {
                        this->resetState();
                    }
                    return true;
                }
                return false;
            });
    }
}

ClimateEntityRowCanvas::~ClimateEntityRowCanvas() {

}

void ClimateEntityRowCanvas::update() {
    BaseEntityRowCanvas::update();
    if (entity) {
        if (touched) {
        } else {
            if (entity->isUnavailable()) {
                modeSelectCanvas->setVisible(false);
                textCanvas->setVisible(false);
            } else {
                modeSelectCanvas->setVisible(true);
                modeSelectCanvas->setValue(entity->getState());
            }
        }
    }
}

void ClimateEntityRowCanvas::setTemperatureState() {
    if (entity) {
        modeSelectCanvas->setVisible(false);
        textCanvas->setVisible(true);
        textCanvas->hAlignRight();
        char temp[11];
        Support::dtostrfd(temperature, 1, temp);
        snprintf(temp, sizeof(temp), "%s°C", temp);
        textCanvas->setText(temp);
    }
}

void ClimateEntityRowCanvas::resetState() {
    if (entity) {
        modeSelectCanvas->setVisible(true);
        textCanvas->setVisible(false);
        textCanvas->hAlignLeft(modeSelectCanvas);
    }
}