#ifndef __LIGHTENTITYROWCANVAS_H__
#define __LIGHTENTITYROWCANVAS_H__

#include "SwitchEntityRowCanvas.hpp"

#define LIGHT_MAX_BRIGHTNESS_LEVEL 4

class LightEntityRowCanvas;

typedef std::function<bool(LightEntityRowCanvas*, bool, uint8_t)> LightStateChangeCallback;

class LightEntityRowCanvas : public SwitchEntityRowCanvas {
    public:
        LightEntityRowCanvas(Canvas * canvas, uint16_t id);
        void setBrightness(uint8_t brightness);
        uint8_t getBrightness();
        virtual void setIconPath(String setIconPath);
        void onStateChange(LightStateChangeCallback callback);
    protected:
        uint8_t brightness;
        uint16_t tmpBrightness;
        uint16_t swipeThreshold = 20;
        String actualIcon;
        LightStateChangeCallback onStateChangeCallback;
        bool onTouchEventCallback (TouchEvent event, TouchEventData eventData); 
        void setBrightnessIconPath();
        bool stateChangeCallback (bool state);
};

#endif // __LIGHTENTITYROWCANVAS_H__