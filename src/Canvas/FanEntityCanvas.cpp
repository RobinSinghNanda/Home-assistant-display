#include "FanEntityCanvas.hpp"

FanEntityCanvas::FanEntityCanvas(Canvas * canvas, uint16_t id) : SwitchEntityCanvas(canvas, id){
    fanSpeedSlider = new CustomSliderCanvas(this, 0);
    fanSpeedSlider->setHeight(40);
    fanSpeedSlider->setY(rowCanvas->getBottomY());
    fanSpeedSlider->setHMargin(20);
    fanSpeedSlider->setVMargin(5);
    fanSpeedSlider->setStep(1);
    fanSpeedSlider->setMax(4);
    fanSpeed = 0;
    rowCanvas->setIconPath("mdi:fan");
    fanSpeedSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            return this->stateChangeCallback(this, this->getState());
        });
    fanSpeedSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
        uint16_t radius = (canvas->getTouched())?SLIDER_TOUCHED_RADIUS:SLIDER_RADIUS;
        uint16_t knobColor = (canvas->getDisabled()?canvas->convert2rgb565(0x919191):canvas->convert2rgb565(SLIDER_KNOB_COLOR));
        tft->drawRect(canvas->getDrawX(),
            canvas->getDrawY(),
            canvas->getDrawableWidth(),
            canvas->getDrawableHeight(),
            canvas->getDarkMode()?TFT_WHITE:TFT_BLACK);
        tft->startWrite();
        tft->setAddrWindow(canvas->getDrawX()+1,
            canvas->getDrawY()+1,
            canvas->getDrawableWidth()-2,
            canvas->getDrawableHeight()-2);
        for (uint16_t y = 2;y < canvas->getDrawableHeight();y++) {
            for (uint16_t x=1; x < canvas->getDrawableWidth()-1;x++) {
                uint8_t color = x*255/canvas->getDrawableWidth();
                if ( x > SLIDER_MARGIN_LEFT &&
                     x < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                     y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                     y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(canvas->convert2rgb565(0x7F7F7F));
                } else {
                    tft->pushColor(canvas->convert2rgb565(color, color, color));
                }
            }
        }
        tft->endWrite();
        if (!canvas->isValueInvalid()) {
            uint16_t circle_x = canvas->getDrawX() + SLIDER_MARGIN_LEFT +
                + ((canvas->getValue() - canvas->getMin())*
                (canvas->getDrawableWidth() - SLIDER_MARGIN_RIGHT - SLIDER_MARGIN_LEFT))
                /(canvas->getMax()-1-canvas->getMin());
            tft->fillCircle(
                    circle_x,
                    canvas->getDrawY() -1 + canvas->getDrawableHeight()/2,
                    radius,
                    knobColor);
                return true;
        }
        return true;
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