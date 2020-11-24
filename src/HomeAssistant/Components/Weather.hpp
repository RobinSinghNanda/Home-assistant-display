#ifndef __WEATHER_H__
#define __WEATHER_H__

#include "Entity.hpp"
#include "RtcTime.hpp"

namespace HomeAssistant {
    class WeatherForecast {
        public:
            typedef const char * Condition;
            WeatherForecast();

            //condition
            static constexpr Condition CONDITION_CLEAR_NIGHT = "clear-night";
            static constexpr Condition CONDITION_CLOUDY = "cloudy";
            static constexpr Condition CONDITION_EXCEPTIONAL = "exceptional";
            static constexpr Condition CONDITION_FOG = "fog";
            static constexpr Condition CONDITION_HAIL = "hail";
            static constexpr Condition CONDITION_LIGHTNING = "lightning";
            static constexpr Condition CONDITION_LIGHTNING_RAINY = "lightning-rainy";
            static constexpr Condition CONDITION_PARTLYCLOUDY = "partlycloudy";
            static constexpr Condition CONDITION_POURING = "pouring";
            static constexpr Condition CONDITION_RAINY = "rainy";
            static constexpr Condition CONDITION_SNOWY = "snowy";
            static constexpr Condition CONDITION_SNOWY_RAINY = "snowy-rainy";
            static constexpr Condition CONDITION_SUNNY = "sunny";
            static constexpr Condition CONDITION_WINDY = "windy";
            static constexpr Condition CONDITION_WINDY_VARIANT = "windy-variant";

            //Attributes
            static constexpr AttributeName ATTR_FORECAST_CONDITION = "condition";
            static constexpr AttributeName ATTR_FORECAST_PRECIPITATION = "precipitation";
            static constexpr AttributeName ATTR_FORECAST_PRECIPITATION_PROBABILITY = "precipitation_probability";
            static constexpr AttributeName ATTR_FORECAST_TEMP = "temperature";
            static constexpr AttributeName ATTR_FORECAST_TEMP_LOW = "templow";
            static constexpr AttributeName ATTR_FORECAST_TIME = "datetime";
            static constexpr AttributeName ATTR_FORECAST_WIND_BEARING = "wind_bearing";
            static constexpr AttributeName ATTR_FORECAST_WIND_SPEED = "wind_speed";

            //API
            inline const char * getCondition();
            inline float getPrecipitation();
            inline float getPrecipitationProbability();
            inline float getTemperature();
            inline float getTemplow();
            inline uint32_t getDatetime();
            inline float getWindBearing();
            inline float getWindSpeed();
            inline const char * getIcon();
            inline bool isCondition(const char * condition);
            void updateAttributes(JsonObject attributesObject);
        protected:
            string icon = "";
            string condition = "";
            float precipitation = 0.0;
            float precipitationProbability = 0.0;
            float temperature = 0.0;
            float templow = 0.0;
            uint32_t datetime;
            float windBearing = 0.0;
            float windSpeed = 0.0;
            void setIcon();
    };
    class Weather : public Entity {
        public:
            Weather(const char * entity);
            virtual ~Weather();
            static constexpr const char * ENTITY_DOMAIN = "weather";

            //States
            static constexpr State STATE_CLEAR_NIGHT = "clear-night";
            static constexpr State STATE_CLOUDY = "cloudy";
            static constexpr State STATE_EXCEPTIONAL = "exceptional";
            static constexpr State STATE_FOG = "fog";
            static constexpr State STATE_HAIL = "hail";
            static constexpr State STATE_LIGHTNING = "lightning";
            static constexpr State STATE_LIGHTNING_RAINY = "lightning-rainy";
            static constexpr State STATE_PARTLYCLOUDY = "partlycloudy";
            static constexpr State STATE_POURING = "pouring";
            static constexpr State STATE_RAINY = "rainy";
            static constexpr State STATE_SNOWY = "snowy";
            static constexpr State STATE_SNOWY_RAINY = "snowy-rainy";
            static constexpr State STATE_SUNNY = "sunny";
            static constexpr State STATE_WINDY = "windy";
            static constexpr State STATE_WINDY_VARIANT = "windy-variant";
            
