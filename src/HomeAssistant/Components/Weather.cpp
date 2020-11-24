#include "Weather.hpp"
using namespace HomeAssistant;

Weather::Weather (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_WEATHER_CLOUDY;
}

void Weather::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_HUMIDITY, humidity);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_OZONE, ozone);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_PRESSURE, pressure);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_TEMPERATURE, temperature);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_VISIBILITY, visibility);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_WIND_BEARING, windBearing);
    StateParser::parse(stateAttributesObject, ATTR_WEATHER_WIND_SPEED, windSpeed);
    if (stateAttributesObject.containsKey(ATTR_FORECAST)) {
        JsonArray forecastArr = stateAttributesObject[ATTR_FORECAST];
        numForecasts = forecastArr.size();
        for (uint8_t index = forecasts.size();index<forecastArr.size();index++) {
            WeatherForecast * weatherForecast = new WeatherForecast();
            if (weatherForecast != nullptr) {
                forecasts.push_back(weatherForecast);
            }
        }
        for (uint8_t index = 0;index<numForecasts;index++) {
            forecasts[index]->updateAttributes(forecastArr[index]);
        }
    }
}

Weather::~Weather() {

}

void Weather::setIcon() {
    if (this->isState(STATE_CLEAR_NIGHT)) {
        this->icon = ICON_WEATHER_NIGHT;
    } else if (this->isState(STATE_CLOUDY)) {
        this->icon = ICON_WEATHER_CLOUDY;
    } else if (this->isState(STATE_EXCEPTIONAL)) {
        this->icon = ICON_ALERT_CIRCLE_OUTLINE;
    } else if (this->isState(STATE_FOG)) {
        this->icon = ICON_WEATHER_FOG;
    } else if (this->isState(STATE_HAIL)) {
        this->icon = ICON_WEATHER_HAIL;
    } else if (this->isState(STATE_LIGHTNING)) {
        this->icon = ICON_WEATHER_LIGHTNING;
    } else if (this->isState(STATE_LIGHTNING_RAINY)) {
        this->icon = ICON_WEATHER_LIGHTNING_RAINY;
    } else if (this->isState(STATE_PARTLYCLOUDY)) {
        this->icon = ICON_WEATHER_PARTLY_CLOUDY;
    } else if (this->isState(STATE_POURING)) {
        this->icon = ICON_WEATHER_POURING;
    } else if (this->isState(STATE_SNOWY)) {
        this->icon = ICON_WEATHER_SNOWY;
    } else if (this->isState(STATE_SNOWY_RAINY)) {
        this->icon = ICON_WEATHER_SNOWY_RAINY;
    } else if (this->isState(STATE_SUNNY)) {
        this->icon = ICON_WEATHER_SUNNY;
    } else if (this->isState(STATE_WINDY)) {
        this->icon = ICON_WEATHER_WINDY;
    } else if (this->isState(STATE_WINDY_VARIANT)) {
        this->icon = ICON_WEATHER_WINDY_VARIANT;
    } else {
        this->icon = ICON_WEATHER_CLOUDY;
    }
}



const char * Weather::getAttributeIcon(const char * attribute) {
    if (strcmp(attribute, ATTR_WEATHER_HUMIDITY) == 0) {
        return ICON_WATER_PERCENT;
    } else if (strcmp(attribute, ATTR_WEATHER_TEMPERATURE) == 0) {
        return ICON_THERMOMETER;
    } else if (strcmp(attribute, ATTR_WEATHER_WIND_BEARING) == 0) {
        return ICON_WEATHER_WINDY;
    } else if (strcmp(attribute, ATTR_WEATHER_WIND_SPEED) == 0) {
        return ICON_WEATHER_WINDY;
    } else if (strcmp(attribute, ATTR_WEATHER_PRESSURE) == 0) {
        return ICON_GAUGE;
    } else if (strcmp(attribute, ATTR_WEATHER_VISIBILITY) == 0) {
        return ICON_WEATHER_FOG;
    } else if (strcmp(attribute, ATTR_WEATHER_PRESSURE) == 0) {
        return ICON_GAUGE;
    } else {
        return "";
    }
}

WeatherForecast::WeatherForecast() {
    
}

void WeatherForecast::updateAttributes(JsonObject attributesObject) {
    StateParser::parse(attributesObject, ATTR_FORECAST_CONDITION, condition);
    StateParser::parse(attributesObject, ATTR_FORECAST_PRECIPITATION, precipitation);
    StateParser::parse(attributesObject, ATTR_FORECAST_PRECIPITATION_PROBABILITY, precipitationProbability);
    StateParser::parse(attributesObject, ATTR_FORECAST_TEMP, temperature);
    StateParser::parse(attributesObject, ATTR_FORECAST_TEMP_LOW, templow);
    StateParser::parse(attributesObject, ATTR_FORECAST_WIND_BEARING, windBearing);
    StateParser::parse(attributesObject, ATTR_FORECAST_WIND_SPEED, windSpeed);
    StateParser::parseTime(attributesObject, ATTR_FORECAST_TIME, datetime);
    this->setIcon();
}

void WeatherForecast::setIcon() {
    if (this->isCondition(CONDITION_CLEAR_NIGHT)) {
        this->icon = ICON_WEATHER_NIGHT;
    } else if (this->isCondition(CONDITION_CLOUDY)) {
        this->icon = ICON_WEATHER_CLOUDY;
    } else if (this->isCondition(CONDITION_EXCEPTIONAL)) {
        this->icon = ICON_ALERT_CIRCLE_OUTLINE;
    } else if (this->isCondition(CONDITION_FOG)) {
        this->icon = ICON_WEATHER_FOG;
    } else if (this->isCondition(CONDITION_HAIL)) {
        this->icon = ICON_WEATHER_HAIL;
    } else if (this->isCondition(CONDITION_LIGHTNING)) {
        this->icon = ICON_WEATHER_LIGHTNING;
    } else if (this->isCondition(CONDITION_LIGHTNING_RAINY)) {
        this->icon = ICON_WEATHER_LIGHTNING_RAINY;
    } else if (this->isCondition(CONDITION_PARTLYCLOUDY)) {
        this->icon = ICON_WEATHER_PARTLY_CLOUDY;
    } else if (this->isCondition(CONDITION_POURING)) {
        this->icon = ICON_WEATHER_POURING;
    } else if (this->isCondition(CONDITION_SNOWY)) {
        this->icon = ICON_WEATHER_SNOWY;
    } else if (this->isCondition(CONDITION_SNOWY_RAINY)) {
        this->icon = ICON_WEATHER_SNOWY_RAINY;
    } else if (this->isCondition(CONDITION_SUNNY)) {
        this->icon = ICON_WEATHER_SUNNY;
    } else if (this->isCondition(CONDITION_WINDY)) {
        this->icon = ICON_WEATHER_WINDY;
    } else if (this->isCondition(CONDITION_WINDY_VARIANT)) {
        this->icon = ICON_WEATHER_WINDY_VARIANT;
    } else {
        this->icon = ICON_WEATHER_CLOUDY;
    }
}