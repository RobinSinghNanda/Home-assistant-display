#include "WeatherForecastCardCanvas.hpp"
#include "Fonts.hpp"
#include "Settings.hpp"

WeatherForecastCardCanvas::WeatherForecastCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, WeatherForecastCardConfig * cardConfig, LcdTheme * theme) :
    CardCanvas(canvas, id, hass, cardConfig, theme) {
    this->cardConfig = cardConfig;
    this->entity = hass->getEntity<Weather>(cardConfig->getEntityId());
    Serial.printf("Entity is %s\n", cardConfig->getEntityId());
    if (entity) {
        stateIconCanvas = new ImageCanvas(this, 0);
        stateTextCanvas = new TextCanvas(this, 1);
        if (cardConfig->getSecondaryInfoAttribute()[0] != '\0') {
            secondaryAttributeTextCanvas = new TextCanvas(this, 2);
            attributeIconCanvas = new IconCanvas(this, 3);
        }
        stateCanvas = new TextCanvas(this, 4);

        stateIconCanvas->setWidth(50);
        stateIconCanvas->setHeight(80);
        stateIconCanvas->setMarginLeft(10);
        stateIconCanvas->setHAlign(ALIGN_CENTER);
        stateIconCanvas->setVAlign(ALIGN_MIDDLE);
        stateIconCanvas->setPath(ICON_WEATHER_CLOUDY);
        stateIconCanvas->setFgColor(theme->colorEntityIconUnavailable);

        stateCanvas->setWidth(80);
        stateCanvas->setHeight(80);
        stateCanvas->hAlignRight();
        stateCanvas->setMarginRight(10);
        stateCanvas->setHAlign(ALIGN_RIGHT);
        stateCanvas->setVAlign(ALIGN_MIDDLE);
        stateCanvas->setFont(FONT_REGULAR_24);

        stateTextCanvas->setWidth(this->getDrawableWidth() - stateCanvas->getWidth() - stateIconCanvas->getWidth());
        stateTextCanvas->setHeight(80);
        stateTextCanvas->setX(stateIconCanvas->getRightX());
        stateTextCanvas->setFont(FONT_REGULAR_24);
        stateTextCanvas->setVAlign(ALIGN_MIDDLE);
        stateTextCanvas->setMarginLeft(10);
        
        if (cardConfig->getSecondaryInfoAttribute()[0] != '\0') {
            secondaryAttributeTextCanvas->setWidth(stateCanvas->getWidth());
            secondaryAttributeTextCanvas->setHeight(40);
            secondaryAttributeTextCanvas->setHAlign(ALIGN_RIGHT);
            secondaryAttributeTextCanvas->setVAlign(ALIGN_MIDDLE);
            secondaryAttributeTextCanvas->hAlignRight();
            secondaryAttributeTextCanvas->setFont(FONT_REGULAR_24);
            secondaryAttributeTextCanvas->setFgColor(Color32Bit(0x7F7F7F));

            attributeIconCanvas->setWidth(30);
            attributeIconCanvas->setHeight(40);
            attributeIconCanvas->setHAlign(ALIGN_RIGHT);
            attributeIconCanvas->setVAlign(ALIGN_MIDDLE);
            attributeIconCanvas->hAlignLeft(secondaryAttributeTextCanvas);
            attributeIconCanvas->setFgColor(Color32Bit(0x7F7F7F));
        }
        for (uint8_t index = 0;index<MAX_FORECAST_CARD;index++) {
            forecastCanvases[index] = nullptr;
        }
    }
}

WeatherForecastCardCanvas::~WeatherForecastCardCanvas() {
}

void WeatherForecastCardCanvas::resetLayout() {
    uint16_t unitWidth = this->getDrawableWidth()/numForecasts;
    for (uint8_t i=0;i<numForecasts;i++) {
        WeatherForecastSubCardCanvas * canvas = forecastCanvases[i];
        canvas->setWidth(unitWidth);
        canvas->setX(unitWidth*i);
        canvas->resetLayout();
    }
}

