#ifndef __BASEROWCONFIG_H__
#define __BASEROWCONFIG_H__

class BaseRowConfig {
  public:
    BaseRowConfig();
    BaseRowConfig(const char * type);
    char * getType();
  protected:
    char type[40];
};

#endif // __BASEROWCONFIG_H__