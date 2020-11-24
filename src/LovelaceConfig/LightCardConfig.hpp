#ifndef __LIGHTCARDCONFIG_H__
#define __LIGHTCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"
#include "CardTypes.hpp"
#include <string.h>
#include <stdint.h>

class LightCardConfig : public BaseEntityCardConfig {
  public:
    LightCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color);
    LightCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    LightCardConfig(const char * entity);

    static constexpr const char * TYPE = "light";
};

#endif // __LIGHTCARDCONFIG_H__