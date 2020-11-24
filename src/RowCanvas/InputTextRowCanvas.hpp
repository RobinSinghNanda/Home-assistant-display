#ifndef __INPUTTEXTROWCANVAS_H__
#define __INPUTTEXTROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "HomeAssistant/Components/InputText.hpp"

class InputTextRowCanvas : public BaseEntityRowCanvas {
    public:
        InputTextRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        ~InputTextRowCanvas();
        virtual void update();
    protected:
        int8_t tmpIndex = 0;
        InputText * entity;
        TextCanvas * textCanvas;
        string value = "";
};

#endif // __INPUTTEXTROWCANVAS_H__