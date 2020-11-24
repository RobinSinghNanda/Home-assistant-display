#include "EntityCardConfig.hpp"

EntityCardConfig::EntityCardConfig(const char * entity) : EntityCardConfig(entity, "", "", "", "", "") {

}

EntityCardConfig::EntityCardConfig(const char * entity, const char * name, const char * attribute, const char * unit, const char * title, const char * icon) :
    BaseEntityCardConfig(TYPE, entity, name, title, icon, icon, 2) {
    this->attribute = attribute;
    this->unit = unit;
}