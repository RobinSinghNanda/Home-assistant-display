#ifndef __FANCARDCONFIG_H__
#define __FANCARDCONFIG_H__

#include "Cards.hpp"
#include <string.h>

class FanCardConfig : public BaseCardConfig {
  public:
    FanCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    FanCardConfig(const char * entity);
    char * getEntityId();
    bool getStateColor();
  protected:
    char entity[40];
    uint8_t state_color;
};
#endif // __FANCARDCONFIG_H__