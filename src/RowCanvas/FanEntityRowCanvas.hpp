#ifndef __FANENTITYROWCANVAS_H__
#define __FANENTITYROWCANVAS_H__

#include "SwitchEntityRowCanvas.hpp"
#include "HomeAssistant/Components/Fan.hpp"

class FanEntityRowCanvas;

class FanEntityRowCanvas : public ToggleEntityRowCanvas {
    public:
        FanEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        virtual void update();
    protected:
        Fan * entity = nullptr;
        uint16_t speed;
        uint16_t tmpSpeed;
        uint16_t swipeThreshold = 20;
        bool onTouchEventCallback (TouchEvent event, TouchEventData eventData); 
        void setFanIconPath();
};

#endif // __FANENTITYROWCANVAS_H__