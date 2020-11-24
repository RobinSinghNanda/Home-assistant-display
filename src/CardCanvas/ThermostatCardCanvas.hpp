#ifndef __THERMOSTATCARDCANVAS_H__
#define __THERMOSTATCARDCANVAS_H__

#include "CardCanvas.hpp"
#include "LovelaceConfig/DefaultRowConfig.hpp"
#include "LovelaceConfig/ButtonsRowConfig.hpp"
#include "LovelaceConfig/ThermostatCardConfig.hpp"
#include "RowCanvas/ClimateEntityRowCanvas.hpp"
#include "Canvas/SliderCanvas.hpp"
#include "Canvas/ImageCanvas.hpp"

class ThermostatCardCanvas : public CardCanvas {
    public:
        ThermostatCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, ThermostatCardConfig * cardConfig, LcdTheme * theme);
        ~ThermostatCardCanvas();
        static constexpr int MAX_HVAC_MODES = 6;
        virtual void update();
        virtual void resetLayout();
    protected:
        ThermostatCardConfig * cardConfig;
        Climate * entity;

        IconCanvas * iconCanvas;
        TextCanvas * nameCanvas;
        TextSelectCanvas * selectCanvas;
        
        IconCanvas * temperatureIconCanvas;
        SliderCanvas * temperatureSliderCanvas;
        TextCanvas * temperatureTextCanvas;

        IconCanvas * fanModeIconCanvas;
        TextCanvas * fanModeTextCanvas;
        TextSelectCanvas * fanModeSelectCanvas;

        IconCanvas * presetModeIconCanvas;
        TextCanvas * presetModeTextCanvas;
        TextSelectCanvas * presetModeSelectCanvas;

        IconCanvas * swingModeIconCanvas;
        TextCanvas * swingModeTextCanvas;
        TextSelectCanvas * swingModeSelectCanvas;
};

#endif // __THERMOSTATCARDCANVAS_H__