#include "SwitchEntityRowCanvas.hpp"

SwitchEntityRowCanvas::SwitchEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    ToggleEntityRowCanvas(canvas, id, hass, rowConfig, theme){
}