#include "BaseRowCanvas.hpp"

BaseRowCanvas::BaseRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, BaseRowConfig * rowConfig, LcdTheme * theme) :
    Canvas(canvas, id) {
    this->height = HEIGHT;
    this->hass = hass;
    this->theme = theme;
};

void BaseRowCanvas::update() {

}