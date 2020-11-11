#include "SwitchCanvas.hpp"

using namespace std::placeholders;

SwitchCanvas::SwitchCanvas(Canvas * canvas, uint16_t id) : ImageCanvas(canvas, id) {
    this->state = false;
    this->disabled = true;
    this->onStateChangeCallback = [](SwitchCanvas*, bool)->bool{return false;};
    this->setImagePath();
    this->maskColor = TFT_WHITE;
    this->secondaryColor = this->convert2rgb565(0x03a9f4);
    this->surfaceColor = TFT_WHITE;
    this->onSurfaceColor = TFT_BLACK;
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
        this->ImageCanvas::setPath(SWITCH_ICON_UNAVAIABLE);
        this->setFgColor(surfaceColor);
        this->setMaskColor(onSurfaceColor); 
    } else if (this->state) {
        this->ImageCanvas::setPath(SWITCH_ICON_ON);
        this->setFgColor(secondaryColor);
    } else {
        this->ImageCanvas::setPath(SWITCH_ICON_OFF);
        this->setFgColor(surfaceColor);
        this->setMaskColor(onSurfaceColor); 
    }
}

void SwitchCanvas::setSecondaryColor(uint16_t color){
    this->secondaryColor = color;
    setImagePath();
}

uint16_t SwitchCanvas::getSecondaryColor() {
    return this->secondaryColor;
}

void SwitchCanvas::setSurfaceColor(uint16_t color) {
    this->surfaceColor = color;
    setImagePath();
}

uint16_t SwitchCanvas::getSurfaceColor() {
    return this->surfaceColor;
}

void SwitchCanvas::setOnSurfaceColor(uint16_t color) {
    this->onSurfaceColor = color;
    setImagePath();
}

uint16_t SwitchCanvas::getOnSurfaceColor() {
    return this->onSurfaceColor;
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