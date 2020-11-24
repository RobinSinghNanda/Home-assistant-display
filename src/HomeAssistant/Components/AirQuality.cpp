#include "AirQuality.hpp"
using namespace HomeAssistant;

AirQuality::AirQuality (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_AIR_FILTER;
}

AirQuality::~AirQuality() {

}