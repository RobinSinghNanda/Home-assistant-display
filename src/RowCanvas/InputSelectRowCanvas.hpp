#ifndef __INPUTSELECTROWCANVAS_H__
#define __INPUTSELECTROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "HomeAssistant/Components/InputSelect.hpp"
#include "Canvas/TextSelectCanvas.hpp"

class InputSelectRowCanvas : public BaseEntityRowCanvas {
    public:
        InputSelectRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        ~InputSelectRowCanvas();
        static constexpr int SWIPE_THRESHOLD = 20;
        static constexpr int STATE_WIDTH = 80;
        virtual void update();
    protected:
        bool touched = false;
        int8_t tmpIndex = 0;
        InputSelect * entity;
        TextSelectCanvas * selectCanvas;
};


#endif // __INPUTSELECTROWCANVAS_H__