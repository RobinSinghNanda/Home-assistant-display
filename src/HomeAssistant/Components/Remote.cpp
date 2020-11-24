#include "Remote.hpp"
using namespace HomeAssistant;

Remote::Remote(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_REMOTE;
}

Remote::~Remote() {

}