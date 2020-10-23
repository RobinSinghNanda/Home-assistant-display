#include "SensorEntityRowCanvas.hpp"

SensorEntityRowCanvas::SensorEntityRowCanvas(Canvas * canvas, uint16_t id) : BaseEntityRowCanvas(canvas, id) {
    stateCanvas = new TextCanvas(this, 2);
    stateCanvas->setWidth(SWITCH_ICON_WIDTH + SWICTH_ENTITY_MARGIN_RIGHT + SWICTH_ENTITY_MARGIN_RIGHT);
    stateCanvas->alignRight();
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setHAlign(ALIGN_CENTER);
    nameCanvas->horizontalAlign(iconCanvas, stateCanvas);
    iconCanvas->setColorMask(convert2rgb565(0x44739e));
}

void SensorEntityRowCanvas::setState(String state) {
    invalidateIfNotEqual(this->stateCanvas->getText(), state);
    this->stateCanvas->setText(state);
}

String SensorEntityRowCanvas::getState() {
    return this->stateCanvas->getText();
}