            static constexpr AttributeName ATTR_FORECAST = "forecast";
            static constexpr AttributeName ATTR_WEATHER_ATTRIBUTION = "attribution";
            static constexpr AttributeName ATTR_WEATHER_HUMIDITY = "humidity";
            static constexpr AttributeName ATTR_WEATHER_OZONE = "ozone";
            static constexpr AttributeName ATTR_WEATHER_PRESSURE = "pressure";
            static constexpr AttributeName ATTR_WEATHER_TEMPERATURE = "temperature";
            static constexpr AttributeName ATTR_WEATHER_VISIBILITY = "visibility";
            static constexpr AttributeName ATTR_WEATHER_WIND_BEARING = "wind_bearing";
            static constexpr AttributeName ATTR_WEATHER_WIND_SPEED = "wind_speed";

            // static Icon ICON_ATTR_HUMIDITY = ICON_WATER_PERCENT;
            // static Icon ICON_ATTR_WIND_BEARING = ICON_WEATHER_WINDY;
            // static Icon ICON_ATTR_WIND_SPEED = ICON_WEATHER_WINDY;
            // static Icon ICON_ATTR_PRESSURE = ICON_GAUGE;
            // static Icon ICON_ATTR_VISIBILITY = ICON_WEATHER_FOG;
            // static Icon ICON_ATTR_PRECIPITATION = ICON_WEATHER_RAINY;

            //API
            inline float getHumidity();
            inline float getOzone();
            inline float getPressure();
            inline float getTemperature();
            inline float getVisibility();
            inline float getWindBearing();
            inline float getWindSpeed();
            inline uint8_t getNumForecasts();
            const char * getAttributeIcon(const char * attribute);
            inline WeatherForecast * getForecast(uint8_t index);
            virtual void updateAttributes(JsonObject stateAttributesObject);
        protected:
            float humidity = 0;
            float ozone = 0;
            float pressure = 0;
            float temperature = 0;
            float visibility = 0;
            float windBearing = 0;
            float windSpeed = 0;
            uint8_t numForecasts = 0;
            std::vector<WeatherForecast*> forecasts;
            virtual void setIcon();
    };
    float Weather::getHumidity() {
        return humidity;
    }

    float Weather::getOzone() {
        return ozone;
    }

    float Weather::getPressure() {
        return pressure;
    }

    float Weather::getTemperature() {
        return temperature;
    }

    float Weather::getVisibility() {
        return visibility;
    }

    float Weather::getWindBearing() {
        return windBearing;
    }

    float Weather::getWindSpeed() {
        return windSpeed;
    }

    uint8_t Weather::getNumForecasts() {
        return numForecasts;
    }

    WeatherForecast * Weather::getForecast(uint8_t index) {
        if (index < numForecasts) {
            return forecasts[index];
        } else {
            return nullptr;
        }
    }

    const char * WeatherForecast::getCondition() {
        return condition.c_str();
    }

    float WeatherForecast::getPrecipitation() {
        return precipitation;
    }

    float WeatherForecast::getPrecipitationProbability() {
        return precipitationProbability;
    }

    float WeatherForecast::getTemperature() {
        return temperature;
    }

    float WeatherForecast::getTemplow() {
        return templow;
    }

    uint32_t WeatherForecast::getDatetime() {
        return datetime;
    }

    float WeatherForecast::getWindBearing() {
        return windBearing;
    }

    float WeatherForecast::getWindSpeed() {
        return windSpeed;
    }

    const char * WeatherForecast::getIcon() {
        return this->icon.c_str();
    }

    bool WeatherForecast::isCondition(const char * condition) {
        return this->condition.compare(condition) == 0;
    }

};

#endif // __WEATHER_H__