#include "Notify.hpp"
using namespace HomeAssistant;

Notify::Notify (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_COMMENT_ALERT;
}

Notify::~Notify() {

}