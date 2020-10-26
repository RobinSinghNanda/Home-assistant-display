#ifndef __FANENTITYCANVAS_H__
#define __FANENTITYCANVAS_H__

#include "SwitchEntityCanvas.hpp"
#include "CustomSliderCanvas.hpp"

class FanEntityCanvas;

typedef std::function<bool(FanEntityCanvas*, bool)> FanEntityCanvasStateChangeCallback;

class FanEntityCanvas : public SwitchEntityCanvas {
    public: 
        FanEntityCanvas(Canvas * canvas, uint16_t id);
        void setFanSpeedSliderEnabled(bool fanSpeedSliderEnabled);
        bool getFanSpeedSliderEnabled();
        void setFanSpeed(uint8_t fanSpeed);
        uint8_t getFanSpeed();
        virtual void setDisabled(bool disabled);
        virtual void setState(bool state);
        void onStateChange(FanEntityCanvasStateChangeCallback callback);
    protected:
        uint8_t fanSpeed;
        bool fanSpeedSliderEnabled;
        SliderCanvas * fanSpeedSlider;
        FanEntityCanvasStateChangeCallback stateChangeCallback;
};

#endif // __FANENTITYCANVAS_H__