void WeatherForecastCardCanvas::update() {
    if (entity) {
        char str[40];
        if (entity->isState(Weather::STATE_CLEAR_NIGHT)) {
            stateIconCanvas->setPath( "/w/clear-night.jpg");
        } else if (entity->isState(Weather::STATE_CLOUDY) ||
                    entity->isState(Weather::STATE_FOG)) {
            stateIconCanvas->setPath( "/w/cloudy.jpg");
        } else if (entity->isState(Weather::STATE_HAIL) ||
                    entity->isState(Weather::STATE_RAINY)) {
            stateIconCanvas->setPath( "/w/rainy.jpg");
        } else if (entity->isState(Weather::STATE_POURING)) {
            stateIconCanvas->setPath( "/w/pouring.jpg");
        } else if (entity->isState(Weather::STATE_SNOWY) ||
                    entity->isState(Weather::STATE_SNOWY_RAINY)) {
            stateIconCanvas->setPath( "/w/snowy.jpg");
        } else if (entity->isState(Weather::STATE_LIGHTNING) ||
                    entity->isState(Weather::STATE_LIGHTNING_RAINY)) {
            stateIconCanvas->setPath( "/w/lightning.jpg");
        } else if (entity->isState(Weather::STATE_WINDY) ||
                    entity->isState(Weather::STATE_WINDY_VARIANT)) {
            stateIconCanvas->setPath( "/w/windy.jpg");
        } else if (entity->isState(Weather::STATE_SUNNY)) {
            stateIconCanvas->setPath( "/w/sunny.jpg");
        } else if (entity->isState(Weather::STATE_PARTLYCLOUDY)) {
            stateIconCanvas->setPath( "/w/partlycloudy.jpg");
        } else {
            stateIconCanvas->setPath(entity->getIcon());
        }
        
        if (entity->isUnavailable()) {
            stateIconCanvas->setFgColor(theme->colorEntityIconUnavailable);
            stateTextCanvas->setText(entity->getFriendlyName());
            stateCanvas->setText("");
            for (uint8_t i=0;i<numForecastsObjects;i++) {
                forecastCanvases[i]->setVisible(false);
            }
        } else {
            for (uint8_t i=0;i<numForecastsObjects;i++) {
                forecastCanvases[i]->setVisible(true);
            }
            snprintf(str, sizeof(str), "%0.1f°C", entity->getTemperature());
            stateCanvas->setText(str);
            stateIconCanvas->setFgColor(theme->colorEntityIcon);
            snprintf(str, sizeof(str), "%s", entity->getState());
            if (str[0] != '\0') {
                str[0] = str[0] + 'A' - 'a';
            }
            for (uint8_t i=0;i<strlen(str);i++) {
                if (str[i] == '-') {
                    str[i] = ' ';
                }
            }
            stateTextCanvas->setText(str);
            if (cardConfig->getShowForecast()) {
                numForecasts = entity->getNumForecasts();
                if (numForecasts > MAX_FORECAST_CARD) {
                    numForecasts = MAX_FORECAST_CARD;
                }
                if (numForecastsObjects < numForecasts) {
                    for(uint8_t i = numForecastsObjects;i<numForecasts;i++) {
                        WeatherForecastSubCardCanvas * canvas = new WeatherForecastSubCardCanvas(this, i+2, entity->getForecast(i), theme);
                        canvas->setHeight(this->getDrawableHeight()-this->stateIconCanvas->getHeight());
                        canvas->setY(this->stateIconCanvas->getBottomY());

                        forecastCanvases[i] = canvas;
                    }
                    numForecastsObjects = numForecasts;
                    resetLayout();
                }
                for(uint8_t i = 0;i<numForecasts;i++) {
                    forecastCanvases[i]->update();
                }
            }
            if (cardConfig->getSecondaryInfoAttribute()[0] != '\0') {
                attributeIconCanvas->setPath(entity->getAttributeIcon(cardConfig->getSecondaryInfoAttribute()));
                if (cardConfig->isSecondaryAttribute(Weather::ATTR_WEATHER_HUMIDITY)) {
                    snprintf(str, sizeof(str), "%f", entity->getHumidity());
                    secondaryAttributeTextCanvas->setText(str);
                } else if (cardConfig->isSecondaryAttribute(Weather::ATTR_WEATHER_TEMPERATURE)) {
                    snprintf(str, sizeof(str), "%f", entity->getTemperature());
                    secondaryAttributeTextCanvas->setText(str);
                }
            }
        }
    }
}

