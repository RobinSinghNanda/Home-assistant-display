#include "LightEntityCanvas.hpp"


const uint32_t colorLookupTable[LIGHT_MAX_COLORS+1] = {
        0xFF0000,
        0xFF003F,
        0xFF007F,
        0xFF00BF,
        0xFF00FF,
        0xBF00FF,
        0x7F00FF,
        0x3F00FF,
        0x0000FF,
        0x003FFF,
        0x007FFF,
        0x00BFFF,
        0x00FFFF,
        0x00FFBF,
        0x00FF7F,
        0x00FF3F,
        0x00FF00,
        0x3FFF00,
        0x7FFF00,
        0xBFFF00,
        0xFFFF00,
        0xFFBF00,
        0xFF7F00,
        0xFF3F00,
        0xFF0000
    };

LightEntityCanvas::LightEntityCanvas(Canvas * canvas, uint16_t id) : SwitchEntityCanvas(canvas, id){
    brightnessSlider = new CustomSliderCanvas(this, 0);
    colorTemperatureSlider = new CustomSliderCanvas(this, 1);
    colorSlider = new CustomSliderCanvas(this, 2);
    whiteSlider = new CustomSliderCanvas(this, 3);
    brightnessSlider->setHeight(40);
    colorTemperatureSlider->setHeight(40);
    colorSlider->setHeight(40);
    whiteSlider->setHeight(40);
    brightnessSlider->setY(canvas->getY()+40);
    colorTemperatureSlider->setY(canvas->getY()+80);
    colorSlider->setY(canvas->getY()+120);
    whiteSlider->setY(canvas->getY()+160);
    brightnessSlider->setHMargin(20);
    colorTemperatureSlider->setHMargin(20);
    colorSlider->setHMargin(20);
    whiteSlider->setHMargin(20);
    brightnessSlider->setVMargin(5);
    colorTemperatureSlider->setVMargin(5);
    colorSlider->setVMargin(5);
    whiteSlider->setVMargin(5);
    brightnessSlider->setStep(20);
    brightnessSlider->setMax(256);
    colorSlider->setStep(1);
    colorSlider->setMax(LIGHT_MAX_COLORS+1);
    colorTemperatureSlider->setStep(20);
    colorTemperatureSlider->setMax(101);
    whiteSlider->setStep(20);
    whiteSlider->setMax(256);
    brightnessSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            return this->stateChangeCallback(this, this->getState());
        });
    colorTemperatureSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t preValue)->bool{
            return this->stateChangeCallback(this, this->getState());
        });
    colorSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            this->color = colorLookupTable[value];
            this->whiteSlider->setValue(0);
            return this->stateChangeCallback(this, this->getState());
        });
    whiteSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            this->colorSlider->invalidateValue();
            return this->stateChangeCallback(this, this->getState());
        });
    brightnessSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
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
                return false;
        }
        return true;
        });
    whiteSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
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
                uint8_t color = 127+x*127/canvas->getDrawableWidth();
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
                return false;
        }
        return false;
        });
    colorTemperatureSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
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
        uint16_t red, green, blue;
        for (uint16_t y = 2;y <canvas->getDrawableHeight();y++) {
            for (uint16_t x=1; x < canvas->getDrawableWidth()-1;x++) {
                if ( x > SLIDER_MARGIN_LEFT &&
                     x < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                     y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                     y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(canvas->convert2rgb565(0x7F7F7F));
                    continue;
                } else if (x < canvas->getDrawableWidth()/2) {
                    red = map(x, 0, canvas->getDrawableWidth()/2, 0xa8, 0xff);
                    green = map(x, 0, canvas->getDrawableWidth()/2, 0xd2, 0xff);
                    blue = 0xff;
                } else {
                    red = 0xff;
                    green = map(x-canvas->getDrawableWidth()/2, 0, canvas->getDrawableWidth()/2, 0xff, 0xa0);
                    blue = map(x-canvas->getDrawableWidth()/2, 0, canvas->getDrawableWidth()/2, 0xff, 0x01);
                }
                if (canvas->getDisabled()) {
                    uint8_t color = (red+blue+green)/3;
                    red = color;
                    blue = color;
                    green = color;
                }
                tft->pushColor(canvas->convert2rgb565(red, green, blue));
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
                return false;
        }
        return false;
        });
    colorSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
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
        for (uint16_t y = 2;y <canvas->getDrawableHeight();y++) {
            uint16_t drawableWidthBy6 = (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT-SLIDER_MARGIN_LEFT)/6;
            uint16_t red = 255,green = 0,blue = 0;
            for (uint16_t k=1; k < canvas->getDrawableWidth()-1;k++) {
                uint16_t x = k -SLIDER_MARGIN_LEFT;
                if ( k > SLIDER_MARGIN_LEFT &&
                     k < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                     y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                     y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(canvas->convert2rgb565(0x7F7F7F));
                    continue;
                } else if (k < SLIDER_MARGIN_LEFT) {
                    red = 255;
                    green = 0;
                    blue = 0;
                } else if (k > canvas->getDrawableWidth() - SLIDER_MARGIN_RIGHT) {
                    red = 255;
                    green = 0;
                    blue = 0;
                } else if (x <  drawableWidthBy6) {
                    red = 255;
                    green = 0;
                    blue = map(x, 0, drawableWidthBy6, 0, 255);
                } else if (x >  1*drawableWidthBy6 && x < 2*drawableWidthBy6) {
                    green = 0;
                    blue = 255;
                    red = map(x - 1*drawableWidthBy6, 0, drawableWidthBy6, 255, 0);
                } else if (x >  2*drawableWidthBy6 && x < 3*drawableWidthBy6) {
                    red = 0;
                    blue = 255;
                    green = map(x - 2*drawableWidthBy6, 0, drawableWidthBy6, 0, 255);
                } else if (x >  3*drawableWidthBy6 && x < 4*drawableWidthBy6) {
                    red = 0;
                    green = 255;
                    blue = map(x - 3*drawableWidthBy6, 0, drawableWidthBy6, 255, 0);
                } else if (x >  4*drawableWidthBy6 && x < 5*drawableWidthBy6) {
                    blue = 0;
                    green = 255;
                    red = map(x - 4*drawableWidthBy6, 0, drawableWidthBy6, 0, 255);
                } else if (x >  5*drawableWidthBy6 && x < 6*drawableWidthBy6) {
                    red = 255;
                    blue = 0;
                    green = map(x - 5*drawableWidthBy6, 0, drawableWidthBy6, 255, 0);
                }
                if (canvas->getDisabled()) {
                    uint8_t color = (red+blue+green)/3;
                    red = color;
                    blue = color;
                    green = color;
                }
                tft->pushColor(canvas->convert2rgb565(red, green, blue));
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
                return false;
        }
        return false;
        });
    this->SwitchEntityCanvas::onStateChange([this](SwitchCanvas * switchCanvas, bool state)->bool{
            this->stateChangeCallback(this, state);
            return true;
        });
    this->brightnessSliderEnabled = true;
    this->colorSliderEnabled = true;
    this->colorTemperatureSliderEnabled = true;
    this->whiteValueSliderEnabled = true;
    setLayout();
}

