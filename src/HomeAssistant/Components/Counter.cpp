#include "Counter.hpp"
using namespace HomeAssistant;

Counter::Counter (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_COUNTER;
}

Counter::~Counter() {

}

void Counter::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_STEP, step);
    StateParser::parse(stateAttributesObject, ATTR_MINIMUM, min);
    StateParser::parse(stateAttributesObject, ATTR_MAXIMUM, max);
    uint32_t tmpValue = 0;
    if (sscanf(state.c_str(), "%u", &tmpValue) == 1) {
        value = tmpValue;
    }
}