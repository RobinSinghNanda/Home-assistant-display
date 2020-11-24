#include "Light.hpp"
#include <string.h>
using namespace HomeAssistant;

Light::Light(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_LIGHTBULB;
}

Light::~Light() {

}

void Light::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_BRIGHTNESS, brightness);
    StateParser::parse(stateAttributesObject, ATTR_COLOR_TEMP, colorTemp);
    StateParser::parse(stateAttributesObject, ATTR_MIN_MIREDS, maxMireds);
    StateParser::parse(stateAttributesObject, ATTR_MAX_MIREDS, colorTemp);
    StateParser::parse(stateAttributesObject, ATTR_WHITE_VALUE, whiteValue);
    StateParser::parse(stateAttributesObject, ATTR_FLASH, flash);
    StateParser::parse(stateAttributesObject, ATTR_TRANSITION, transition);
    StateParser::parseColor(stateAttributesObject, ATTR_RGB_COLOR, color.data);
}

void Light::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_TURN_ON) == 0) {
        if (isFeatureSupported(SUPPORT_BRIGHTNESS) && brightness != 0) {
            serviceData->add(ATTR_BRIGHTNESS, brightness);
        }
        if (isFeatureSupported(SUPPORT_COLOR_TEMP) && colorTemp != 0) {
            serviceData->add(ATTR_COLOR_TEMP, colorTemp);
        }
        if (isFeatureSupported(SUPPORT_COLOR) && color.data != 0) {
            uint8_t color[3];
            color[0] = this->color.red;
            color[1] = this->color.green;
            color[2] = this->color.blue;
            serviceData->add(ATTR_RGB_COLOR, color, 3);
        }
        if (isFeatureSupported(SUPPORT_FLASH)) {
            serviceData->add(ATTR_FLASH, flash);
        }
        if (isFeatureSupported(SUPPORT_TRANSITION)) {
            serviceData->add(ATTR_TRANSITION, transition);
        }
        if (isFeatureSupported(SUPPORT_WHITE_VALUE)) {
            serviceData->add(ATTR_WHITE_VALUE, whiteValue);
        }
    }
    callEntityService(client, service);
}

void Light::callServiceTurnOn(HomeAssistantClient * client) {
    
}