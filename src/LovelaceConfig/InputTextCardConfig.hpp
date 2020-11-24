#ifndef __INPUTTEXTCARDCONFIG_H__
#define __INPUTTEXTCARDCONFIG_H__

#include "BaseCardConfig.hpp"
#include <string.h>
#include <stdint.h>

class InputTextCardConfig : public BaseCardConfig {
  public:
    InputTextCardConfig(const char * entity, const char * title, const char * icon, bool state_color);
    InputTextCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon, bool state_color);
    InputTextCardConfig(const char * entity);

    static constexpr const char * TYPE = "input_text";

    inline const char * getEntityId();
    inline bool getStateColor();
    inline const char * getRowIcon();
  protected:
    string rowIcon;
    string entity;
    uint8_t state_color;
};

const char * InputTextCardConfig::getEntityId() {
    return entity.c_str();
}

bool InputTextCardConfig::getStateColor() {
    return state_color;
}

const char * InputTextCardConfig::getRowIcon() {
    return this->rowIcon.c_str();
}

#endif // __INPUTTEXTCARDCONFIG_H__