WeatherForecastSubCardCanvas::WeatherForecastSubCardCanvas(Canvas * canvas, uint16_t id, WeatherForecast * forecast, LcdTheme * theme) : Canvas(canvas, id) {
    this->forecast = forecast;
    dateTimeTextCanvas  = new TextCanvas(this, 0);
    conditionIconCanvas = new ImageCanvas(this, 1);
    temperatureTextCanvas = new TextCanvas(this, 2);
    minTemperatureTextCanvas = new TextCanvas(this, 3);
    dateTimeTextCanvas->setHeight(28);
    dateTimeTextCanvas->setHAlign(ALIGN_CENTER);
    dateTimeTextCanvas->setVAlign(ALIGN_MIDDLE);
    dateTimeTextCanvas->setFont(FONT_LIGHT_24);
    conditionIconCanvas->setHeight(36);
    conditionIconCanvas->setHAlign(ALIGN_CENTER);
    conditionIconCanvas->setVAlign(ALIGN_MIDDLE);
    conditionIconCanvas->setY(dateTimeTextCanvas->getBottomY());
    conditionIconCanvas->setFgColor(theme->colorEntityIcon);
    temperatureTextCanvas->setHeight(28);
    temperatureTextCanvas->setHAlign(ALIGN_CENTER);
    temperatureTextCanvas->setVAlign(ALIGN_MIDDLE);
    temperatureTextCanvas->setY(conditionIconCanvas->getBottomY());
    temperatureTextCanvas->setFont(FONT_LIGHT_24);
    minTemperatureTextCanvas->setHeight(28);
    minTemperatureTextCanvas->setHAlign(ALIGN_CENTER);
    minTemperatureTextCanvas->setVAlign(ALIGN_MIDDLE);
    minTemperatureTextCanvas->setY(temperatureTextCanvas->getBottomY());
    minTemperatureTextCanvas->setFont(FONT_LIGHT_24);
    minTemperatureTextCanvas->setFgColor(Color32Bit(0x7F7F7F));
}

void WeatherForecastSubCardCanvas::resetLayout() {
    dateTimeTextCanvas->setX(this->getX());
    conditionIconCanvas->setX(this->getX());
    temperatureTextCanvas->setX(this->getX());
    minTemperatureTextCanvas->setX(this->getX());
    dateTimeTextCanvas->setWidth(this->getWidth());
    conditionIconCanvas->setWidth(this->getWidth());
    temperatureTextCanvas->setWidth(this->getWidth());
    minTemperatureTextCanvas->setWidth(this->getWidth());
    dateTimeTextCanvas->setY(this->getY());
    conditionIconCanvas->setY(dateTimeTextCanvas->getBottomY());
    temperatureTextCanvas->setY(conditionIconCanvas->getBottomY());
    minTemperatureTextCanvas->setY(temperatureTextCanvas->getBottomY());
}

void WeatherForecastSubCardCanvas::update() {
    if (forecast) {
        char str[40];
        memset(str, 0, sizeof(str));
        if (forecast->isCondition(WeatherForecast::CONDITION_CLEAR_NIGHT)) {
            conditionIconCanvas->setPath( "/w/clear-night.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_CLOUDY) ||
            forecast->isCondition(WeatherForecast::CONDITION_FOG)) {
            conditionIconCanvas->setPath( "/w/cloudy.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_HAIL) ||
                    forecast->isCondition(WeatherForecast::CONDITION_RAINY)) {
            conditionIconCanvas->setPath( "/w/rainy.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_POURING)) {
            conditionIconCanvas->setPath( "/w/pouring.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_SNOWY) ||
            forecast->isCondition(WeatherForecast::CONDITION_SNOWY_RAINY)) {
            conditionIconCanvas->setPath( "/w/snowy.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_LIGHTNING) ||
            forecast->isCondition(WeatherForecast::CONDITION_LIGHTNING_RAINY)) {
            conditionIconCanvas->setPath( "/w/lightning.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_WINDY) ||
            forecast->isCondition(WeatherForecast::CONDITION_WINDY_VARIANT)) {
            conditionIconCanvas->setPath( "/w/windy.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_SUNNY)) {
            conditionIconCanvas->setPath( "/w/sunny.jpg");
        } else if (forecast->isCondition(WeatherForecast::CONDITION_PARTLYCLOUDY)) {
            conditionIconCanvas->setPath( "/w/partlycloudy.jpg");
        } else {
            conditionIconCanvas->setPath(forecast->getIcon());
        }
        snprintf(str, sizeof(str)-1, "%0.1f°", forecast->getTemperature());
        temperatureTextCanvas->setText(str);
        snprintf(str, sizeof(str)-1, "%0.1f°", forecast->getTemplow());
        minTemperatureTextCanvas->setText(str);
        char day[4];
        day[3] = '\0';
        TIME_T ut;
        BreakTime(forecast->getDatetime() + settings.getTimezone()*60*60 + settings.getTimezoneMinutes() * 60, ut);
        uint8_t index=(ut.day_of_week-1)*3;
        memcpy(day, (void *)(kDayNamesEngligh+index), 3);
        dateTimeTextCanvas->setText(day);
    }
}