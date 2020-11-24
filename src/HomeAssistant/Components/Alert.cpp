#include "Alert.hpp"
using namespace HomeAssistant;

Alert::Alert (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_ALERT;
}

Alert::~Alert() {

}