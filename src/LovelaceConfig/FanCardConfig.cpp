#include "FanCardConfig.hpp"

FanCardConfig::FanCardConfig(const char * entity, const char * title, const char * icon, bool state_color) : BaseCardConfig(PAGE_TYPE_FAN) {
    strlcpy(this->entity, entity, sizeof(this->entity));
    strlcpy(this->title, title, sizeof(this->title));
    strlcpy(this->icon, icon, sizeof(this->icon));
    this->state_color = state_color;
}

FanCardConfig::FanCardConfig(const char * entity) {
    FanCardConfig(entity, "", "", 0);
}

char * FanCardConfig::getEntityId() {
    return entity;
}

bool FanCardConfig::getStateColor() {
    return state_color;
}