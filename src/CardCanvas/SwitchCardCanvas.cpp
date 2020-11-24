#include "SwitchCardCanvas.hpp"
#include "LovelaceConfig/SwitchCardConfig.hpp"

SwitchCardCanvas::SwitchCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, SwitchCardConfig * cardConfig, LcdTheme * theme) :
    ToggleCardCanvas(canvas, id, hass, cardConfig, theme) {
}

SwitchCardCanvas::~SwitchCardCanvas() {
    
}