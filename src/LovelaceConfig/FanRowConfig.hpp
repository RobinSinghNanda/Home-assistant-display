#ifndef __FANROWCONFIG_H__
#define __FANROWCONFIG_H__

#include "DefaultRowConfig.hpp"
#include "CardDefines.hpp"

class FanRowConfig : public DefaultRowConfig {
    public:
        FanRowConfig(const char * entity, const char * name, const char * icon, bool state_color, bool hide_if_unavailable);
        FanRowConfig(const char * entity);
};

#endif // __FANROWCONFIG_H__