#include "DefaultRowConfig.hpp"

DefaultRowConfig::DefaultRowConfig(const char * entity, const char * name, const char * icon, bool state_color, bool hide_if_unavailable) : BaseRowConfig(ENTITES_ROW_TYPE_DEFAULT) {
    strlcpy(this->entity, entity, sizeof(this->entity));
    strlcpy(this->name, name, sizeof(this->name));
    strlcpy(this->icon, icon, sizeof(this->icon));
    this->state_color = state_color;
    this->hide_if_unavailable = this->hide_if_unavailable;
}

DefaultRowConfig::DefaultRowConfig(const char * entity) {
    DefaultRowConfig(entity, "", "", 0, 0);
}

const char * DefaultRowConfig::getEntityId() {
    return entity;
}

const char * DefaultRowConfig::getName() {
    return name;
}

const char * DefaultRowConfig::getIcon() {
    return icon;
}

bool DefaultRowConfig::getStateColor() {
    return state_color;
}

bool DefaultRowConfig::getHideIfUnavailable() {
    return hide_if_unavailable;
}