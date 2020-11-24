#include "Script.hpp"
using namespace HomeAssistant;

Script::Script(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_SCRIPT_TEXT;
}

Script::~Script() {

}