void LightEntityCanvas::setLayout() {
    uint16_t y = rowCanvas->getBottomY();
    if (brightnessSliderEnabled) {
        brightnessSlider->setVisible(true);
        brightnessSlider->setY(y);
        brightnessSlider->setHeight(rowCanvas->getHeight());
        y += rowCanvas->getHeight();
    } else {
        brightnessSlider->setVisible(false);
    }
    if (colorTemperatureSliderEnabled) {
        colorTemperatureSlider->setVisible(true);
        colorTemperatureSlider->setY(y);
        colorTemperatureSlider->setHeight(rowCanvas->getHeight());
        y += rowCanvas->getHeight();
    } else {
        colorTemperatureSlider->setVisible(false);
    }
    if (whiteValueSliderEnabled) {
        whiteSlider->setVisible(true);
        whiteSlider->setY(y);
        whiteSlider->setHeight(rowCanvas->getHeight());
        y += rowCanvas->getHeight();
    } else {
        whiteSlider->setVisible(false);
    }
    if (colorSliderEnabled) {
        colorSlider->setVisible(true);
        colorSlider->setY(y);
        colorSlider->setHeight(rowCanvas->getHeight());
        y += rowCanvas->getHeight();
    } else {
        colorSlider->setVisible(false);
    }
}

void LightEntityCanvas::onStateChange(LightEntityCanvasStateChangeCallback callback) {
    this->stateChangeCallback = callback;
}

void LightEntityCanvas::setBrighnessSliderEnabled(bool brightnessSliderEnabled) {
    returnIfEqual(this->brightnessSliderEnabled, brightnessSliderEnabled);
    this->brightnessSliderEnabled = brightnessSliderEnabled;
    setLayout();
}

