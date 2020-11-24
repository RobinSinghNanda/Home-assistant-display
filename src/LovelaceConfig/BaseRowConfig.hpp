#ifndef __BASEROWCONFIG_H__
#define __BASEROWCONFIG_H__

#include <string>
using namespace std;

class BaseRowConfig {
  public:
    BaseRowConfig();
    BaseRowConfig(const char * type);
    static constexpr const char * TYPE = "";
    inline const char * getType();
    inline bool isType(const char * type);
  protected:
    string type;
};

const char * BaseRowConfig::getType() {
    return type.c_str();
}

bool BaseRowConfig::isType(const char * type) {
  return this->type.compare(type) == 0;
}

#endif // __BASEROWCONFIG_H__