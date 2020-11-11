#include "SliderCanvas.hpp"


const uint8_t custom_icons_bin18_cirle_bin[] = {
  0x00, 0x00, 0x40, 0xd9, 0xff, 0x9d, 0x04, 0x00, 0x00,
  0x00, 0x20, 0xfb, 0xff, 0xff, 0xff, 0xbf, 0x02, 0x00, 0x00, 0xe3, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x3e, 0x00, 0x20, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xef, 0x02, 0xb0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0b,
  0xf4, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4f, 0xf9, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x9f, 0xf4, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x4f,
  0xb0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0b, 0x20, 0xfe, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xef, 0x02, 0x00, 0xe3, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x3e, 0x00, 0x00, 0x20, 0xfb, 0xff, 0xff, 0xff, 0xbf, 0x02, 0x00,
  0x00, 0x00, 0x40, 0xd9, 0xff, 0x9d, 0x04, 0x00, 0x00
};

const uint8_t custom_icons_bin12_cirle_bin[] = {
  0x00, 0x60, 0xfc, 0xcf, 0x06, 0x00, 0x10, 0xfc, 0xff,
  0xff, 0xcf, 0x01, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x0c, 0xf6, 0xff, 0xff,
  0xff, 0xff, 0x6f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff,
  0xff, 0xff, 0xcf, 0xf6, 0xff, 0xff, 0xff, 0xff, 0x6f, 0xc0, 0xff, 0xff,
  0xff, 0xff, 0x0c, 0x10, 0xfc, 0xff, 0xff, 0xcf, 0x01, 0x00, 0x60, 0xfc,
  0xcf, 0x06, 0x00
};

SliderCanvas::SliderCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {
    this->min = 0;
    this->max = 100;
    this->step = 10;
    this->value = 0;
    this->prevValue = 0;
    this->prevDragValue = 0;
    this->slideAnywhere = 0;
    this->disabled = false;
    this->invalidValue = true;
    if (canvas->getMarginLeft() == 0)
        this->marginLeft = SLIDER_MARGIN_LEFT;
    else 
        this->marginLeft = canvas->getMarginLeft();
    if (canvas->getMarginRight() == 0)
        this->marginRight = SLIDER_MARGIN_RIGHT;
    else 
        this->marginRight = canvas->getMarginRight();
    knob_color = this->convert2rgb565(SLIDER_KNOB_COLOR);
    slider_color = this->convert2rgb565(SLIDER_COLOR);
    onValueChangeCallback = [](Canvas*, int16_t, int16_t){};
    using namespace std::placeholders;
    onTouch(std::bind(&SliderCanvas::onTouchEventCallback, this, _2, _3));
}

bool SliderCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (isEvent(event, TouchActionTapped)) {
        if (this->disabled)
            return true;
        double tmp_value;
        if (eventData.endX < getDrawX()+SLIDER_MARGIN_LEFT) {
            tmp_value = this->min;
        } else if (eventData.endX>(this->getDrawX()+this->getDrawableWidth()-SLIDER_MARGIN_RIGHT)) {
            tmp_value = this->max-1;
        } else {
            tmp_value = this->min + ((double)(eventData.endX-getDrawX()-SLIDER_MARGIN_LEFT)*(this->max-this->min))/(this->getDrawableWidth()-SLIDER_MARGIN_RIGHT-SLIDER_MARGIN_LEFT);
        }
        double remainder = tmp_value - this->min;
        while(remainder >= this->step) {
            remainder -= this->step;
        }
        if (remainder > this->step/2.0) {
            this->value = this->step + tmp_value - remainder;
        } else {
            this->value = tmp_value - remainder;
        }
        if (this->value > this->max - 1) {
            this->value = this->max - 1;
        }
        this->invalidValue = false;
        if (this->value != prevValue) {
            this->invalidate();
            onValueChangeCallback(this, this->value, this->prevValue);
            this->prevValue = value;
        }
        return true;
    } else if (event & TouchActionDraged) {
        if (this->disabled)
            return true;
        touched = true;
        double tmp_value;
        if (eventData.endX < this->getDrawX()+SLIDER_MARGIN_LEFT) {
            tmp_value = this->min;
        } else if (eventData.endX>(this->getDrawX()+this->getDrawableWidth()-SLIDER_MARGIN_RIGHT)) {
            tmp_value = this->max-1;
        } else {
            tmp_value = this->min + (double(eventData.endX-getDrawX()-SLIDER_MARGIN_LEFT)*(this->max-this->min))/(this->getDrawableWidth()-SLIDER_MARGIN_RIGHT-SLIDER_MARGIN_LEFT);
        }
        double remainder = tmp_value - this->min;
        while(remainder >= this->step) {
            remainder -= this->step;
        }
        if (remainder > this->step/2.0) {
            this->value = this->step + tmp_value - remainder;
        } else {
            this->value = tmp_value - remainder;
        }
        if (this->value > this->max - 1) {
            this->value = this->max - 1;
        }
        this->invalidValue = false;
        if (this->value != prevDragValue) {
            this->invalidate();
            this->prevDragValue = this->value;
        }
        return true;
    } else if (event & TouchActionDragReleased) {
        if (this->disabled)
            return true;
        touched = false;
        if (this->prevValue != this->value) {
            this->invalidate();
            onValueChangeCallback(this, this->value, this->prevValue);
            this->prevValue = this->value;
        }
        return true;
    }
    return false;
}

