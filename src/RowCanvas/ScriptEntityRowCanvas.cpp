#include "ScriptEntityRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"

ScriptEntityRowCanvas::ScriptEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme):
   ButtonEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    stateCanvas->setText("EXECUTE");
}

ScriptEntityRowCanvas::~ScriptEntityRowCanvas() {

}