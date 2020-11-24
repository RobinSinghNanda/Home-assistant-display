#include "ThermostatCardConfig.hpp"

ThermostatCardConfig::ThermostatCardConfig(const char * entity) : ThermostatCardConfig(entity, "", "", "") {

}

ThermostatCardConfig::ThermostatCardConfig(const char * entity, const char * name, const char * title, const char * icon) : ThermostatCardConfig(entity, name, title, icon, icon) {
    
}

ThermostatCardConfig::ThermostatCardConfig(const char * entity, const char * name, const char * title,  const char * icon, const char * rowIcon) : 
    BaseEntityCardConfig(TYPE, entity, name, title, icon, rowIcon, 0) {
}