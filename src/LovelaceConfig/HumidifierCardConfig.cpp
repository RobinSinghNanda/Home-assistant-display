#include "HumidifierCardConfig.hpp"

HumidifierCardConfig::HumidifierCardConfig(const char * entity) : HumidifierCardConfig(entity, "", "", "") {

}

HumidifierCardConfig::HumidifierCardConfig(const char * entity, const char * name, const char * title, const char * icon) : BaseEntityCardConfig(TYPE, entity, name, title, icon, icon, 0) {

}