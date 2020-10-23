#include "BaseRowConfig.hpp"
#include <string.h>

BaseRowConfig::BaseRowConfig() {
    BaseRowConfig("");
}

BaseRowConfig::BaseRowConfig(const char * type) {
    strncpy(this->type, type, sizeof(this->type));
}

char * BaseRowConfig::getType() {
    return type;
}