#include "Calendar.hpp"
using namespace HomeAssistant;

Calendar::Calendar (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_CALENDAR;
}

Calendar::~Calendar() {

}