#include "FanCardConfig.hpp"

FanCardConfig::FanCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color) :
    BaseEntityCardConfig(TYPE, entity, title, title, icon, rowIcon, state_color) {
}

FanCardConfig::FanCardConfig(const char * entity, const char * title, const char * icon, bool state_color) :
    FanCardConfig(entity, title, icon, icon, state_color) {
}

FanCardConfig::FanCardConfig(const char * entity) :
    FanCardConfig(entity, "", "", 0) {
}