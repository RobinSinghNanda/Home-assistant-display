#ifndef __SWITCHCARDCANVAS_H__
#define __SWITCHCARDCANVAS_H__

#include "ToggleCardCanvas.hpp"
#include "LovelaceConfig/SwitchCardConfig.hpp"
#include "HomeAssistant/Components/Switch.hpp"

class SwitchCardCanvas : public ToggleCardCanvas {
    public:
        SwitchCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, SwitchCardConfig * cardConfig, LcdTheme * theme);
        ~SwitchCardCanvas();
    protected:
        Switch * entity;
        SwitchCardConfig * cardConfig;
};

#endif // __SWITCHCARDCANVAS_H__