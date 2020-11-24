#ifndef __THERMOSTATCARDCONFIG_H__
#define __THERMOSTATCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class ThermostatCardConfig : public BaseEntityCardConfig {
    public:
        ThermostatCardConfig(const char * entity);
        ThermostatCardConfig(const char * entity, const char * name, const char * title,  const char * icon);
        ThermostatCardConfig(const char * entity, const char * name, const char * title,  const char * icon, const char * rowIcon);
        static constexpr const char * TYPE = "thermostat";
};

#endif // __THERMOSTATCARDCONFIG_H__