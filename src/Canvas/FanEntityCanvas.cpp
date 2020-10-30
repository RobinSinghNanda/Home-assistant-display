#include "FanEntityCanvas.hpp"

FanEntityCanvas::FanEntityCanvas(Canvas * canvas, uint16_t id) : SwitchEntityCanvas(canvas, id){
    rowCanvas->setStateColor(false);
    fanSpeedSlider = new SliderCanvas(this, 0);
    fanSpeedSlider->setHeight(40);
    fanSpeedSlider->setY(rowCanvas->getBottomY());
    fanSpeedSlider->setHMargin(20);
    fanSpeedSlider->setVMargin(5);
    fanSpeedSlider->setStep(1);
    fanSpeedSlider->setMax(4);
    fanSpeed = 0;
    rowCanvas->setIconPath("mdi:fan");
    fanSpeedSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            uint8_t fanSpeed = this->getFanSpeed();
            if (this->getFanSpeed() != 0) {
                this->setState(true);
            } else {
                this->setState(false);
            }
            this->setFanSpeed(fanSpeed);
            return this->stateChangeCallback(this, this->getState());
        });
    this->SwitchEntityCanvas::onStateChange([this](SwitchCanvas * switchCanvas, bool state)->bool{
            this->stateChangeCallback(this, state);
            return true;
        });
    this->fanSpeedSliderEnabled = true;
}

void FanEntityCanvas::onStateChange(FanEntityCanvasStateChangeCallback callback) {
    this->stateChangeCallback = callback;
}

void FanEntityCanvas::setFanSpeedSliderEnabled(bool fanSpeedSliderEnabled) {
    returnIfEqual(this->fanSpeedSliderEnabled, fanSpeedSliderEnabled);
    this->fanSpeedSliderEnabled = fanSpeedSliderEnabled;
    this->fanSpeedSlider->setVisible(fanSpeedSliderEnabled);
}

bool FanEntityCanvas::getFanSpeedSliderEnabled() {
    return this->fanSpeedSliderEnabled;
}

void FanEntityCanvas::setFanSpeed(uint8_t fanSpeed) {
    this->fanSpeed = fanSpeed;
    if (getState()) {
        this->fanSpeedSlider->setValue(fanSpeed);
    } else {
        this->fanSpeedSlider->setValue(0);
    }
}

uint8_t FanEntityCanvas::getFanSpeed() {
    return this->fanSpeedSlider->getValue();
}

void FanEntityCanvas::setDisabled(bool disabled) {
    this->rowCanvas->setDisabled(disabled);
    this->fanSpeedSlider->setDisabled(disabled);
}

void FanEntityCanvas::setState(bool state) {
    this->rowCanvas->setState(state);
    if (state) {
        this->fanSpeedSlider->setValue(fanSpeed);
    } else {
        this->fanSpeedSlider->setValue(0);
    }
}