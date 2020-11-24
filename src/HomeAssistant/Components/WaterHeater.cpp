#include "WaterHeater.hpp"
using namespace HomeAssistant;

WaterHeater::WaterHeater(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_THERMOMETER;
}

WaterHeater::~WaterHeater() {

}

void WaterHeater::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_CURRENT_TEMPERATURE, currentTemperature);
    StateParser::parse(stateAttributesObject, ATTR_TEMPERATURE, temperature);
    StateParser::parse(stateAttributesObject, ATTR_MAX_TEMP, maxTemperature);
    StateParser::parse(stateAttributesObject, ATTR_MIN_TEMP, minTemperature);
    StateParser::parse(stateAttributesObject, ATTR_AWAY_MODE, awayMode);
    StateParser::parse(stateAttributesObject, ATTR_OPERATION_MODE, operationMode);
    StateParser::parse(stateAttributesObject, ATTR_OPERATION_LIST, operationModes);
}

void WaterHeater::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_TEMPERATURE) == 0) {
        serviceData->add(ATTR_TEMPERATURE, temperature);
    } else if (strcmp(service, SERVICE_SET_OPERATION_MODE) == 0) {
        serviceData->add(ATTR_OPERATION_MODE, operationMode);
    } else if (strcmp(service, SERVICE_SET_AWAY_MODE) == 0) {
        serviceData->add(ATTR_AWAY_MODE, awayMode);
    }
    callEntityService(client, service);
}