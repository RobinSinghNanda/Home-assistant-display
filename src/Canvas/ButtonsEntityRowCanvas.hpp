#ifndef __BUTTONSENTITYROWCANVAS_H__
#define __BUTTONSENTITYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "ImageCanvas.hpp"
#define BUTTONS_PRIMARY_COLOR convert2rgb565(0x44739e)


class ButtonsEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        ButtonsEntityRowCanvas(Canvas * canvas, uint16_t id, uint16_t numButtons);
        ~ButtonsEntityRowCanvas();
        void setIconPath(String iconPath, uint16_t index);
        String getIconPath(uint16_t index);
        virtual void setWidth(uint16_t width);
        virtual void setFgColor(uint16_t fgColor);
        void onTouch(CanvasTouchCallback callback, uint16_t index);
    protected:
        ImageCanvas ** buttons;
        uint16_t numButtons;
};

#endif // __BUTTONSENTITYROWCANVAS_H__