void LightEntityCanvas::setColorTemperatureSliderEnabled(bool colorTemperatureSliderEnabled) {
    returnIfEqual(this->colorTemperatureSliderEnabled, colorTemperatureSliderEnabled);
    this->colorTemperatureSliderEnabled = colorTemperatureSliderEnabled;
    setLayout();
}

void LightEntityCanvas::setColorSliderEnabled(bool colorSliderEnabled) {
    returnIfEqual(this->colorSliderEnabled, colorSliderEnabled);
    this->colorSliderEnabled = colorSliderEnabled;
    setLayout();
}

void LightEntityCanvas::setWhiteValueSliderEnabled(bool whiteValueSliderEnabled) {
    returnIfEqual(this->whiteValueSliderEnabled, whiteValueSliderEnabled);
    this->whiteValueSliderEnabled = whiteValueSliderEnabled;
    setLayout();
}

bool LightEntityCanvas::getBrighnessSliderEnabled() {
    return this->brightnessSliderEnabled;
}

bool LightEntityCanvas::getColorTemperatureSliderEnabled() {
    return this->colorTemperatureSliderEnabled;
}

bool LightEntityCanvas::getColorSliderEnabled() {
    return this->colorSliderEnabled;
}

bool LightEntityCanvas::getWhiteValueSliderEnabled() {
    return this->whiteValueSliderEnabled;
}

void LightEntityCanvas::setBrightness(uint8_t brightness) {
    this->brightnessSlider->setValue(brightness);
}

uint8_t LightEntityCanvas::getBrightness() {
    return this->brightnessSlider->getValue();
}

void LightEntityCanvas::setWhiteValue(uint8_t whiteValue) {
    this->whiteSlider->setValue(whiteValue);
}

uint8_t LightEntityCanvas::getWhiteValue() {
    return this->whiteSlider->getValue();
}

void LightEntityCanvas::setColorTemperature(uint16_t colorTemperature) {
    this->colorTemperatureSlider->setValue(colorTemperature);
}

uint16_t LightEntityCanvas::getColorTemperature() {
    return this->colorTemperatureSlider->getValue();
}

void LightEntityCanvas::setColor(uint32_t color) {
    color &= 0xFFFFFF;
    color = findNearestColor(color);
    returnIfEqual(this->color,color);
    if (color == 0xFFFFFF) {
        this->colorSlider->invalidateValue();
        return;
    }
    this->color = color;
    for (uint8_t index = 0;index<LIGHT_MAX_COLORS;index++) {
        if (colorLookupTable[index]==color) {
            this->colorSlider->setValue(index);
            this->whiteSlider->setValue(0);
            return;
        }
    }
    this->colorSlider->setValue(0);
    this->whiteSlider->setValue(0);
}

void LightEntityCanvas::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    this->setColor((red<<16)|(green<<8)|blue);
}

uint32_t LightEntityCanvas::findNearestColor (uint32_t color) {
    uint8_t tmpRed = (color>>16) & 0xC0;
    uint8_t tmpGreen = (color>>8) & 0xC0;
    uint8_t tmpBlue = color & 0xC0;
    uint8_t red,green,blue;
    if (tmpRed == 0) {
        red = 0;
    } else {
        red = tmpRed | 0x3F;
    }
    if (tmpGreen == 0) {
        green = 0;
    } else {
        green = tmpGreen | 0x3F;
    }
    if (tmpBlue == 0) {
        blue = 0;
    } else {
        blue = tmpBlue | 0x3F;
    }
    return (red<<16)|(green<<8)|blue;
}

uint32_t LightEntityCanvas::getColor() {
    return this->color;
}

void LightEntityCanvas::setMinColorTemperature(uint16_t minColorTemperature) {
    this->colorTemperatureSlider->setMin(minColorTemperature);
}

void LightEntityCanvas::setMaxColorTemperature(uint16_t maxColorTemperature) {
    this->colorTemperatureSlider->setMax(maxColorTemperature);
}

uint16_t LightEntityCanvas::getMinColorTemperature() {
    return this->colorTemperatureSlider->getMin();
}

uint16_t LightEntityCanvas::getMaxColorTemperature() {
    return this->colorTemperatureSlider->getMax();
}

void LightEntityCanvas::setDisabled(bool disabled) {
    this->rowCanvas->setDisabled(disabled);
    this->brightnessSlider->setDisabled(disabled);
    this->colorTemperatureSlider->setDisabled(disabled);
    this->colorSlider->setDisabled(disabled);
}