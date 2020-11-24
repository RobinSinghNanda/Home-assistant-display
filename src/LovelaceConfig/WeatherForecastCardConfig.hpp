#ifndef __WEATHERFORECASTCARDCONFIG_H__
#define __WEATHERFORECASTCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class WeatherForecastCardConfig : public BaseEntityCardConfig {
    public:
        WeatherForecastCardConfig(const char * entity);
        WeatherForecastCardConfig(const char * entity, const char * name, bool showForecast, const char * secondaryInfoAttribute, const char * title,  const char * icon);
        static constexpr const char * TYPE = "weather-forecast";
        inline bool getShowForecast();
        inline const char * getSecondaryInfoAttribute();
        inline bool isSecondaryAttribute(const char * attribute);
        inline bool isSecondaryAttributeDefined();
    protected:
        bool showForecast;
        string secondaryInfoAttribute;
};

bool WeatherForecastCardConfig::getShowForecast() {
    return showForecast;
}

const char * WeatherForecastCardConfig::getSecondaryInfoAttribute() {
    return secondaryInfoAttribute.c_str();
}

bool WeatherForecastCardConfig::isSecondaryAttribute(const char * attribute) {
    return this->secondaryInfoAttribute.compare(attribute) == 0;
}

bool WeatherForecastCardConfig::isSecondaryAttributeDefined() {
    return !(this->secondaryInfoAttribute == "");
}

#endif // __WEATHERFORECASTCARDCONFIG_H__