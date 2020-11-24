#ifndef __FANCARDCONFIG_H__
#define __FANCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"
#include <string.h>

class FanCardConfig : public BaseEntityCardConfig {
  public:
    FanCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color);
    FanCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    FanCardConfig(const char * entity);

    static constexpr const char * TYPE = "fan";
};

#endif // __FANCARDCONFIG_H__