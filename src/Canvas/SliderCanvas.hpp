#ifndef SLIDER_CANVAS_
#define SLIDER_CANVAS_

#include "Canvas.hpp"
#include <Arduino.h>

#define SLIDER_MARGIN_LEFT 12
#define SLIDER_MARGIN_RIGHT 12
#define SLIDER_RADIUS 5
#define SLIDER_HEIGHT 3
#define SLIDER_TOUCHED_RADIUS 8
#define SLIDER_KNOB_COLOR 0x03a9f4
#define SLIDER_COLOR 0x6f6f6f

class SliderCanvas;

typedef std::function<void(SliderCanvas*, int16_t, int16_t)> SliderCanvasValueChangeCallback;

class SliderCanvas : public Canvas{
    public:
      SliderCanvas(Canvas * canvas, uint16_t id);
      void setMin(int16_t min);
      void setMax(int16_t max);
      void setStep(int16_t step);
      void setValue(int16_t value);
      void setName(String name);
      void setIcon(String icon);
      int16_t getMin();
      int16_t getMax();
      int16_t getStep();
      int16_t getValue();
      String getName();
      String getIcon();
      bool getTouched();
      void setDisabled(bool disabled);
      bool getDisabled();
      void invalidateValue();
      bool isValueInvalid();
      void onValueChange(SliderCanvasValueChangeCallback callback);
    protected:
      bool disabled;
      int16_t min;
      int16_t max;
      int16_t step;
      int16_t value;
      bool invalidValue;
      String name;
      String icon;
      bool touched = 0;
      int16_t prevValue = 0;
      int16_t prevDragValue = 0;
      uint16_t slideAnywhere = 0;
      uint16_t knob_color;
      uint16_t slider_color;
      virtual bool draw();
      SliderCanvasValueChangeCallback onValueChangeCallback;
      bool onTouchEventCallback (TouchEvent event, TouchEventData eventData);
};

#endif