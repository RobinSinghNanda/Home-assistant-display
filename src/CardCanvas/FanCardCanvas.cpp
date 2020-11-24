#include "FanCardCanvas.hpp"

FanCardCanvas::FanCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, FanCardConfig * cardConfig, LcdTheme * theme) :
    ToggleCardCanvas(canvas, id, hass, cardConfig, theme) {
    this->entity = hass->getEntity<Fan>(cardConfig->getEntityId());
    if (entity) {
        this->cardConfig = cardConfig;
        fanSpeedSlider = new SliderCanvas(this, 0);
        fanSpeedSlider->setHeight(40);
        fanSpeedSlider->setY(entityCanvas->getBottomY());
        fanSpeedSlider->setHMargin(20);
        fanSpeedSlider->setVMargin(5);
        fanSpeedSlider->setStep(1);
        fanSpeedSlider->setMax(4);
        fanSpeedSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
                if (value == 0) {
                    entity->setState(false);
                } else {
                    entity->setState(true);
                }
                entity->setSpeed(value);
                this->hass->callStateService(entity);
                return true;
            });
        this->fanSpeedSlider->setVisible(false);
        update();
    }
}

FanCardCanvas::~FanCardCanvas() {
    
}

void FanCardCanvas::update() {
    this->entityCanvas->update();
    if (entity->isOn()) {
        if (entity->isFeatureSupported(Fan::SUPPORT_SET_SPEED)) {
            this->fanSpeedSlider->setVisible(true);
            this->fanSpeedSlider->setMax(entity->getNumSpeeds());
            this->fanSpeedSlider->setValue(entity->getSpeedIndex());
        }
    } else if (entity->isOff()) {
        if (entity->isFeatureSupported(Fan::SUPPORT_SET_SPEED)) {
            fanSpeedSlider->setVisible(true);
            this->fanSpeedSlider->setMax(entity->getNumSpeeds());
            this->fanSpeedSlider->setValue(0);
        }
    } else {
        fanSpeedSlider->setVisible(false);
    }
}