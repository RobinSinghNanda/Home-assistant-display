#ifndef __FANCARDCANVAS_H__
#define __FANCARDCANVAS_H__

#include "ToggleCardCanvas.hpp"
#include "Canvas/SliderCanvas.hpp"
#include "LovelaceConfig/FanCardConfig.hpp"
#include "HomeAssistant/Components/Fan.hpp"

class FanEntityCanvas;

class FanCardCanvas : public ToggleCardCanvas {
    public: 
        FanCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, FanCardConfig * cardConfig, LcdTheme * theme);
        ~FanCardCanvas();
        virtual void update();
    protected:
        Fan * entity;
        FanCardConfig * cardConfig;
        SliderCanvas * fanSpeedSlider;
};

#endif // __FANCARDCANVAS_H__