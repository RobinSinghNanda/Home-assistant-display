#include "Switch.hpp"
using namespace HomeAssistant;

Switch::Switch(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_FLASH;
}

Switch::~Switch() {

}

void Switch::updateAttributes(JsonObject stateAttributesObject) {

}