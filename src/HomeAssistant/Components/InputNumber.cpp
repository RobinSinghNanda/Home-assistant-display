#include "InputNumber.hpp"
using namespace HomeAssistant;

InputNumber::InputNumber(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_RAY_VERTEX;
    
}

InputNumber::~InputNumber() {

}

void InputNumber::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_MAX, max);
    StateParser::parse(stateAttributesObject, ATTR_MIN, min);
    StateParser::parse(stateAttributesObject, ATTR_STEP, step);
    float tmpValue = 0.0;
    if (sscanf(state.c_str(), "%f", &tmpValue) == 1) {
        value = tmpValue;
    }
}

void InputNumber::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_VALUE) == 0) {
        serviceData->add(ATTR_VALUE, value);
    }
    callEntityService(client, service);
}