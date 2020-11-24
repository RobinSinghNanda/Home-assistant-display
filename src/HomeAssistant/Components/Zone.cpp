#include "Zone.hpp"
using namespace HomeAssistant;

Zone::Zone (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_MAP_MARKER_RADIUS;
}

Zone::~Zone() {

}