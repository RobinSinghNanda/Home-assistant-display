#ifndef __SWITCHCARDCONFIG_H__
#define __SWITCHCARDCONFIG_H__

#include "Cards.hpp"
#include <string.h>

class SwitchCardConfig : public BaseCardConfig {
  public:
    SwitchCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    SwitchCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color);
    SwitchCardConfig(const char * entity);
    char * getEntityId();
    bool getStateColor();
    const char * getRowIcon();
  protected:
    char rowIcon[40];
    char entity[40];
    uint8_t state_color;
};
#endif // __SWITCHCARDCONFIG_H__