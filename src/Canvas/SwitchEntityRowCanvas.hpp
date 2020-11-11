#ifndef __SWITCHENTTIYROWCANVAS_H__
#define __SWITCHENTTIYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"

class SwitchEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        SwitchEntityRowCanvas(Canvas * canvas, uint16_t id);
        void onStateChange(SwitchCanvasStateChangeCallback callback);
        virtual void setStateColor(bool stateColor);
        bool getStateColor();
        void setState(bool state);
        bool getState();
        void setDisabled(bool disabled);
        bool getDisabled();
        void setSecondaryColor(uint16_t color);
        uint16_t getSecondaryColor();
        void setSurfaceColor(uint16_t color);
        uint16_t getSurfaceColor();
        void setOnSurfaceColor(uint16_t color);
        uint16_t getOnSurfaceColor();
    protected:
        bool stateColor = true;
        SwitchCanvas * stateCanvas;
        SwitchCanvasStateChangeCallback onStateChangeCallback;
        bool stateChangeCallback (bool state);
        void setIconColor();
};

#endif // __SWITCHENTTIYROWCANVAS_H__