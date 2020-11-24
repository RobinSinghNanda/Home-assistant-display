#include "InputTextCardConfig.hpp"

InputTextCardConfig::InputTextCardConfig(const char * entity, const char * title, const char * icon, bool state_color) :
    InputTextCardConfig(entity, title, icon, icon, state_color) {
}

InputTextCardConfig::InputTextCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color)
    : BaseCardConfig(TYPE) {
    this->entity = entity;
    this->title = title;
    this->icon = icon;
    this->rowIcon = rowIcon;
    this->state_color = state_color;
}

InputTextCardConfig::InputTextCardConfig(const char * entity) :
    InputTextCardConfig(entity, "", "", 0) {
}