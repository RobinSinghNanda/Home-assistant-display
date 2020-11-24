#ifndef __CARDCANVAS_H__
#define __CARDCANVAS_H__

#include "LovelaceConfig/BaseCardConfig.hpp"
#include "Canvas/Canvas.hpp"
#include "HomeAssistant/HomeAssistantManager.hpp"
#include "LcdTheme.hpp"
using namespace HomeAssistant;

class CardCanvas : public Canvas {
    public:
        CardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, BaseCardConfig * cardConfig, LcdTheme * theme);
        virtual ~CardCanvas();
        virtual const char * getCardTitle();
        virtual const char * getCardIcon();
        virtual void update();
    protected:
        LcdTheme * theme;
        HomeAssistantManager * hass;
        BaseCardConfig * cardConfig;
};

#endif // __CARDCANVAS_H__