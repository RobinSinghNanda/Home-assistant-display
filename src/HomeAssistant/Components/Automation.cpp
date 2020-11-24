#include "Automation.hpp"
using namespace HomeAssistant;

Automation::Automation (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_ROBOT;
}

Automation::~Automation() {

}