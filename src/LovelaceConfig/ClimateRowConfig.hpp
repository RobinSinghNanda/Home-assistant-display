#ifndef __CLIMATEROWCONFIG_H__
#define __CLIMATEROWCONFIG_H__

#include "DefaultRowConfig.hpp"
#include "CardDefines.hpp"

class ClimateRowConfig : public DefaultRowConfig {
    public:
        ClimateRowConfig(const char * entity, const char * name, const char * icon, bool state_color, bool hide_if_unavailable);
        ClimateRowConfig(const char * entity);
};

#endif // __CLIMATEROWCONFIG_H__
