#ifndef __INPUTNUMBERROWCANVAS_H__
#define __INPUTNUMBERROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "HomeAssistant/Components/InputNumber.hpp"
#include "Canvas/TextSelectCanvas.hpp"

class InputNumberRowCanvas : public BaseEntityRowCanvas {
    public:
        InputNumberRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        ~InputNumberRowCanvas();
        static constexpr int SWIPE_THRESHOLD = 20;
        static constexpr int STATE_WIDTH = 80;
        virtual void update();
    protected:
        bool touched = false;
        float tmpValue = 0;
        InputNumber * entity;
        TextSelectCanvas * selectCanvas;
};

#endif // __INPUTNUMBERROWCANVAS_H__