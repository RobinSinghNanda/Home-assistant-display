#include "InputText.hpp"
using namespace HomeAssistant;

InputText::InputText(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_FORM_TEXTBOX;
}

InputText::~InputText() {

}

void InputText::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_MIN, min);
    StateParser::parse(stateAttributesObject, ATTR_MAX, max);
    StateParser::parse(stateAttributesObject, ATTR_MODE, mode);
    if (state != STATE_UNAVAILABLE) {
        value = state;
    }
}

void InputText::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_VALUE) == 0) {
        serviceData->add(ATTR_VALUE, value);
    }
    callEntityService(client, service);
}