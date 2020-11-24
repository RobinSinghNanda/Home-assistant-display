#ifndef __LIGHTENTITYROWCANVAS_H__
#define __LIGHTENTITYROWCANVAS_H__

#include "SwitchEntityRowCanvas.hpp"
#include "HomeAssistant/Components/Light.hpp"

#define LIGHT_MAX_BRIGHTNESS_LEVEL 4

class LightEntityRowCanvas;

class LightEntityRowCanvas : public ToggleEntityRowCanvas {
    public:
        LightEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        static constexpr int SWIPE_THRESHOLD = 20;
        virtual void update();
    protected:
        Light * entity = nullptr;
        uint8_t brightness;
        uint16_t tmpBrightness;
        String actualIcon;
        bool onTouchEventCallback (TouchEvent event, TouchEventData eventData); 
        void setBrightnessIconPath();
        bool stateChangeCallback (bool state);
};

#endif // __LIGHTENTITYROWCANVAS_H__