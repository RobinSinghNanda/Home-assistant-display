#include "InputSelect.hpp"
using namespace HomeAssistant;

InputSelect::InputSelect(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_FORMAT_LIST_BULLETED;
}

InputSelect::~InputSelect() {

}

void InputSelect::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_OPTIONS, options);
    for (string option : options) {
        if (option == state) {
            this->option = state;
        }
    }
}

void InputSelect::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SELECT_OPTION) == 0) {
        serviceData->add(ATTR_OPTION, option);
    }
    callEntityService(client, service);
}