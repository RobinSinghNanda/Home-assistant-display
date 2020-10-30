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
#define SLIDER_COLOR 0x7f7f7f

#define MASK_RB       63519 // 0b1111100000011111
#define MASK_G         2016 // 0b0000011111100000
#define MASK_MUL_RB 4065216 // 0b1111100000011111000000
#define MASK_MUL_G   129024 // 0b0000011111100000000000
#define MAX_ALPHA        64 // 6bits+1 with rounding

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
      uint16_t drawKnob(uint16_t x, uint16_t y, uint16_t bgColor);
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
      uint16_t alphaBlend(uint16_t fg, uint16_t bg, uint8 alpha);
};

#endif