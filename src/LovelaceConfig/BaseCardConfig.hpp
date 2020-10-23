#ifndef __BASECARDCONFIG_H__
#define __BASECARDCONFIG_H__

#include <string.h>

class BaseCardConfig {
  public:
    BaseCardConfig(const char * type);
    BaseCardConfig();
    BaseCardConfig(const char * type, const char * title, const char * icon);
    char * getTitle();
    char * getType();
    char * getIcon();
  protected:
    char title[20];
    char type[20];
    char icon[40];
};

#endif // __BASECARDCONFIG_H__