#include "Humidifier.hpp"
using namespace HomeAssistant;

Humidifier::Humidifier (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_AIR_HUMIDIFIER;
}

Humidifier::~Humidifier() {

}

void Humidifier::setIcon() {
    if (isOff()) {
        this->icon = ICON_AIR_HUMIDIFIER_OFF;
    } else {
        this->icon = ICON_AIR_HUMIDIFIER;
    }
}

void Humidifier::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_MODE, mode);
    StateParser::parse(stateAttributesObject, ATTR_AVAILABLE_MODES, modes);
    StateParser::parse(stateAttributesObject, ATTR_HUMIDITY, humidity);
    StateParser::parse(stateAttributesObject, ATTR_MAX_HUMIDITY, maxHumidity);
    StateParser::parse(stateAttributesObject, ATTR_MIN_HUMIDITY, minHumidity);
}

void Humidifier::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_MODE) == 0) {
        serviceData->add(ATTR_MODE, mode);
    } else if (strcmp(service, SERVICE_SET_HUMIDITY) == 0) {
        serviceData->add(ATTR_HUMIDITY, humidity);
    }
    callEntityService(client, service);
}