#include "SwitchCanvas.hpp"

using namespace std::placeholders;

SwitchCanvas::SwitchCanvas(Canvas * canvas, uint16_t id) : ImageCanvas(canvas, id) {
    this->state = false;
    this->disabled = true;
    this->onStateChangeCallback = [](SwitchCanvas*, bool)->bool{return false;};
    this->setImagePath();
    onTouch(std::bind(&SwitchCanvas::onTouchEventCallback, this, _2, _3));
}

SwitchCanvas::SwitchCanvas(ImageCanvas * canvas, uint16_t id) : ImageCanvas(canvas, id) {
    this->state = false;
    this->disabled = true;
    this->onStateChangeCallback = [](SwitchCanvas*, bool)->bool{return false;};
    this->setImagePath();
    onTouch(std::bind(&SwitchCanvas::onTouchEventCallback, this, _2, _3));
}

SwitchCanvas:: SwitchCanvas(SwitchCanvas * canvas, uint16_t id): ImageCanvas(canvas, id) {
    this->state = canvas->state;
    this->disabled = canvas->disabled;
    this->onStateChangeCallback = [](SwitchCanvas*, bool) -> bool{return false;};
    this->setImagePath();
    onTouch(std::bind(&SwitchCanvas::onTouchEventCallback, this, _2, _3));
}

void SwitchCanvas::setState(bool state) {
    if (this->state != state) {
        this->state = state;
        this->setImagePath();
        this->invalidate();
    }
}

bool SwitchCanvas::getState() {
    return this->state;
}

void SwitchCanvas::setDisabled(bool disabled) {
    bool prev_disabled = this->disabled;
    this->disabled = disabled;
    if (prev_disabled != disabled) {
        this->setImagePath();
        this->invalidate();
    }
}

bool SwitchCanvas::getDisabled() {
    return this->disabled;
}

void SwitchCanvas::resetIcon() {
    setImagePath();
    this->invalidate();
}



bool SwitchCanvas::draw() {
  #ifdef CANVAS_DEBUG
    this->drawDebugBorder();
  #endif
  drawBackground();
  return this->ImageCanvas::draw();
}

void SwitchCanvas::setImagePath() {
    if (this->disabled) {
      this->ImageCanvas::setPath((this->darkMode)?SWITCH_ICON_UNAVAIABLE_DARK:SWITCH_ICON_UNAVAIABLE_LIGHT);
    } else if (this->state) {
      this->ImageCanvas::setPath((this->darkMode)?SWITCH_ICON_ON_DARK:SWITCH_ICON_ON_LIGHT);
    } else {
      this->ImageCanvas::setPath((this->darkMode)?SWITCH_ICON_OFF_DARK:SWITCH_ICON_OFF_LIGHT);
    }
}

void SwitchCanvas::onStateChange(SwitchCanvasStateChangeCallback callback) {
    this->onStateChangeCallback = callback;
}

bool SwitchCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (event == TouchActionTapped || event == TouchActionLongPressed ) {
        if (!this->disabled) {
            this->setState(!this->getState());
            return this->onStateChangeCallback(this, this->state);
        }
        return true;
    }
    return false;
}

void SwitchCanvas::setDarkMode(bool darkMode) {
    invalidateIfNotEqual(this->darkMode, darkMode);
    this->darkMode = darkMode;
    this->setImagePath();
    this->invalidate();
}