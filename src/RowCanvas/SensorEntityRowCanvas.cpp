#include "SensorEntityRowCanvas.hpp"
#include "Fonts.hpp"

SensorEntityRowCanvas::SensorEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    stateCanvas = new TextCanvas(this, 2);
    stateCanvas->setWidth(this->getWidth()/3);
    stateCanvas->hAlignRight();
    stateCanvas->setPaddingRight(10);
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setHAlign(ALIGN_RIGHT);
    stateCanvas->setFont(FONT_REGULAR_24);
    nameCanvas->hAlignBetween(iconCanvas, stateCanvas);
    iconCanvas->setFgColor(ICON_PRIMARY_COLOR);
}

void SensorEntityRowCanvas::setState(String state) {
    if (state.length() > 10) {
        this->stateCanvas->setFont(FONT_REGULAR_12);
    } else {
        this->stateCanvas->setFont(FONT_REGULAR_24);
    }
    this->stateCanvas->setText(state);
}

String SensorEntityRowCanvas::getState() {
    return this->stateCanvas->getText();
}

void SensorEntityRowCanvas::setFgColor(Color16Bit fgColor) {
    this->BaseRowCanvas::setFgColor(fgColor);
    this->stateCanvas->setFgColor(fgColor);
}

void SensorEntityRowCanvas::update() {
    BaseEntityRowCanvas::update();
    const char * state = entity->getState();
    if (strlen(state) > 10) {
        this->stateCanvas->setFont(FONT_REGULAR_12);
    } else {
        this->stateCanvas->setFont(FONT_REGULAR_24);
    }
    this->stateCanvas->setText(state);
    uint16_t width = tft->textWidth(state)+10;
    if (width < this->getWidth()/2) {
        this->stateCanvas->setWidth(width);
    } else {
        this->stateCanvas->setWidth(this->getWidth()/2);
    }
    this->stateCanvas->hAlignRight();
    this->nameCanvas->hAlignBetween(iconCanvas, stateCanvas);
    
}