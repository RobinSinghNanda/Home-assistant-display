#include "Proximity.hpp"
using namespace HomeAssistant;

Proximity::Proximity (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_APPLE_SAFARI;
}

Proximity::~Proximity() {

}