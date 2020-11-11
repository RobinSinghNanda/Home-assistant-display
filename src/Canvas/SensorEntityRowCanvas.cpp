#include "SensorEntityRowCanvas.hpp"

SensorEntityRowCanvas::SensorEntityRowCanvas(Canvas * canvas, uint16_t id) : BaseEntityRowCanvas(canvas, id) {
    stateCanvas = new TextCanvas(this, 2);
    stateCanvas->setWidth(this->getWidth()/3);
    stateCanvas->alignRight();
    stateCanvas->setMarginRight(10);
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setHAlign(ALIGN_RIGHT);
    stateCanvas->setFont("Roboto-Regular24");
    nameCanvas->horizontalAlign(iconCanvas, stateCanvas);
    iconCanvas->setFgColor(ICON_PRIMARY_COLOR);
}

void SensorEntityRowCanvas::setState(String state) {
    if (state.length() > 10) {
        this->stateCanvas->setFont("Roboto-Regular12");
    } else {
        this->stateCanvas->setFont("Roboto-Regular24");
    }
    this->stateCanvas->setText(state);
}

String SensorEntityRowCanvas::getState() {
    return this->stateCanvas->getText();
}

void SensorEntityRowCanvas::setFgColor(uint16_t fgColor) {
    this->BaseEntityRowCanvas::setFgColor(fgColor);
    this->stateCanvas->setFgColor(fgColor);
}