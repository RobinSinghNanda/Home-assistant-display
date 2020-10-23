#include "CustomSliderCanvas.hpp"

CustomSliderCanvas::CustomSliderCanvas(Canvas * canvas, uint16_t id) : SliderCanvas(canvas, id) {
    drawCallback = [](CustomSliderCanvas*, TFT_eSPI*) ->bool{return false;};
}

bool CustomSliderCanvas::draw() {
    return this->drawCallback(this, this->tft);
}

void CustomSliderCanvas::onDraw(SliderDrawCallback callback) {
    this->drawCallback = callback;
}