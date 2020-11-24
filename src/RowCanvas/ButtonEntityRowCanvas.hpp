#ifndef __BUTTONENTITYROWCANVAS_H__
#define __BUTTONENTITYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "HomeAssistant/Components/Script.hpp"

class ButtonEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        ButtonEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        ~ButtonEntityRowCanvas();
        virtual void update();
    protected:
        bool touched = false;
        int8_t tmpIndex = 0;
        TextCanvas * stateCanvas;
};

#endif // __BUTTONENTITYROWCANVAS_H__