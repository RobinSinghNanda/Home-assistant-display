#ifndef __HUMIDIFIERCARDCANVAS_H__
#define __HUMIDIFIERCARDCANVAS_H__

#include "CardCanvas.hpp"
#include "LovelaceConfig/DefaultRowConfig.hpp"
#include "LovelaceConfig/ButtonsRowConfig.hpp"
#include "LovelaceConfig/HumidifierCardConfig.hpp"
#include "RowCanvas/ToggleEntityRowCanvas.hpp"
#include "Canvas/SliderCanvas.hpp"
#include "Canvas/ImageCanvas.hpp"
#include "Canvas/TextSelectCanvas.hpp"

class HumidifierCardCanvas : public CardCanvas {
    public:
        HumidifierCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, HumidifierCardConfig * cardConfig, LcdTheme * theme);
        ~HumidifierCardCanvas();
        virtual void update();
        virtual void resetLayout();
    protected:
        HumidifierCardConfig * cardConfig;
        DefaultRowConfig * entityConfig = nullptr;
        Humidifier * entity;

        ToggleEntityRowCanvas * entityCanvas;

        IconCanvas * modeIconCanvas;
        TextCanvas * modeTextCanvas;
        TextSelectCanvas * modeSelectCanvas;
        
        IconCanvas * humidityIconCanvas;
        SliderCanvas * humiditySliderCanvas;
        TextCanvas * humidityTextCanvas;
};

#endif // __HUMIDIFIERCARDCANVAS_H__