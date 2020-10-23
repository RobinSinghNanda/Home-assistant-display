#ifndef __SENSORENTITYROWCANVAS_H__
#define __SENSORENTITYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"

class SensorEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        SensorEntityRowCanvas(Canvas * canvas, uint16_t id);
        void setState(String state);
        String getState();
    protected:
        TextCanvas * stateCanvas;
};

#endif // __SENSORENTITYROWCANVAS_H__