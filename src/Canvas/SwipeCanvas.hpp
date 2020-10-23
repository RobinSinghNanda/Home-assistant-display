#ifndef SWIPE_CANVAS_H_
#define SWIPE_CANVAS_H_

#include "Canvas.hpp"

class SwipeCanvas;

enum SwipeDirection{
  SwipeInvalid = 0x00,
  SwipeUp = 0x10,
  SwipeDown = 0x20,
  SwipeLeft = 0x01,
  SwipeRight = 0x02,
  SwipeUpLeft = 0x11,
  SwipeUpRight = 0x12,
  SwipeDownLeft = 0x21,
  SwipeDownRight = 0x22
};

struct SwipeEventData {
  uint16_t startX;
  uint16_t startY;
  uint16_t endX;
  uint16_t endY;
  SwipeDirection swipeDirection;
};

#define getHorizontalSwipe(swipe) ((SwipeDirection)(swipe&0x0F))
#define getVerticalSwipe(swipe) ((SwipeDirection)(swipe&0xF0))

typedef std::function<bool(SwipeCanvas*, SwipeEventData eventData)> SwipeCanvasSwipeCallback;

class SwipeCanvas : public Canvas {
    public:
      SwipeCanvas(CanvasRef canvas, uint16_t id);
      void setSwipeThreshold(uint16_t swipeThreshold);
      uint16_t getSwipeThreshold();
      void onSwipeComplete(SwipeCanvasSwipeCallback callback);
      void onSwipe(SwipeCanvasSwipeCallback callback);
    protected:
      uint16_t swipeThreshold;
      SwipeDirection swipeValue;
      SwipeCanvasSwipeCallback onSwipeCallback;
      SwipeCanvasSwipeCallback onSwipeCompleteCallback;
      bool onTouchEventCallback (TouchEvent event, TouchEventData eventData);
};

#endif