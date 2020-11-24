#ifndef __BASEENTITYCARDCONFIG_H__
#define __BASEENTITYCARDCONFIG_H__

#include "BaseCardConfig.hpp"

class BaseEntityCardConfig : public BaseCardConfig {
  public:
    BaseEntityCardConfig(const char * type, const char * entity, const char * title, const char * icon, uint8_t state_color);
    BaseEntityCardConfig(const char * type, const char * entity, const char * name, const char * title, const char * icon, const char * rowIcon, uint8_t state_color);
    BaseEntityCardConfig(const char * type, const char * entity);

    static constexpr const char * TYPE = "";

    inline const char * getEntityId();
    inline uint8_t getStateColor();
    inline const char * getRowIcon();
    inline const char * getName();
  protected:
    string rowIcon;
    string entityId;
    string name;
    uint8_t stateColor;
};

const char * BaseEntityCardConfig::getEntityId() {
    return this->entityId.c_str();
}

uint8_t BaseEntityCardConfig::getStateColor() {
    return this->stateColor;
}

const char * BaseEntityCardConfig::getRowIcon() {
    return this->rowIcon.c_str();
}

const char * BaseEntityCardConfig::getName() {
    return this->name.c_str();
}

#endif // __BASEENTITYCARDCONFIG_H__