#include "BaseCardConfig.hpp"

#include "Arduino.h"

BaseCardConfig::BaseCardConfig(const char * type) {
    BaseCardConfig(type,"","");
}

BaseCardConfig::BaseCardConfig() {
    BaseCardConfig("","","");
}

BaseCardConfig::BaseCardConfig(const char * type, const char * title, const char * icon) {
    strlcpy(this->type, type, sizeof(this->type));
    strlcpy(this->title, title, sizeof(this->title));
    strlcpy(this->icon, title, sizeof(this->icon));
}

char * BaseCardConfig::getTitle() {
    return title;
}

char * BaseCardConfig::getType() {
    return type;
}

char * BaseCardConfig::getIcon() {
    return icon;
}