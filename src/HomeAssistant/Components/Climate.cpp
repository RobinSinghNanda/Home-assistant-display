#include "Climate.hpp"
using namespace HomeAssistant;

Climate::Climate(const char * entity) : Entity(entity, Climate::ENTITY_DOMAIN){
    icon = ICON_THERMOSTAT;
}

Climate::~Climate() {

}

void Climate::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_AUX_HEAT, auxHeat);
    StateParser::parse(stateAttributesObject, ATTR_FAN_MODES, fanModes);
    StateParser::parse(stateAttributesObject, ATTR_FAN_MODE, fanMode);
    StateParser::parse(stateAttributesObject, ATTR_PRESET_MODES, presetModes);
    StateParser::parse(stateAttributesObject, ATTR_PRESET_MODE, presetMode);
    StateParser::parse(stateAttributesObject, ATTR_SWING_MODES, swingModes);
    StateParser::parse(stateAttributesObject, ATTR_SWING_MODE, swingMode);
    StateParser::parse(stateAttributesObject, ATTR_CURRENT_TEMPERATURE, currentTemperature);
    StateParser::parse(stateAttributesObject, ATTR_TEMPERATURE, temperature);
    StateParser::parse(stateAttributesObject, ATTR_MAX_TEMP, maxTemperature);
    StateParser::parse(stateAttributesObject, ATTR_MIN_TEMP, minTemperature);
    StateParser::parse(stateAttributesObject, ATTR_CURRENT_HUMIDITY, currentHumidity);
    StateParser::parse(stateAttributesObject, ATTR_HUMIDITY, humidity);
    StateParser::parse(stateAttributesObject, ATTR_MAX_HUMIDITY, maxHumidity);
    StateParser::parse(stateAttributesObject, ATTR_MIN_HUMIDITY, minHumidity);
    StateParser::parse(stateAttributesObject, ATTR_HVAC_ACTION, hvacMode);
    StateParser::parse(stateAttributesObject, ATTR_HVAC_MODES, hvacModes);
}

void Climate::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_AUX_HEAT) == 0) {
        if (isFeatureSupported(SUPPORT_AUX_HEAT)) {
            serviceData->add(ATTR_AUX_HEAT, auxHeat);
        }
    } else if (strcmp(service, SERVICE_SET_PRESET_MODE) == 0) {
        if (isFeatureSupported(SUPPORT_PRESET_MODE)) {
            serviceData->add(ATTR_PRESET_MODE, presetMode);
        }
    } else if (strcmp(service, SERVICE_SET_TEMPERATURE) == 0) {
        if (isHvacMode(HVAC_MODE_HEAT_COOL)) {
            serviceData->add(ATTR_TARGET_TEMP_HIGH, targetHighTemperature);
            serviceData->add(ATTR_TARGET_TEMP_LOW, targetLowTemperature);
        } else {
            serviceData->add(ATTR_TEMPERATURE, temperature);
        }
        serviceData->add(ATTR_HVAC_MODE, hvacMode);
    } else if (strcmp(service, SERVICE_SET_HUMIDITY) == 0) {
        serviceData->add(ATTR_HUMIDITY, humidity);
    } else if (strcmp(service, SERVICE_SET_FAN_MODE) == 0) {
        serviceData->add(ATTR_FAN_MODE, fanMode);
    } else if (strcmp(service, SERVICE_SET_HVAC_MODE) == 0) {
        serviceData->add(ATTR_HVAC_MODE, hvacMode);
    } else if (strcmp(service, SERVICE_SET_SWING_MODE) == 0) {
        serviceData->add(ATTR_HVAC_MODE, swingMode);
    }
    callEntityService(client, service);
}