#ifndef __LIGHTCARDCANVAS_H__
#define __LIGHTCARDCANVAS_H__

#include "Canvas/CustomSliderCanvas.hpp"
#include "ToggleCardCanvas.hpp"
#include "LovelaceConfig/LightCardConfig.hpp"
#include "HomeAssistant/Components/Light.hpp"

#define LIGHT_MAX_COLORS 24

class LightCardCanvas : public ToggleCardCanvas {
    public:
        LightCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, LightCardConfig * cardConfig, LcdTheme * theme);
        ~LightCardCanvas();
        virtual void update();
    protected:
        Light * entity;
        uint32_t color;
        CustomSliderCanvas * brightnessSlider;
        CustomSliderCanvas * colorTemperatureSlider;
        CustomSliderCanvas * colorSlider;
        CustomSliderCanvas * whiteSlider;
        void setLayout();
        uint32_t findNearestColor(uint32_t color);
        void setState(bool state);
        void setColor(uint32_t color);
};

#endif // __LIGHTCARDCANVAS_H__