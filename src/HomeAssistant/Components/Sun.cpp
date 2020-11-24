#include "Sun.hpp"
using namespace HomeAssistant;

Sun::Sun (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_WHITE_BALANCE_SUNNY;
}

Sun::~Sun() {

}