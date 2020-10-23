#include "LightCardConfig.hpp"

LightCardConfig::LightCardConfig(const char * entity, const char * title, const char * icon, bool state_color) : BaseCardConfig(PAGE_TYPE_LIGHT) {
    strlcpy(this->entity, entity, sizeof(this->entity));
    strlcpy(this->title, title, sizeof(this->title));
    strlcpy(this->icon, icon, sizeof(this->icon));
    this->state_color = state_color;
}

LightCardConfig::LightCardConfig(const char * entity) {
    LightCardConfig(entity, "", "", 0);
}

char * LightCardConfig::getEntityId() {
    return entity;
}

bool LightCardConfig::getStateColor() {
    return state_color;
}