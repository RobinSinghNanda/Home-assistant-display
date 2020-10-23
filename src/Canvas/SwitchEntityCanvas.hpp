#ifndef __BASEENTITYCANVAS_H__
#define __BASEENTITYCANVAS_H__

#include "Canvas.hpp"
#include "SwitchCanvas.hpp"
#include "SwitchEntityRowCanvas.hpp"

class SwitchEntityCanvas : public Canvas {
    public:
        SwitchEntityCanvas(Canvas* canvas, uint16_t id);
        virtual void setIconPath(String setIconPath);
        String getIconPath();
        void setName(String name);
        String getName();
        virtual void setState(bool state);
        bool getState();
        virtual void setDisabled(bool disabled);
        bool getDisabled();
        void setStateColor(bool stateColor);
        bool getStateColor();
        void onStateChange(SwitchCanvasStateChangeCallback callback);
        void onIconTouch(CanvasTouchCallback callback);
        void onNameTouch(CanvasTouchCallback callback);
    protected:
        SwitchEntityRowCanvas * rowCanvas;
};

#endif // __BASEENTITYCANVAS_H__