#ifndef __DEFAULTROWCONFIG_H__
#define __DEFAULTROWCONFIG_H__

#include "BaseRowConfig.hpp"
#include <string.h>
#include "CardTypes.hpp"

class DefaultRowConfig : public BaseRowConfig {
  public:
    DefaultRowConfig(const char * entity, const char * name, const char * icon, uint8_t state_color, bool hide_if_unavailable);
    DefaultRowConfig(const char * entity);
    static constexpr const char * TYPE = "";
    inline const char * getEntityId();
    inline const char * getName();
    inline const char * getIcon();
    inline uint8_t getStateColor();
    inline bool getHideIfUnavailable();
  protected:
    string entity;
    string name;
    string icon;
    uint8_t state_color;
    bool hide_if_unavailable;
};

const char * DefaultRowConfig::getEntityId() {
    return entity.c_str();
}

const char * DefaultRowConfig::getName() {
    return name.c_str();
}

const char * DefaultRowConfig::getIcon() {
    return icon.c_str();
}

uint8_t DefaultRowConfig::getStateColor() {
    return state_color;
}

bool DefaultRowConfig::getHideIfUnavailable() {
    return hide_if_unavailable;
}

#endif // __DEFAULTROWCONFIG_H__