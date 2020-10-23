#include "SliderCanvas.hpp"

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
            this->draw();
            onValueChangeCallback(this, this->value, this->prevValue);
            this->prevValue = value;
        }
        return true;
    } else if (event & TouchActionDraged) {
        if (this->disabled)
            return true;
        touched = 1;
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
            this->prevDragValue = this->value;
            this->draw();
        }
        return true;
    } else if (event & TouchActionDragReleased) {
        if (this->disabled)
            return true;
        touched = 0;
        this->draw();
        if (this->prevValue != this->value) {
            onValueChangeCallback(this, this->value, this->prevValue);
            this->prevValue = this->value;
        }
        return true;
    }
    return false;
}

bool SliderCanvas::draw() {
    uint16_t radius = (this->touched)?SLIDER_TOUCHED_RADIUS:SLIDER_RADIUS;
    uint16_t knobColor = (this->getDisabled()?this->convert2rgb565(0x919191):this->convert2rgb565(SLIDER_KNOB_COLOR));
    tft->fillRect(
            this->getDrawX() - 2*SLIDER_TOUCHED_RADIUS,
            this->getDrawY() + this->getDrawableHeight()/2-2*SLIDER_TOUCHED_RADIUS,
            this->getDrawableWidth() + 4 * SLIDER_TOUCHED_RADIUS,
            SLIDER_TOUCHED_RADIUS*4,
            TFT_WHITE);
    uint16_t circle_x = this->getDrawX() + (this->value - this->min)*(this->getDrawableWidth())/(this->max-this->min);
    tft->fillRect(this->getDrawX(),
        this->getDrawY() + this->getDrawableHeight()/2,
        circle_x - this->getDrawX(),
        SLIDER_HEIGHT,
        this->knob_color);
        tft->fillRect(circle_x,
        this->getDrawY() + this->getDrawableHeight()/2,
        this->x + this->width - this->marginRight - circle_x,
        SLIDER_HEIGHT,
        this->convert2rgb565(0x6f6f6f));
    if (!this->isValueInvalid()) {
        tft->fillCircle(
                circle_x,
                this->getDrawY() + this->getDrawableHeight()/2,
                radius,
                knobColor);
    }
    return true;
}


void SliderCanvas::onValueChange(SliderCanvasValueChangeCallback callback) {
    onValueChangeCallback = callback;
}

void SliderCanvas::setMin(int16_t min) {
    if (this->min != min) {
        this->min = min;
        if (this->value < this->min) {
            this->value = this->min;
        }
        this->invalidate();
    }
}

void SliderCanvas::setMax(int16_t max) {
    if (this->max != max) {
        this->max = max;
        if (this->value > this->max) {
            this->value = this->max -1;
        }
        this->invalidate();
    }
}

void SliderCanvas::setStep(int16_t step) {
    if (this->step != step) {
        this->step = step;
        this->invalidate();
    }
}

void SliderCanvas::setValue(int16_t value) {
    if (this->value != value) {
        this->value = value;
        this->invalidValue = false;
        this->invalidate();
    }
}

void SliderCanvas::setName(String name) {
    if (this->name != name) {
        this->name = name;
        this->invalidate();
    }
}

void SliderCanvas::setIcon(String icon) {
    if (this->icon != icon) {
        this->icon = icon;
        this->invalidate();
    }
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
    this->disabled = disabled;
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