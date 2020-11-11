#ifndef SWITCH_CANVAS_H_
#define SWITCH_CANVAS_H_

#include "ImageCanvas.hpp"


#define SWITCH_ICON_WIDTH 40
#define SWITCH_ICON_HEIGHT 20

#define SWITCH_ICON_UNAVAIABLE_DARK "/img/sw-un.jpg"
#define SWITCH_ICON_UNAVAIABLE_LIGHT "/img/sw-un-li.jpg"
#define SWITCH_ICON_ON_DARK "/img/sw-on.jpg"
#define SWITCH_ICON_ON_LIGHT "/img/sw-on-li.jpg"
#define SWITCH_ICON_OFF_DARK "/img/sw-off.jpg"
#define SWITCH_ICON_OFF_LIGHT "/img/sw-off-li.jpg"

#define SWITCH_ICON_UNAVAIABLE "/img/sw-off-un.bin"
#define SWITCH_ICON_ON "/img/sw-on.bin"
#define SWITCH_ICON_OFF "/img/sw-off.bin"

class SwitchCanvas;

typedef std::function<bool(SwitchCanvas*, bool)> SwitchCanvasStateChangeCallback;

class SwitchCanvas: public ImageCanvas {
    public:
      SwitchCanvas(Canvas * canvas, uint16_t id);
      SwitchCanvas(ImageCanvas * canvas, uint16_t id);
      SwitchCanvas(SwitchCanvas * canvas, uint16_t id);
      void setState(bool state);
      bool getState();
      void setDisabled(bool disabled);
      bool getDisabled();
      void onStateChange(SwitchCanvasStateChangeCallback callback);
      void resetIcon();
      void setSecondaryColor(uint16_t color);
      uint16_t getSecondaryColor();
      void setSurfaceColor(uint16_t color);
      uint16_t getSurfaceColor();
      void setOnSurfaceColor(uint16_t color);
      uint16_t getOnSurfaceColor();
      void setCustomState(bool state);
      bool isCustomState();
    private:
      bool state = false;
      bool disabled = true;
      bool customState = false;
      void setImagePath();
      uint16_t secondaryColor;
      uint16_t surfaceColor;
      uint16_t onSurfaceColor;
      SwitchCanvasStateChangeCallback onStateChangeCallback;
    protected:
      bool onTouchEventCallback (TouchEvent event, TouchEventData eventData); 
      virtual bool draw();
};


#endif