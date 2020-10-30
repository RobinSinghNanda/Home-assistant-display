#include "SensorEntityRowCanvas.hpp"

SensorEntityRowCanvas::SensorEntityRowCanvas(Canvas * canvas, uint16_t id) : BaseEntityRowCanvas(canvas, id) {
    stateCanvas = new TextCanvas(this, 2);
    stateCanvas->setWidth(SWITCH_ICON_WIDTH + SWICTH_ENTITY_MARGIN_RIGHT + SWICTH_ENTITY_MARGIN_RIGHT);
    stateCanvas->alignRight();
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setHAlign(ALIGN_CENTER);
    stateCanvas->setFont("Roboto-Regular24");
    nameCanvas->horizontalAlign(iconCanvas, stateCanvas);
    iconCanvas->setFgColor(ICON_PRIMARY_COLOR);
}

void SensorEntityRowCanvas::setState(String state) {
    invalidateIfNotEqual(this->stateCanvas->getText(), state);
    this->stateCanvas->setText(state);
}

String SensorEntityRowCanvas::getState() {
    return this->stateCanvas->getText();
}

void SensorEntityRowCanvas::setFgColor(uint16_t fgColor) {
    this->BaseEntityRowCanvas::setFgColor(fgColor);
    this->stateCanvas->setFgColor(fgColor);
}