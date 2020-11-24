#include "LightCardCanvas.hpp"

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

LightCardCanvas::LightCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, LightCardConfig * cardConfig, LcdTheme * theme) :
    ToggleCardCanvas(canvas, id, hass, cardConfig, theme) {
    this->cardConfig = cardConfig;
    this->entity = hass->getEntity<Light>(cardConfig->getEntityId());
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
    brightnessSlider->setVisible(false);
    colorTemperatureSlider->setVisible(false);
    colorSlider->setVisible(false);
    whiteSlider->setVisible(false);
    brightnessSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            if (value == 0) {
                entity->setState(false);
            } else {
                entity->setState(true);
            }
            this->entity->setBrightness(value);
            this->hass->callStateService(entity);
            return true;
        });
    colorTemperatureSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t preValue)->bool{
            entity->setColorTemp(value);
            this->hass->callStateService(entity);
            return true;
        });
    colorSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            this->color = colorLookupTable[value];
            this->whiteSlider->setValue(0);
            this->entity->setColor(color);
            this->hass->callStateService(entity);
            return true;
        });
    whiteSlider->onValueChange([this](Canvas * canvas, int16_t value, int16_t prevValue)->bool{
            this->colorSlider->invalidateValue();
            this->entity->setWhiteValue(value);
            this->hass->callStateService(entity);
            return true;
        });
    brightnessSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
       tft->drawRect(canvas->getDrawX(),
            canvas->getDrawY(),
            canvas->getDrawableWidth(),
            canvas->getDrawableHeight(),
            canvas->getFgColor());
        tft->startWrite();
        tft->setAddrWindow(canvas->getDrawX()+1,
            canvas->getDrawY()+1,
            canvas->getDrawableWidth()-2,
            canvas->getDrawableHeight()-2);
        for (uint16_t y = 2;y < canvas->getDrawableHeight();y++) {
            for (uint16_t x=1; x < canvas->getDrawableWidth()-1;x++) {
                uint8_t color = x*255/canvas->getDrawableWidth();
                uint16_t bgColor = Color16Bit(color, color, color);
                uint16_t knobColor = canvas->drawKnob(x, y, bgColor);
                if (knobColor != bgColor) {
                    tft->pushColor(knobColor);
                } else if ( x > SLIDER_MARGIN_LEFT &&
                        x < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                        y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                        y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(Color32Bit(SLIDER_COLOR));
                } else {
                    tft->pushColor(bgColor);
                }
            }
        }
        tft->endWrite();
        return true;
        });
    whiteSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
        tft->drawRect(canvas->getDrawX(),
            canvas->getDrawY(),
            canvas->getDrawableWidth(),
            canvas->getDrawableHeight(),
            canvas->getFgColor());
        tft->startWrite();
        tft->setAddrWindow(canvas->getDrawX()+1,
            canvas->getDrawY()+1,
            canvas->getDrawableWidth()-2,
            canvas->getDrawableHeight()-2);
        for (uint16_t y = 2;y < canvas->getDrawableHeight();y++) {
            for (uint16_t x=1; x < canvas->getDrawableWidth()-1;x++) {
                uint8_t color = 127+x*127/canvas->getDrawableWidth();
                Color16Bit bgColor = Color16Bit(color, color, color);
                Color16Bit knobColor = canvas->drawKnob(x, y, bgColor);
                if (knobColor != bgColor) {
                    tft->pushColor(knobColor);
                } else if ( x > SLIDER_MARGIN_LEFT &&
                        x < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                        y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                        y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(Color32Bit(SLIDER_COLOR).get16Bit());
                } else {
                    tft->pushColor(bgColor);
                }
            }
        }
        tft->endWrite();
        return false;
        });
    colorTemperatureSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
        tft->drawRect(canvas->getDrawX(),
            canvas->getDrawY(),
            canvas->getDrawableWidth(),
            canvas->getDrawableHeight(),
            canvas->getFgColor());
        tft->startWrite();
        tft->setAddrWindow(canvas->getDrawX()+1,
            canvas->getDrawY()+1,
            canvas->getDrawableWidth()-2,
            canvas->getDrawableHeight()-2);
        uint16_t red, green, blue;
        for (uint16_t y = 2;y <canvas->getDrawableHeight();y++) {
            for (uint16_t x=1; x < canvas->getDrawableWidth()-1;x++) {
                if (x < canvas->getDrawableWidth()/2) {
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
                Color16Bit bgColor = Color16Bit(red, green, blue);
                Color16Bit knobColor = canvas->drawKnob(x, y, bgColor);
                if (knobColor != bgColor) {
                    tft->pushColor(knobColor);
                } else if ( x > SLIDER_MARGIN_LEFT &&
                        x < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                        y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                        y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(Color32Bit(SLIDER_COLOR).get16Bit());
                } else {
                    tft->pushColor(bgColor);
                }
            }
        }
        tft->endWrite();
        return false;
        });
    colorSlider->onDraw([](CustomSliderCanvas * canvas, TFT_eSPI * tft)->bool{
        tft->drawRect(canvas->getDrawX(),
            canvas->getDrawY(),
            canvas->getDrawableWidth(),
            canvas->getDrawableHeight(),
            canvas->getFgColor());
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
                if (k < SLIDER_MARGIN_LEFT) {
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
                uint16_t bgColor = Color16Bit(red, green, blue);
                uint16_t knobColor = canvas->drawKnob(x, y, bgColor);
                if (knobColor != bgColor) {
                    tft->pushColor(knobColor);
                } else if ( x > SLIDER_MARGIN_LEFT &&
                        x < (canvas->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                        y > (canvas->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                        y < (canvas->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                    tft->pushColor(Color32Bit(SLIDER_COLOR).get16Bit());
                } else {
                    tft->pushColor(bgColor);
                }
            }
        }
        tft->endWrite();
        return false;
    });
}

LightCardCanvas::~LightCardCanvas() {
    
}

void LightCardCanvas::setLayout() {
    Canvas * prevCanvas = entityCanvas;
    if (entity->isFeatureSupported(Light::SUPPORT_BRIGHTNESS)) {
        brightnessSlider->setY(prevCanvas->getBottomY());
        prevCanvas = brightnessSlider;
    }
    if (entity->isFeatureSupported(Light::SUPPORT_COLOR_TEMP)) {
        colorTemperatureSlider->setY(prevCanvas->getBottomY());
        prevCanvas = colorTemperatureSlider;
    }
    if (entity->isFeatureSupported(Light::SUPPORT_WHITE_VALUE)) {
        whiteSlider->setY(prevCanvas->getBottomY());
        prevCanvas = whiteSlider;
    }
    if (entity->isFeatureSupported(Light::SUPPORT_COLOR)) {
        colorSlider->setY(prevCanvas->getBottomY());
        prevCanvas = colorSlider;
    }
    if (!entity->isFeatureSupported(Light::SUPPORT_BRIGHTNESS)) {
        brightnessSlider->setY(prevCanvas->getBottomY());
        prevCanvas = brightnessSlider;
    }
    if (!entity->isFeatureSupported(Light::SUPPORT_COLOR_TEMP)) {
        colorTemperatureSlider->setY(prevCanvas->getBottomY());
        prevCanvas = colorTemperatureSlider;
    }
    if (!entity->isFeatureSupported(Light::SUPPORT_WHITE_VALUE)) {
        whiteSlider->setY(prevCanvas->getBottomY());
        prevCanvas = whiteSlider;
    }
    if (!entity->isFeatureSupported(Light::SUPPORT_COLOR)) {
        colorSlider->setY(prevCanvas->getBottomY());
        prevCanvas = colorSlider;
    }
}

void LightCardCanvas::setColor(uint32_t color) {
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

uint32_t LightCardCanvas::findNearestColor (uint32_t color) {
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

void LightCardCanvas::update() {
    this->entityCanvas->update();
    if (entity->isOn()) {
        if (entity->isFeatureSupported(Light::SUPPORT_BRIGHTNESS)) {
            this->brightnessSlider->setVisible(true);
            this->brightnessSlider->setValue(entity->getBrightness());
        }

        if (entity->isFeatureSupported(Light::SUPPORT_WHITE_VALUE)) {
            this->whiteSlider->setVisible(true);
            this->whiteSlider->setValue(entity->getWhiteValue());
        }
        
        if (entity->isFeatureSupported(Light::SUPPORT_COLOR)) {
            this->colorSlider->setVisible(true);
            setColor(entity->getColor());
        }
      
        if (entity->isFeatureSupported(Light::SUPPORT_COLOR_TEMP)) {
            this->colorTemperatureSlider->setVisible(true);
            this->colorTemperatureSlider->setMin(entity->getMinMireds());
            this->colorTemperatureSlider->setMax(entity->getMaxMireds());
            this->colorTemperatureSlider->setValue(entity->getColorTemp());
        }
    } else {
        if (entity->isFeatureSupported(Light::SUPPORT_BRIGHTNESS)) {
            if (entity->isOff()) {
                this->brightnessSlider->setValue(0);
            } else {
                this->brightnessSlider->invalidateValue();
            }
            this->brightnessSlider->setVisible(true);
        }
        this->whiteSlider->setVisible(false);
        this->colorSlider->setVisible(false);
        this->colorTemperatureSlider->setVisible(false);
    }
    setLayout();
}