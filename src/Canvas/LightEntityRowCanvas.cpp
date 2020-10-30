#include "LightEntityRowCanvas.hpp"

using namespace std::placeholders;

LightEntityRowCanvas::LightEntityRowCanvas(Canvas * canvas, uint16_t id) :SwitchEntityRowCanvas(canvas, id) {
    this->brightness = 0;
    this->tmpBrightness = 0;
    this->actualIcon = "";
    this->setBrightnessIconPath();
    swipeThreshold = 20;
    onTouch(std::bind(&LightEntityRowCanvas::onTouchEventCallback, this, _2, _3));
    this->onStateChangeCallback = [](LightEntityRowCanvas*, bool, uint16_t) -> bool{return false;};
    this->stateCanvas->onStateChange(std::bind(&LightEntityRowCanvas::stateChangeCallback, this, _2));
    this->stateColor = true;
}

void LightEntityRowCanvas::setBrightness(uint8_t brightness) {
    invalidateIfNotEqual(this->brightness, brightness);
    tmpBrightness = brightness;
    this->brightness = brightness;
    if (brightness <= 0) {
        this->setState(false);
    } else {
        this->setState(true);
    }
}

uint8_t LightEntityRowCanvas::getBrightness() {
    return this->brightness;
}

void LightEntityRowCanvas::setIconPath(String iconPath) {
    this->actualIcon = iconPath;
    this->iconCanvas->setPath(iconPath);
}

void LightEntityRowCanvas::onStateChange(LightStateChangeCallback callback) {
    this->onStateChangeCallback = callback;
}

bool LightEntityRowCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (isEvent(event, TouchActionLongPressedAndDragged)) {
        int16_t deltaBrightness = (eventData.endX -eventData.startX)/swipeThreshold;
        uint16_t currBrightness = brightness;
        if (getState()) {
            currBrightness = brightness;
            if (brightness > 192) {
                currBrightness = 255;
            } else if (brightness > 128) {
                currBrightness = 191;
            } else if (brightness > 64) {
                currBrightness = 127;
            } else if (brightness > 0) {
                currBrightness = 63;
            } else {
                currBrightness = 0;
            }
        } else {
            currBrightness = 0;
        }
        int16_t nextBrightness = currBrightness + ((deltaBrightness)<<6);
        if (nextBrightness > 192) {
            nextBrightness = 255;
        } else if (nextBrightness > 128) {
            nextBrightness = 191;
        } else if (nextBrightness > 64) {
            nextBrightness = 127;
        } else if (nextBrightness > 0) {
            nextBrightness = 63;
        } else {
            nextBrightness = 0;
        }
        tmpBrightness = nextBrightness;
        this->setBrightnessIconPath();
        this->stateCanvas->redraw();
        return true;
    } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
        if (tmpBrightness != this->brightness) {
            this->setBrightness(tmpBrightness);
            this->stateCanvas->resetIcon();
            this->stateCanvas->redraw();
            this->onStateChangeCallback(this, getState(), brightness);
            this->stateCanvas->invalidate();
        } else {
            this->stateCanvas->resetIcon();
            this->stateCanvas->redraw();
        }
        return true;
    } else if (isEvent(event, TouchActionLongPressed)) {
        if (getState()) {
            tmpBrightness = brightness;
        } else {
            tmpBrightness = 0;
        }
        this->setBrightnessIconPath();
        this->stateCanvas->redraw();
        return true;
    } else if (isEvent(event, TouchActionLongPressReleased)) {
        if (tmpBrightness != this->brightness) {
            this->setBrightness(tmpBrightness);
            this->stateCanvas->resetIcon();
            this->stateCanvas->redraw();
            this->onStateChangeCallback(this, getState(), brightness);
            this->stateCanvas->invalidate();
        } else {
            this->stateCanvas->resetIcon();
            this->stateCanvas->redraw();
        }
        return true;
    }
    return false;
}

void LightEntityRowCanvas::setBrightnessIconPath() {
    if (tmpBrightness == 0) {
        this->stateCanvas->setPath("mdi:light-off");
    } else if (tmpBrightness < 0x40) {
        this->stateCanvas->setPath("mdi:light-1");
    } else if (tmpBrightness < 0x80) {
        this->stateCanvas->setPath("mdi:light-2");
    } else if (tmpBrightness < 0xC0) {
        this->stateCanvas->setPath("mdi:light-3");
    } else if (tmpBrightness <= 0xFF) {
        this->stateCanvas->setPath("mdi:light-4");
    } else {
        this->stateCanvas->setPath("mdi:light-off");
    }
}

bool LightEntityRowCanvas::stateChangeCallback (bool state) {
    this->setState(state);
    return this->onStateChangeCallback(this, state, brightness);
}

void LightEntityRowCanvas::setFgColor(uint16_t fgColor) {
    this->SwitchEntityRowCanvas::setFgColor(fgColor);
    this->stateCanvas->setFgColor(fgColor);
}