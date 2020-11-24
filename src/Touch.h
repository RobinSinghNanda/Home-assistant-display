#ifndef TOUCH_H_
#define TOUCH_H_

#include <Arduino.h>

#define PRESSED 0
#define RELEASED 8
#define SINGLE_TAPPED 2
#define DOUBLE_TAPPED 3
#define DRAGED 4
#define LONG_PRESSED 5

enum TouchEvent {
  TouchActionPressed = ((1<<SINGLE_TAPPED)<<PRESSED),
  TouchActionDoublePressed = ((1<<DOUBLE_TAPPED)<<PRESSED)|((1<<SINGLE_TAPPED)<<PRESSED),
  TouchActionDraged = ((1<<DRAGED)<<PRESSED)|((1<<SINGLE_TAPPED)<<PRESSED),
  TouchActionLongPressed = ((1<<LONG_PRESSED)<<PRESSED)|((1<<SINGLE_TAPPED)<<PRESSED),
  TouchActionTapAndDrag = (((1<<DOUBLE_TAPPED)|(1<<DRAGED))<<PRESSED|((1<<SINGLE_TAPPED)<<PRESSED)),
  TouchActionLongPressedAndDragged = (((1<<LONG_PRESSED)|(1<<DRAGED))<<PRESSED|((1<<SINGLE_TAPPED)<<PRESSED)),
  TouchActionTapAndLongPress = (((1<<DOUBLE_TAPPED)|(1<<LONG_PRESSED))<<PRESSED|((1<<SINGLE_TAPPED)<<PRESSED)),
  TouchActionTapped = ((1<<SINGLE_TAPPED)<<RELEASED),
  TouchActionLongPressReleased = (((1<<LONG_PRESSED)<<RELEASED)|((1<<SINGLE_TAPPED)<<RELEASED)),
  TouchActionDragReleased = ((1<<DRAGED)<<RELEASED)|((1<<SINGLE_TAPPED)<<RELEASED),
  TouchActionLongPressedAndDraggedReleased = (((1<<LONG_PRESSED)|(1<<DRAGED))<<RELEASED|((1<<SINGLE_TAPPED)<<RELEASED)),
  TouchActionDoubleTapped = (((1<<DOUBLE_TAPPED)<<RELEASED)|((1<<SINGLE_TAPPED)<<RELEASED)),
  TouchActionTapAndLongPressReleased = (((1<<DOUBLE_TAPPED)|(1<<LONG_PRESSED))<<RELEASED|((1<<SINGLE_TAPPED)<<RELEASED)),
  TouchActionTapAndDragReleased = (((1<<DOUBLE_TAPPED)|(1<<DRAGED)<<RELEASED)|((1<<SINGLE_TAPPED)<<RELEASED)),
};


#define isEvent(event, event_type) (event == event_type)
#define isEventLosely(event, event_type) ((event&event_type) == event_type)

struct TouchEventData {
  uint16_t startX;
  uint16_t startY;
  uint16_t endX;
  uint16_t endY;
};

void touch_begin();
uint8_t touch_poll ();


#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
#define maximum(a,b)     (((a) > (b)) ? (a) : (b))

typedef bool (*CallbackFunction)(TouchEvent event, TouchEventData eventData);

#endif
