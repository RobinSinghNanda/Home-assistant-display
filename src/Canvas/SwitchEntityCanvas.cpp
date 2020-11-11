#include "SwitchEntityCanvas.hpp"

SwitchEntityCanvas::SwitchEntityCanvas(Canvas* canvas, uint16_t id) : Canvas(canvas, id) {
    rowCanvas = new SwitchEntityRowCanvas(this, 0);
    rowCanvas->setHeight(40);
}

void SwitchEntityCanvas::setIconPath(String iconPath) {
    this->rowCanvas->setIconPath(iconPath);
}

String SwitchEntityCanvas::getIconPath() {
    return this->rowCanvas->getIconPath();
}

void SwitchEntityCanvas::setName(String name) {
    this->rowCanvas->setName(name);
}

String SwitchEntityCanvas::getName() {
    return this->rowCanvas->getName();
}

void SwitchEntityCanvas::setState(bool state) {
    this->rowCanvas->setState(state);
}

bool SwitchEntityCanvas::getState() {
    return this->rowCanvas->getState();
}

void SwitchEntityCanvas::setDisabled(bool disabled) {
    this->rowCanvas->setDisabled(disabled);
}

bool SwitchEntityCanvas::getDisabled() {
    return this->rowCanvas->getDisabled();
}

void SwitchEntityCanvas::setStateColor(bool stateColor) {
    this->rowCanvas->setStateColor(stateColor);
}

bool SwitchEntityCanvas::getStateColor() {   
    return this->rowCanvas->getStateColor();
}

void SwitchEntityCanvas::setSecondaryColor(uint16_t color) {
    this->rowCanvas->setSecondaryColor(color);
}

uint16_t SwitchEntityCanvas::getSecondaryColor() {
    return this->rowCanvas->getSecondaryColor();
}

void SwitchEntityCanvas::setSurfaceColor(uint16_t color) {
    this->rowCanvas->setSurfaceColor(color);
}

uint16_t SwitchEntityCanvas::getSurfaceColor() {
    return this->rowCanvas->getSurfaceColor();
}

void SwitchEntityCanvas::setOnSurfaceColor(uint16_t color) {
    this->rowCanvas->setOnSurfaceColor(color);
}

uint16_t SwitchEntityCanvas::getOnSurfaceColor() {
    return this->rowCanvas->getOnSurfaceColor();
}

void SwitchEntityCanvas::onStateChange(SwitchCanvasStateChangeCallback callback) {
    this->rowCanvas->onStateChange(callback);
}

void SwitchEntityCanvas::onIconTouch(CanvasTouchCallback callback) {
    this->rowCanvas->onIconTouch(callback);
}

void SwitchEntityCanvas::onNameTouch(CanvasTouchCallback callback) {
    this->rowCanvas->onNameTouch(callback);
}