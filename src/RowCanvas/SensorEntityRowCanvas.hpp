#ifndef __SENSORENTITYROWCANVAS_H__
#define __SENSORENTITYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"

class SensorEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        SensorEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        void setState(String state);
        String getState();
        virtual void setFgColor(Color16Bit fgColor);
        virtual void update();
    protected:
        TextCanvas * stateCanvas;
};

#endif // __SENSORENTITYROWCANVAS_H__