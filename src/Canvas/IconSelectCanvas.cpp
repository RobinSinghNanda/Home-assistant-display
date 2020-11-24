#include "IconSelectCanvas.hpp"

IconSelectCanvas::IconSelectCanvas(Canvas * canvas, uint16_t id) : SelectCanvas(canvas, id) {
    valueCanvas = new IconCanvas(this, 2);
    valueCanvas->setHAlign(ALIGN_CENTER);
    valueCanvas->setVAlign(ALIGN_MIDDLE);
    valueCanvas->setWidth(40);
    setValueCanvas(valueCanvas);
}

IconSelectCanvas::~IconSelectCanvas() {

}

void IconSelectCanvas::setValue(const char * value) {
    this->valueCanvas->setPath(value);
    resetLayout();
}