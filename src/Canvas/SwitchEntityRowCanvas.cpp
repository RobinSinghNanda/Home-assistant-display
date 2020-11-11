#include "SwitchEntityRowCanvas.hpp"
using namespace std::placeholders;
SwitchEntityRowCanvas::SwitchEntityRowCanvas(Canvas * canvas, uint16_t id) : BaseEntityRowCanvas(canvas, id){
    stateCanvas = new SwitchCanvas(this, 2);
    this->onStateChangeCallback = [](SwitchCanvas*, bool) -> bool{return false;};
    this->stateCanvas->onStateChange(std::bind(&SwitchEntityRowCanvas::stateChangeCallback, this, _2));
    stateCanvas->setWidth(SWITCH_ICON_WIDTH + SWICTH_ENTITY_MARGIN_RIGHT + SWICTH_ENTITY_MARGIN_RIGHT);
    stateCanvas->alignRight();
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setHAlign(ALIGN_CENTER);
    nameCanvas->horizontalAlign(iconCanvas, stateCanvas);
}

void SwitchEntityRowCanvas::setState(bool state) {
    this->stateCanvas->setState(state);
    this->stateCanvas->setDisabled(false);
    this->setIconColor();
}

bool SwitchEntityRowCanvas::getState() {
    return this->stateCanvas->getState();
}

void SwitchEntityRowCanvas::setDisabled(bool disabled) {
    this->stateCanvas->setDisabled(disabled);
    this->setIconColor();
}

bool SwitchEntityRowCanvas::getDisabled() {
    return this->stateCanvas->getDisabled();
}

void SwitchEntityRowCanvas::onStateChange(SwitchCanvasStateChangeCallback callback) {
    this->onStateChangeCallback = callback;
}

void SwitchEntityRowCanvas::setStateColor(bool stateColor) {
    this->stateColor = this->stateColor;
    this->setIconColor();
}
bool SwitchEntityRowCanvas::getStateColor() {
    return this->stateColor;
}

void SwitchEntityRowCanvas::setSecondaryColor(uint16_t color) {
    this->stateCanvas->setSecondaryColor(color);
}

uint16_t SwitchEntityRowCanvas::getSecondaryColor() {
    return this->stateCanvas->getSecondaryColor();
}

void SwitchEntityRowCanvas::setSurfaceColor(uint16_t color) {
    this->stateCanvas->setSurfaceColor(color);
}

uint16_t SwitchEntityRowCanvas::getSurfaceColor() {
    return this->stateCanvas->getSurfaceColor();
}

void SwitchEntityRowCanvas::setOnSurfaceColor(uint16_t color) {
    this->stateCanvas->setOnSurfaceColor(color);
}

uint16_t SwitchEntityRowCanvas::getOnSurfaceColor() {
    return this->stateCanvas->getOnSurfaceColor();
}

void SwitchEntityRowCanvas::setIconColor() {
    if (this->stateCanvas->getDisabled()) {
        this->iconCanvas->setFgColor(convert2rgb565(0x6f6f6f));
    } else if (this->stateCanvas->getState()){
        this->iconCanvas->setFgColor((this->stateColor)?convert2rgb565(0xfdd835):convert2rgb565(0x44739e));
    } else {
        this->iconCanvas->setFgColor(convert2rgb565(0x44739e)); 
    }
}

bool SwitchEntityRowCanvas::stateChangeCallback (bool state) {
    this->setState(state);
    return this->onStateChangeCallback(this->stateCanvas, state);
}