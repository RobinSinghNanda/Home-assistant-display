#include "MoreInfoCardConfig.hpp"

MoreInfoCardConfig::MoreInfoCardConfig(const char * entity) : MoreInfoCardConfig(entity, "", "", "") {

}

MoreInfoCardConfig::MoreInfoCardConfig(const char * entity, const char * name, const char * title, const char * icon) : MoreInfoCardConfig(entity, name, title, icon, icon) {
    
}

MoreInfoCardConfig::MoreInfoCardConfig(const char * entity, const char * name, const char * title,  const char * icon, const char * rowIcon) : 
    BaseEntityCardConfig(TYPE, entity, name, title, icon, rowIcon, 0) {
}