#ifndef __TOGGLEENTITYROWCANVAS_H__
#define __TOGGLEENTITYROWCANVAS_H__


#include "BaseEntityRowCanvas.hpp"

class ToggleEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        ToggleEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        virtual void update();
    protected:
        SwitchCanvas * stateCanvas;
};


#endif // __TOGGLEENTITYROWCANVAS_H__