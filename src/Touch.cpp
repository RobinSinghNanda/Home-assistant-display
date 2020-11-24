#include <Arduino.h>
#include "TouchScreen.h"
#include "Touch.h"
#define YP 32 // must be an analog pin, use "An" notation!
#define XM 33 // must be an analog pin, use "An" notation!
#define YM 26 // can be a digital pin
#define XP 25 // can be a digital pin

bool pressed = 0;


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

CallbackFunction callback;

void touch_add_callback(CallbackFunction func);

void touch_begin() {
  callback =  [](TouchEvent, TouchEventData) -> bool {return false;};
}



uint32_t last_touch_time = 0;
uint32_t lastReleaseTime = 0;
uint32_t lastPressedTime = 0;
uint32_t lastTappedTime = 0;
uint32_t touch_period = 100;
uint32_t releasedCounter = 0;
uint32_t pressedCounter = 0;
uint32_t releaseThresold = 300;
uint32_t longPressThreshold = 500;
uint32_t doubleTapThreshold = 700;
uint16_t pressedPointX = 0;
uint16_t pressedPointY = 0;
uint16_t lastPointX = 0;
uint16_t lastPointY = 0;
uint16_t dragThreshold = 20;
TouchEventData eventData;
uint16_t x;
uint16_t y;
uint16_t longPressed = 0;
uint16_t dragged = 0;
uint16_t doubleTapped = 0;
uint8_t touch_poll () {
  TSPoint p = ts.getPoint();
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold) {
    
    y = map(p.x,170,960,239,0);
    x = map(p.y,190,990,0,319);
    if (y>239)
      y = 239;
    if (y<0)
      y = 0;
    if (x>319)
      x = 319;
    if (x<0)
      x = 0;
    uint32_t curr_time = millis();
    lastPointX = x;
    lastPointY = y;
    if (!pressed) {
      longPressed = 0;
      dragged = 0;
      pressed = 1;
      pressedPointX = x;
      pressedPointY = y;
      
      if (curr_time - lastPressedTime < doubleTapThreshold) {
        doubleTapped = 1;
        eventData.startX = pressedPointX;
        eventData.startY = pressedPointY;
        eventData.endX = pressedPointX;
        eventData.endY = pressedPointY;
        callback(TouchActionDoublePressed, eventData);
      } else {
        lastTappedTime = curr_time;
        eventData.startX = pressedPointX;
        eventData.startY = pressedPointY;
        eventData.endX = pressedPointX;
        eventData.endY = pressedPointY;
        callback(TouchActionPressed, eventData);
      }
    } else {
      int16_t x_diff = ((int16_t)x - (int16_t)pressedPointX);
      if (x_diff < 0)
        x_diff = -x_diff;
      int16_t y_diff = ((int16_t)y - (int16_t)pressedPointY);
      if (y_diff < 0)
        y_diff = -y_diff;
      eventData.endX = x;
      eventData.endY = y;
      eventData.startX = pressedPointX;
      eventData.startY = pressedPointY;
      if (((x_diff > dragThreshold) || (y_diff > dragThreshold))) {
        dragged = 1;
        lastTappedTime = curr_time;
        if (doubleTapped) {
          callback(TouchActionTapAndDrag, eventData);
        } else if (longPressed) {
          callback(TouchActionLongPressedAndDragged, eventData);
        } else {
          callback(TouchActionDraged, eventData);
        }
      } else if ((curr_time - lastTappedTime > longPressThreshold) && !longPressed && !dragged) {
        longPressed = 1;
        lastTappedTime = curr_time;
        if (doubleTapped) {
          callback(TouchActionTapAndLongPress, eventData);
        } else {
          callback(TouchActionLongPressed, eventData);
        }
      }
    }
    lastPressedTime = curr_time;
    return 1;
  } else {
    uint32_t curr_time = millis();
    if ((curr_time - lastPressedTime > releaseThresold)) {
      if (pressed) {
        pressed = 0;
        lastReleaseTime = curr_time;
        eventData.startX = pressedPointX;
        eventData.startY = pressedPointY;
        eventData.endX = lastPointX;
        eventData.endY = lastPointY;
        if (doubleTapped) {
          if (dragged) {
            callback(TouchActionTapAndDragReleased, eventData);
          } else if (longPressed) {
            callback(TouchActionTapAndLongPressReleased, eventData);
          } else {
            callback(TouchActionDoubleTapped, eventData);
          }
        } else {
          if (dragged) {
            if (longPressed) {
              callback(TouchActionLongPressedAndDraggedReleased, eventData);
            } else {
              callback(TouchActionDragReleased, eventData);
            }
          } else if (longPressed) {
            callback(TouchActionLongPressReleased, eventData);
          } else {
            callback(TouchActionTapped, eventData);
          }
        }
        pressed = 0;
        longPressed = 0;
        dragged = 0;
        longPressed = 0;
        doubleTapped =0;
      }
    }
    return 0;
  }
}

void touch_add_callback(CallbackFunction func) {
  callback =  func;
}
