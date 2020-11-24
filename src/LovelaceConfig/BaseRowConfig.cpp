#include "BaseRowConfig.hpp"
#include <string.h>

BaseRowConfig::BaseRowConfig() : BaseRowConfig("") {
   
}

BaseRowConfig::BaseRowConfig(const char * type) {
    this->type = type;
}