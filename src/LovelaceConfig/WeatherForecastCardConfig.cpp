#include "WeatherForecastCardConfig.hpp"

WeatherForecastCardConfig::WeatherForecastCardConfig(const char * entity) :
    WeatherForecastCardConfig(entity, "", false, "", "", ""){

}

WeatherForecastCardConfig::WeatherForecastCardConfig(const char * entity, const char * name, bool showForecast, const char * secondaryInfoAttribute, const char * title,  const char * icon) :
    BaseEntityCardConfig (TYPE, entity, name, title, icon, icon, 0) {
        this->secondaryInfoAttribute = secondaryInfoAttribute;
        this->showForecast = showForecast;
}