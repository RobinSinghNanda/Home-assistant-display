#include "DefaultRowConfig.hpp"

DefaultRowConfig::DefaultRowConfig(const char * entity, const char * name, const char * icon, uint8_t state_color, bool hide_if_unavailable) :
    BaseRowConfig(DefaultRowConfig::TYPE) {
    this->entity = entity;
    this->name = name;
    this->icon = icon;
    this->state_color = state_color;
    this->hide_if_unavailable = this->hide_if_unavailable;
}

DefaultRowConfig::DefaultRowConfig(const char * entity) :
    DefaultRowConfig(entity, "", "", 0, 0) {
    
}