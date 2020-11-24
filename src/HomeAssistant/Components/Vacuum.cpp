#include "Vacuum.hpp"
using namespace HomeAssistant;

Vacuum::Vacuum(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_ROBOT_VACUUM;
}

Vacuum::~Vacuum() {

}

void Vacuum::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_FAN_SPEED, fanSpeed);
    StateParser::parse(stateAttributesObject, ATTR_FAN_SPEED_LIST, fanSpeeds);
}

void Vacuum::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_FAN_SPEED) == 0) {
        serviceData->add(ATTR_FAN_SPEED, fanSpeed);
    } else if (strcmp(service, SERVICE_SET_FAN_SPEED) == 0) {
        serviceData->add(ATTR_COMMAND, command);
        if (params != "") {
            serviceData->add(ATTR_PARAMS, params);
        }
    }
    callEntityService(client, service);
}