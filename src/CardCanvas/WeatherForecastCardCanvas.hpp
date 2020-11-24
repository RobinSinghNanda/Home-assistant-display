#ifndef __WEATHERFORECASTCARDCANVAS_H__
#define __WEATHERFORECASTCARDCANVAS_H__

#include "CardCanvas.hpp"
#include "HomeAssistant/Components/Weather.hpp"
#include "LovelaceConfig/WeatherForecastCardConfig.hpp"
#include "RowCanvas/ToggleEntityRowCanvas.hpp"

class WeatherForecastSubCardCanvas : public Canvas {
    public:
        WeatherForecastSubCardCanvas(Canvas * canvas, uint16_t id, WeatherForecast * weatherForecast, LcdTheme * theme);
        void resetLayout();
        void update();
    protected:
        TextCanvas * dateTimeTextCanvas;
        ImageCanvas * conditionIconCanvas;
        TextCanvas * temperatureTextCanvas;
        TextCanvas * minTemperatureTextCanvas;
        WeatherForecast * forecast = nullptr;
};

class WeatherForecastCardCanvas : public CardCanvas {
    public:
        WeatherForecastCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, WeatherForecastCardConfig * cardConfig, LcdTheme * theme);
        static constexpr int MAX_FORECAST_CARD = 5;
        virtual ~WeatherForecastCardCanvas();
        virtual void update();
        void resetLayout();
    protected:
        uint8_t numForecasts = 0;
        uint8_t numForecastsObjects = 0;
        WeatherForecastCardConfig * cardConfig;
        Weather * entity;
        ImageCanvas * stateIconCanvas;
        TextCanvas * stateTextCanvas;
        TextCanvas * stateCanvas;
        TextCanvas * secondaryAttributeTextCanvas;
        IconCanvas * attributeIconCanvas;
        WeatherForecastSubCardCanvas * forecastCanvases[MAX_FORECAST_CARD];
};


#endif // __WEATHERFORECASTCARDCANVAS_H__