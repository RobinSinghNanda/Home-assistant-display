#include "SwitchCardConfig.hpp"

SwitchCardConfig::SwitchCardConfig(const char * entity, const char * title, const char * icon, bool state_color) :
    SwitchCardConfig(entity, title, icon, icon, state_color) {
}

SwitchCardConfig::SwitchCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color) :
    BaseEntityCardConfig(TYPE, entity, title, title, icon, rowIcon, state_color) {
}

SwitchCardConfig::SwitchCardConfig(const char * entity) :
    SwitchCardConfig(entity, "", "", 0) {
}