#ifndef __CLIMATEENTITYROWCANVAS_H__
#define __CLIMATEENTITYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "HomeAssistant/Components/Climate.hpp"
#include "Canvas/TextSelectCanvas.hpp"

class ClimateEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        ClimateEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        virtual ~ClimateEntityRowCanvas();
        static constexpr int SWIPE_THRESHOLD = 20;
        virtual void update();
    protected:
        TextSelectCanvas * modeSelectCanvas;
        bool touched = false;
        float temperature = 0.0;
        TextCanvas * textCanvas;
        Climate * entity;
        void setTemperatureState();
        void resetState();
};

#endif // __CLIMATEENTITYROWCANVAS_H__