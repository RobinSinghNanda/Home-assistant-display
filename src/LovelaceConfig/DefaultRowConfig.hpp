#ifndef __DEFAULTROWCONFIG_H__
#define __DEFAULTROWCONFIG_H__

#include "BaseRowConfig.hpp"
#include <string.h>
#include "CardDefines.hpp"

class DefaultRowConfig : public BaseRowConfig {
  public:
    DefaultRowConfig(const char * entity, const char * name, const char * icon, bool state_color, bool hide_if_unavailable);
    DefaultRowConfig(const char * entity);
    const char * getEntityId();
    const char * getName();
    const char * getIcon();
    bool getStateColor();
    bool getHideIfUnavailable();
  protected:
    char entity[40];
    char name[20];
    char icon[40];
    bool state_color;
    bool hide_if_unavailable;
};

#endif // __DEFAULTROWCONFIG_H__