#include "SceneEntityRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"

SceneEntityRowCanvas::SceneEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme):
   ButtonEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    stateCanvas->setText("ACTIVATE");
}

SceneEntityRowCanvas::~SceneEntityRowCanvas() {

}