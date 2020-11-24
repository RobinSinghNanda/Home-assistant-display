#include "Entity.hpp"
#include <stdlib.h>
using namespace HomeAssistant;

Entity::Entity(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
}

Entity::Entity(const char * entityId, const char * domain) {
    this->friendlyName = entityId;
    this->entityId = entityId;
    this->domain = domain;
    this->icon = ICON_BOOKMARK;
    this->state = STATE_UNAVAILABLE;
}

Entity::~Entity() {

}

void Entity::updateState(JsonObject stateObject) {
    if (stateObject.containsKey(STATE)) {
        setState(stateObject[STATE].as<string>().c_str());
    }
    if (stateObject.containsKey(ATTRIBUTES)) {
        this->updateAttributes(stateObject[ATTRIBUTES]);
    }
}

void Entity::updateAttributes(JsonObject stateAttributesObject) {
    StateParser::parse(stateAttributesObject, ATTR_SUPPORTED_FEATURES, supportedFeatures);
    StateParser::parse(stateAttributesObject, ATTR_FRIENDLY_NAME, friendlyName);
    StateParser::parse(stateAttributesObject, ATTR_FRIENDLY_NAME, friendlyName);
    StateParser::parse(stateAttributesObject, ATTR_ICON, icon);
    StateParser::parse(stateAttributesObject, ATTR_DEVICE_CLASS, deviceClass);
    this->setIcon();
}

void Entity::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    callEntityService(client, service);
}

void Entity::callEntityService(HomeAssistantClient * client, const char * service) {
    serviceData->add(ENTITY_ID, entityId.c_str());
    client->callService(domain.c_str(), service, *serviceData);
}

void Entity::setIcon() {
}

Entity::operator const char* () const {
    return this->entityId.c_str();
}