#include "AlarmControlPanel.hpp"
using namespace HomeAssistant;

AlarmControlPanel::AlarmControlPanel (const char * entity) : Entity(entity, AlarmControlPanel::ENTITY_DOMAIN) {
    icon = ICON_BELL;
}

AlarmControlPanel::~AlarmControlPanel() {

}

void AlarmControlPanel::setIcon() {
    if (this->state == STATE_ALARM_ARMED_HOME) {
        this->icon = ICON_BELL_PLUS;
    } else if (this->state == STATE_ALARM_ARMED_NIGHT) {
        this->icon = ICON_BELL_SLEEP;
    } else if (this->state == STATE_ALARM_DISARMED) {
        this->icon = ICON_BELL_OUTLINE;
    } else if (this->state == STATE_ALARM_TRIGGERED) {
        this->icon = ICON_BELL_RING;
    } else {
        this->icon = ICON_BELL;
    }
}

void AlarmControlPanel::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_CODE_FORMAT, codeFormat);
    StateParser::parse(stateAttributesObject, ATTR_CODE_ARM_REQUIRED, codeArmRequired);
}

void AlarmControlPanel::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if ((strcmp(service, SERVICE_ALARM_ARM_HOME) == 0) || 
        (strcmp(service, SERVICE_ALARM_ARM_AWAY) == 0) ||
        (strcmp(service, SERVICE_ALARM_ARM_NIGHT) == 0) ||
        (strcmp(service, SERVICE_ALARM_ARM_NIGHT) == 0) ||
        (strcmp(service, SERVICE_ALARM_DISARM) == 0) ||
        (strcmp(service, SERVICE_ALARM_ARM_CUSTOM_BYPASS) == 0)) {
        if (codeSet)
            serviceData->add(ATTR_CODE, code);
        callEntityService(client, service);
    } else {
        callEntityService(client, service);
    }
}