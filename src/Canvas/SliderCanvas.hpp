#ifndef SLIDER_CANVAS_
#define SLIDER_CANVAS_

#include "Canvas.hpp"
#include <Arduino.h>

#define SLIDER_CANVAS_MIN_HEIGHT 40

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

typedef std::function<void(SliderCanvas*, int32_t, int32_t)> SliderCanvasValueChangeCallback;

class SliderCanvas : public Canvas{
    public:
      SliderCanvas(Canvas * canvas, uint16_t id);

      static constexpr uint16_t HEIGHT = 40;
      void setMin(int32_t min);
      void setMax(int32_t max);
      void setStep(int32_t step);
      void setValue(int32_t value);
      void setName(String name);
      void setIcon(String icon);
      int32_t getMin();
      int32_t getMax();
      int32_t getStep();
      int32_t getValue();
      String getName();
      String getIcon();
      bool getTouched();
      void setDisabled(bool disabled);
      bool getDisabled();
      void setViewOnly(bool viewOnly);
      bool isViewOnly();
      void invalidateValue();
      bool isValueInvalid();
      void setHighlightValue(bool highlight);
      bool isValueHightlighted();
      uint16_t drawKnob(uint16_t x, uint16_t y, uint16_t bgColor);
      void onValueChange(SliderCanvasValueChangeCallback callback);
    protected:
      bool beingDragged = false;
      bool disabled;
      bool viewOnly = false;
      bool highlightValue = false;
      int32_t min;
      int32_t max;
      int32_t step;
      int32_t value;
      bool invalidValue;
      String name;
      String icon;
      bool touched = 0;
      int32_t prevValue = 0;
      int32_t prevDragValue = 0;
      uint16_t slideAnywhere = 0;
      uint16_t knob_color;
      uint16_t slider_color;
      virtual bool draw();
      SliderCanvasValueChangeCallback onValueChangeCallback;
      bool onTouchEventCallback (TouchEvent event, TouchEventData eventData);
};

#endif