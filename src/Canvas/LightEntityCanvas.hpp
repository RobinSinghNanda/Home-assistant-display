#ifndef __LIGHTENTITYCANVAS_H__
#define __LIGHTENTITYCANVAS_H__

#include "SwitchEntityCanvas.hpp"
#include "CustomSliderCanvas.hpp"

#define LIGHT_MAX_COLORS 24

class LightEntityCanvas;

typedef std::function<bool(LightEntityCanvas*, bool)> LightEntityCanvasStateChangeCallback;

class LightEntityCanvas : public SwitchEntityCanvas {
    public:
        LightEntityCanvas(Canvas * canvas, uint16_t id);
        void setBrighnessSliderEnabled(bool brightnessSliderEnabled);
        void setColorTemperatureSliderEnabled(bool colorTemperatureSliderEnabled);
        void setColorSliderEnabled(bool colorSliderEnabled);
        void setWhiteValueSliderEnabled(bool whiteValueSliderEnabled);
        bool getBrighnessSliderEnabled();
        bool getColorTemperatureSliderEnabled();
        bool getColorSliderEnabled();
        bool getWhiteValueSliderEnabled();
        void setBrightness(uint8_t brightness);
        uint8_t getBrightness();
        void setWhiteValue(uint8_t whiteValue);
        uint8_t getWhiteValue();
        void setColorTemperature(uint16_t colorTemperature);
        uint16_t getColorTemperature();
        void setColor(uint32_t color);
        void setColor(uint8_t red, uint8_t green, uint8_t blue);
        uint32_t getColor();
        void setMinColorTemperature(uint16_t minColorTemperature);
        void setMaxColorTemperature(uint16_t maxColorTemperature);
        uint16_t getMinColorTemperature();
        uint16_t getMaxColorTemperature();
        virtual void setDisabled(bool disabled);
        void onStateChange(LightEntityCanvasStateChangeCallback callback);
    protected:
        uint32_t color;
        bool brightnessSliderEnabled;
        bool colorTemperatureSliderEnabled;
        bool colorSliderEnabled;
        bool whiteValueSliderEnabled;
        CustomSliderCanvas * brightnessSlider;
        CustomSliderCanvas * colorTemperatureSlider;
        CustomSliderCanvas * colorSlider;
        CustomSliderCanvas * whiteSlider;
        LightEntityCanvasStateChangeCallback stateChangeCallback;
        void setLayout();
        uint32_t findNearestColor(uint32_t color);
};

#endif // __LIGHTENTITYCANVAS_H__