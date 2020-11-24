#include "DeviceTracker.hpp"
using namespace HomeAssistant;

DeviceTracker::DeviceTracker (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_ACCOUNT;
}

DeviceTracker::~DeviceTracker() {

}