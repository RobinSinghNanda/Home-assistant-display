#include "Lock.hpp"
using namespace HomeAssistant;

Lock::Lock(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_LOCK;
}

Lock::~Lock() {

}

void Lock::setIcon() {
    this->icon = (this->state==STATE_UNLOCKED)?ICON_LOCK_OPEN:ICON_LOCK;
}