bool SliderCanvas::draw() {
    tft->startWrite();
    tft->setAddrWindow(this->getDrawX()+1,
        this->getDrawY()+1,
        this->getDrawableWidth()-2,
        this->getDrawableHeight()-2);
    for (uint16_t y = 1;y < this->getDrawableHeight()-1;y++) {
        for (uint16_t x=1; x < this->getDrawableWidth()-1;x++) {
            uint16_t bgColor = this->getBgColor();
            uint16_t knobColor = drawKnob(x, y, this->getBgColor());
            if (knobColor != bgColor) {
                tft->pushColor(knobColor);
            } else if ( x > SLIDER_MARGIN_LEFT &&
                    x < (this->getDrawableWidth()-SLIDER_MARGIN_RIGHT) &&
                    y > (this->getDrawableHeight() - SLIDER_HEIGHT)/2 &&
                    y < (this->getDrawableHeight() + SLIDER_HEIGHT)/2 ) {
                tft->pushColor(this->convert2rgb565(SLIDER_COLOR));
            } else {
                tft->pushColor(bgColor);
            }
        }
    }
    tft->endWrite();
    return true;
}

uint16_t SliderCanvas::drawKnob(uint16_t x, uint16_t y, uint16_t bgColor) {
    uint16_t knobColor = (this->getDisabled()?this->convert2rgb565(0x919191):this->convert2rgb565(SLIDER_KNOB_COLOR));
    if (!this->isValueInvalid()) {
        uint16_t circle_x = SLIDER_MARGIN_LEFT +
            + ((this->getValue() - this->getMin())*
            (this->getDrawableWidth() - SLIDER_MARGIN_RIGHT - SLIDER_MARGIN_LEFT))
            /(this->getMax()-1-this->getMin());
        uint16_t circle_y = this->getDrawableHeight()/2;
        uint16_t knobRadius;
        if (this->touched) {
            knobRadius = 9;
        } else {
            knobRadius = 6;
        }
        if ((x < circle_x - knobRadius) || (x >= circle_x + knobRadius) ||
            (y < circle_y - knobRadius) || (y >= circle_y + knobRadius)) {
            return bgColor;
        }
        uint8_t knobX = x - (circle_x - knobRadius);
        uint8_t knobY = y - (circle_y - knobRadius);
        uint8_t alpha;
        if (this->touched) {
            if (knobX%2 == 0) {
                alpha = ((*(custom_icons_bin18_cirle_bin+knobY*knobRadius+knobX/2))<<4)&0xF0;
            } else {
                alpha = (*(custom_icons_bin18_cirle_bin+knobY*knobRadius+knobX/2))&0xF0;
            }
        } else {
            if (knobX%2 == 0) {
                alpha = ((*(custom_icons_bin12_cirle_bin+knobY*knobRadius+knobX/2))<<4)&0xF0;
            } else {
                alpha = (*(custom_icons_bin12_cirle_bin+knobY*knobRadius+knobX/2))&0xF0;
            }
        }
        return tft->alphaBlend(alpha, knobColor, bgColor);
    }
    return bgColor;
}


void SliderCanvas::onValueChange(SliderCanvasValueChangeCallback callback) {
    onValueChangeCallback = callback;
}

void SliderCanvas::setMin(int16_t min) {
    if (touched)
        return;
    invalidateIfNotEqual(this->min, min);
    if (this->value < this->min) {
        this->value = this->min;
    }
}

void SliderCanvas::setMax(int16_t max) {
    if (touched)
        return;
    invalidateIfNotEqual(this->max, max);
    if (this->value > this->max) {
        this->value = this->max -1;
    }
}

void SliderCanvas::setStep(int16_t step) {
    invalidateIfNotEqual(this->step, step);
}

void SliderCanvas::setValue(int16_t value) {
    if (touched)
        return;
    invalidateIfNotEqual(this->value, value);
    this->invalidValue = false;
}

void SliderCanvas::setName(String name) {
    invalidateIfNotEqual(this->name, name);
}

void SliderCanvas::setIcon(String icon) {
    invalidateIfNotEqual(this->icon, icon);
}

int16_t SliderCanvas::getMin() {
    return this->min;
}

int16_t SliderCanvas::getMax() {
    return this->max;
}

int16_t SliderCanvas::getStep() {
    return this->step;
}

int16_t SliderCanvas::getValue() {
    return this->value;
}

String SliderCanvas::getName() {
    return this->name;
}

String SliderCanvas::getIcon() {
    return this->icon;
}

bool SliderCanvas::getTouched() {
    return this->touched;
}

void SliderCanvas::setDisabled(bool disabled) {
    invalidateIfNotEqual(this->disabled, disabled);
}

bool SliderCanvas::getDisabled() {
    return this->disabled;
}

void SliderCanvas::invalidateValue() {
    invalidateIfNotEqual(this->invalidValue, true);
    this->invalidValue = true;
}

bool SliderCanvas::isValueInvalid() {
    return this->invalidValue;
}