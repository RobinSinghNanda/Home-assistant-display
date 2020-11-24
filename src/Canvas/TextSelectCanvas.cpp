#include "TextSelectCanvas.hpp"

TextSelectCanvas::TextSelectCanvas(Canvas * canvas, uint16_t id) : SelectCanvas(canvas, id) {
    valueCanvas = new TextCanvas(this, 2);
    valueCanvas->setHAlign(ALIGN_CENTER);
    valueCanvas->setVAlign(ALIGN_MIDDLE);
    valueCanvas->setWidth(40);
    setValueCanvas(valueCanvas);
    this->valueCanvas->setMaxWidth(this->width - leftButtonCanvas->getWidth() - rightButtonCanvas->getWidth());
}

TextSelectCanvas::~TextSelectCanvas() {

}

void TextSelectCanvas::resetLayout() {
    this->valueCanvas->setMaxWidth(this->width - leftButtonCanvas->getWidth() - rightButtonCanvas->getWidth());
    this->valueCanvas->resize();
    SelectCanvas::resetLayout();
}

void TextSelectCanvas::setValue(const char * value) {
    this->valueCanvas->setText(value);
    if (valueCanvas->isInvalid()) {
        resetLayout();
    }
}