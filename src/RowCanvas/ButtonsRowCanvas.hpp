#ifndef __BUTTONSENTITYROWCANVAS_H__
#define __BUTTONSENTITYROWCANVAS_H__

#include "BaseRowCanvas.hpp"
#include "Canvas/ImageCanvas.hpp"
#include "LovelaceConfig/ButtonsRowConfig.hpp"
#define BUTTONS_PRIMARY_COLOR Color32Bit(0x44739e).get16Bit()


class ButtonsRowCanvas : public BaseRowCanvas {
    public:
        ButtonsRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, ButtonsRowConfig * rowConfig, LcdTheme * theme);
        ~ButtonsRowCanvas();
        void setIconPath(String iconPath, uint16_t index);
        String getIconPath(uint16_t index);
        virtual void setWidth(uint16_t width);
        virtual void setFgColor(Color16Bit fgColor);
        virtual void update();
    protected:
        ButtonsRowConfig * rowConfig;
        ImageCanvas ** buttons;
};

#endif // __BUTTONSENTITYROWCANVAS_H__