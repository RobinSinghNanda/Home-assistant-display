#ifndef __TOGGLECARDCANVAS_H__
#define __TOGGLECARDCANVAS_H__

#include "CardCanvas.hpp"
#include "LovelaceConfig/SwitchCardConfig.hpp"
#include "RowCanvas/ToggleEntityRowCanvas.hpp"

class ToggleCardCanvas : public CardCanvas {
    public:
        ToggleCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, BaseEntityCardConfig * cardConfig, LcdTheme * theme);
        ~ToggleCardCanvas();
        virtual const char * getCardTitle();
        virtual const char * getCardIcon();
        virtual void update();
    protected:
        DefaultRowConfig * rowConfig;
        ToggleEntityRowCanvas * entityCanvas;
        BaseEntityCardConfig * cardConfig;
        Entity * entity;
};


#endif // __TOGGLECARDCANVAS_H__