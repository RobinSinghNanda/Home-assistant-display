#ifndef __SWITCHCARDCONFIG_H__
#define __SWITCHCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"
#include <string.h>

class SwitchCardConfig : public BaseEntityCardConfig {
  public:
    SwitchCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    SwitchCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color);
    SwitchCardConfig(const char * entity);
    static constexpr const char * TYPE = "switch";
};

#endif // __SWITCHCARDCONFIG_H__