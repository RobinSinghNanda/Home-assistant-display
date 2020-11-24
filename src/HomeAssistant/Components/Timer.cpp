#include "Timer.hpp"
using namespace HomeAssistant;

Timer::Timer(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_TIMER_OUTLINE;
}

Timer::~Timer() {

}

void Timer::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_DURATION, duration);
    StateParser::parse(stateAttributesObject, ATTR_FINISHES_AT, finishesAt);
    StateParser::parse(stateAttributesObject, ATTR_REMAINING, remaining);
}

void Timer::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_START) == 0) {
        serviceData->add(ATTR_DURATION, duration);
    }
    callEntityService(client, service);
}