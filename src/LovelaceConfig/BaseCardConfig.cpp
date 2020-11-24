#include "BaseCardConfig.hpp"

#include "Arduino.h"

BaseCardConfig::BaseCardConfig(const char * type) : BaseCardConfig(type,"","") {
}

BaseCardConfig::BaseCardConfig(const char * type, const char * title, const char * icon) {
    this->type = type;
    this->title = title;
    this->icon = icon;
}