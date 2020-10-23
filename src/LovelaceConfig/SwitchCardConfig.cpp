#include "SwitchCardConfig.hpp"

SwitchCardConfig::SwitchCardConfig(const char * entity, const char * title, const char * icon, bool state_color) : BaseCardConfig(PAGE_TYPE_SWITCH) {
    strlcpy(this->entity, entity, sizeof(this->entity));
    strlcpy(this->title, title, sizeof(this->title));
    strlcpy(this->icon, icon, sizeof(this->icon));
    this->state_color = state_color;
}

SwitchCardConfig::SwitchCardConfig(const char * entity) {
    SwitchCardConfig(entity, "", "", 0);
}

char * SwitchCardConfig::getEntityId() {
    return entity;
}

bool SwitchCardConfig::getStateColor() {
    return state_color;
}