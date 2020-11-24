#ifndef __BASEROWCANVAS_H__
#define __BASEROWCANVAS_H__

#include "Canvas/Canvas.hpp"

#include "LovelaceConfig/BaseRowConfig.hpp"
#include "HomeAssistant/HomeAssistantManager.hpp"
#include "LcdTheme.hpp"
using namespace HomeAssistant;

class BaseRowCanvas : public Canvas {
    public:
        BaseRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, BaseRowConfig * rowConfig, LcdTheme * theme);
        static constexpr uint16_t HEIGHT = 40;
        virtual void update();
    protected:
        LcdTheme * theme;
        HomeAssistantManager * hass;
        BaseRowConfig * rowConfig;
};

#endif // __BASEROWCANVAS_H__