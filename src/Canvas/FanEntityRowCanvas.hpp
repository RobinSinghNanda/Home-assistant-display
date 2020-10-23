#ifndef __FANENTITYROWCANVAS_H__
#define __FANENTITYROWCANVAS_H__

#include "SwitchEntityRowCanvas.hpp"

class FanEntityRowCanvas;

typedef std::function<bool(FanEntityRowCanvas*, bool, uint16_t)> FanStateChangeCallback;

class FanEntityRowCanvas : public SwitchEntityRowCanvas {
    public:
        FanEntityRowCanvas(Canvas * canvas, uint16_t id);
        void setSpeed(uint16_t speed);
        uint16_t getSpeed();
        virtual void setIconPath(String setIconPath);
        void onStateChange(FanStateChangeCallback callback);
        virtual void setStateColor(bool stateColor);
    protected:
        uint16_t speed;
        uint16_t tmpSpeed;
        uint16_t swipeThreshold = 20;
        String actualIcon;
        FanStateChangeCallback onStateChangeCallback;
        bool onTouchEventCallback (TouchEvent event, TouchEventData eventData); 
        void setFanIconPath();
        bool stateChangeCallback (bool state);
};

#endif // __FANENTITYROWCANVAS_H__