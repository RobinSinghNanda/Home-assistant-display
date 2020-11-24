#ifndef __BASECARDCONFIG_H__
#define __BASECARDCONFIG_H__

#include <string.h>
#include <Arduino.h>
#include <string>
using namespace std;

class BaseCardConfig {
  public:
    BaseCardConfig(const char * type);
    BaseCardConfig(const char * type, const char * title, const char * icon);
    static constexpr const char * TYPE = "";

    inline const char * getTitle();
    inline const char * getType();
    inline const char * getIcon();
    inline bool isType(const char * type);
  protected:
    string title;
    string type;
    string icon;
};

const char * BaseCardConfig::getTitle() {
    return title.c_str();
}

const char * BaseCardConfig::getType() {
    return type.c_str();
}

const char * BaseCardConfig::getIcon() {
    return icon.c_str();
}

bool BaseCardConfig::isType(const char * type) {
  return this->type.compare(type) == 0;
}


#endif // __BASECARDCONFIG_H__