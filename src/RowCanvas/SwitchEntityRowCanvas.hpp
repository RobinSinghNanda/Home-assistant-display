#ifndef __SWITCHENTTIYROWCANVAS_H__
#define __SWITCHENTTIYROWCANVAS_H__

#include "ToggleEntityRowCanvas.hpp"

class SwitchEntityRowCanvas : public ToggleEntityRowCanvas {
    public:
        SwitchEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
};

#endif // __SWITCHENTTIYROWCANVAS_H__