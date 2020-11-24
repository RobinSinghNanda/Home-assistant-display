#include "LightCardConfig.hpp"

LightCardConfig::LightCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color) :
    BaseEntityCardConfig(TYPE, entity, title, title, icon, rowIcon, state_color) {
}

LightCardConfig::LightCardConfig(const char * entity, const char * title, const char * icon, bool state_color) :
    LightCardConfig(entity, title, icon, icon, state_color) {
}

LightCardConfig::LightCardConfig(const char * entity) : LightCardConfig(entity, "", "", 0) {
}