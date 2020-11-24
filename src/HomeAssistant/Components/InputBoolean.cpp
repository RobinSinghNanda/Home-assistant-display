#include "InputBoolean.hpp"
using namespace HomeAssistant;

InputBoolean::InputBoolean(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_TOGGLE_SWITCH_OUTLINE;
}

InputBoolean::~InputBoolean() {

}