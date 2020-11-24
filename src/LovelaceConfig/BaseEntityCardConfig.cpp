#include "BaseEntityCardConfig.hpp"


BaseEntityCardConfig::BaseEntityCardConfig(const char * type, const char * entity, const char * title, const char * icon, uint8_t stateColor) :
    BaseEntityCardConfig(type, entity, title, title, icon, icon, stateColor) {

}

BaseEntityCardConfig::BaseEntityCardConfig(const char * type, const char * entity, const char * name, const char * title, const char * icon, const char * rowIcon, uint8_t stateColor) :
    BaseCardConfig(type, title, icon) {
    this->entityId = entity;
    this->rowIcon = rowIcon;
    this->name = name;
    this->stateColor = stateColor;
}

BaseEntityCardConfig::BaseEntityCardConfig(const char * type, const char * entity) :
    BaseEntityCardConfig(type, entity, "", "", "", "", 2) {

}