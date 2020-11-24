#include "Fan.hpp"
using namespace HomeAssistant;

Fan::Fan(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_FAN;
}

Fan::~Fan() {

}

void Fan::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_SPEED, speed);
    StateParser::parse(stateAttributesObject, ATTR_SPEED_LIST, speedList);
    StateParser::parse(stateAttributesObject, ATTR_OSCILLATING, osscillate);
    StateParser::parse(stateAttributesObject, ATTR_DIRECTION, direction);
}

void Fan::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_TURN_ON) == 0) {
        if (isFeatureSupported(SUPPORT_SET_SPEED)) {
            serviceData->add(ATTR_SPEED, speed);
        }
        if (isFeatureSupported(SUPPORT_OSCILLATE)) {
            serviceData->add(ATTR_OSCILLATING, osscillate);
        }
        if (isFeatureSupported(SUPPORT_DIRECTION)) {
            serviceData->add(ATTR_DIRECTION, direction);
        }
    }
    callEntityService(client, service);
}