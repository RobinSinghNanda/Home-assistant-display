#include "InputDateTime.hpp"
using namespace HomeAssistant;

InputDateTime::InputDateTime(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_CALENDAR_CLOCK;
}

InputDateTime::~InputDateTime() {

}

void InputDateTime::setIcon() {
    if (!hasDate) {
        this->icon = ICON_CLOCK;
    } else if (!hasTime) {
        this->icon = ICON_CALENDAR;
    } else {
        this->icon = ICON_CALENDAR_CLOCK;
    }
}

void InputDateTime::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_HAS_DATE, hasDate);
    StateParser::parse(stateAttributesObject, ATTR_HAS_TIME, hasTime);
    if (hasDate) {
        StateParser::parse(stateAttributesObject, ATTR_YEAR, year);
        StateParser::parse(stateAttributesObject, ATTR_MONTH, month);
        StateParser::parse(stateAttributesObject, ATTR_DAY, day);
    }
    if (hasTime) {
        StateParser::parse(stateAttributesObject, ATTR_HOUR, hour);
        StateParser::parse(stateAttributesObject, ATTR_MINUTE, minute);
        StateParser::parse(stateAttributesObject, ATTR_SECOND, second);
        StateParser::parse(stateAttributesObject, ATTR_TIMESTAMP, timestamp);
    }
}

void InputDateTime::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_SET_DATETIME) == 0) {
        if (hasDate) {
            char date[11];
            snprintf(date, sizeof(date), "%u-%u-%u", year, month, day);
            serviceData->add(ATTR_DATE, date);
        }
        if (hasTime) {
            char time[9];
            snprintf(time, sizeof(time), "%u:%u:%u", hour, minute, second);
            serviceData->add(ATTR_DATE, time);
        }
    }
    callEntityService(client, service);
}