#include "FanEntityRowCanvas.hpp"

using namespace std::placeholders;

FanEntityRowCanvas::FanEntityRowCanvas(Canvas * canvas, uint16_t id) : SwitchEntityRowCanvas(canvas, id){
    this->speed = 0;
    this->tmpSpeed = 0;
    this->actualIcon = "";
    this->setFanIconPath();
    swipeThreshold = 20;
    onTouch(std::bind(&FanEntityRowCanvas::onTouchEventCallback, this, _2, _3));
    this->onStateChangeCallback = [](FanEntityRowCanvas*, bool, uint16_t) -> bool{return false;};
    this->stateCanvas->onStateChange(std::bind(&FanEntityRowCanvas::stateChangeCallback, this, _2));
    this->stateColor = false;
}

void FanEntityRowCanvas::setSpeed(uint16_t speed) {
    returnIfEqual(this->speed, speed);
    tmpSpeed = speed;
    this->speed = speed;
    if (speed <= 0) {
        this->setState(false);
    } else {
        this->setState(true);
    }
}

void FanEntityRowCanvas::setStateColor(bool stateColor) {

}

void FanEntityRowCanvas::setFanIconPath() {
    if (tmpSpeed == 0){
        this->stateCanvas->setPath("mdi:fan-off");
    } else if (tmpSpeed == 1) {
        this->stateCanvas->setPath("mdi:fan-speed-1");
    } else if (tmpSpeed == 2) {
        this->stateCanvas->setPath("mdi:fan-speed-2");
    } else if (tmpSpeed == 3) {
        this->stateCanvas->setPath("mdi:fan-speed-3");
    } else {
        this->stateCanvas->setPath("mdi:fan-off");
    }
}

uint16_t FanEntityRowCanvas::getSpeed(){
    return this->speed;
}

void FanEntityRowCanvas::setIconPath(String iconPath) {
    this->actualIcon = iconPath;
    this->iconCanvas->setPath(iconPath);
}

bool FanEntityRowCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (isEvent(event, TouchActionLongPressedAndDragged)) {
        int16_t deltaSpeed = (eventData.endX -eventData.startX)/swipeThreshold;
        uint16_t currSpeed = speed;
        if (getState()) {
            currSpeed = speed;
        } else {
            currSpeed = 0;
        }
        int16_t nextSpeed = currSpeed + deltaSpeed;
        if (nextSpeed < 0) {
            nextSpeed = 0;
        } else if (nextSpeed >= 3) {
            nextSpeed = 3;
        }
        tmpSpeed = nextSpeed;
        this->setFanIconPath();
        this->stateCanvas->redraw();
        return true;
    } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
        this->setSpeed(tmpSpeed);
        this->stateCanvas->resetIcon();
        this->stateCanvas->redraw();
        this->onStateChangeCallback(this, getState(), speed);
        return true;
    } else if (isEvent(event, TouchActionLongPressed)) {
        if (getState()) {
            tmpSpeed = speed;
        } else {
            tmpSpeed = 0;
        }
        this->setFanIconPath();
        this->stateCanvas->redraw();
        return true;
    } else if (isEvent(event, TouchActionLongPressReleased)) {
        if (tmpSpeed != this->speed) {
            this->setSpeed(tmpSpeed);
            this->stateCanvas->resetIcon();
            this->stateCanvas->redraw();
            this->onStateChangeCallback(this, getState(), speed);
        } else {
            this->stateCanvas->resetIcon();
            this->stateCanvas->redraw();
        }
        return true;
    }
    return false;
}

void FanEntityRowCanvas::onStateChange(FanStateChangeCallback callback) {
    this->onStateChangeCallback = callback;
}

bool FanEntityRowCanvas::stateChangeCallback (bool state) {
    this->setState(state);
    return this->onStateChangeCallback(this, state, speed);
}