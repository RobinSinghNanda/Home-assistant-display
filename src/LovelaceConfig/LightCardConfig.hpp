#ifndef __LIGHTCARDCONFIG_H__
#define __LIGHTCARDCONFIG_H__

#include "BaseCardConfig.hpp"
#include "CardDefines.hpp"
#include <string.h>
#include <stdint.h>

class LightCardConfig : public BaseCardConfig {
  public:
    LightCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    LightCardConfig(const char * entity);
    char * getEntityId();
    bool getStateColor();
  protected:
    char entity[40];
    uint8_t state_color;
};
#endif // __LIGHTCARDCONFIG_H__