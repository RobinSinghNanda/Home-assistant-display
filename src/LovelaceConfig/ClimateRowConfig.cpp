#include "ClimateRowConfig.hpp"

ClimateRowConfig::ClimateRowConfig(const char * entity,
    const char * name,
    const char * icon,
    bool state_color,
    bool hide_if_unavailable) : DefaultRowConfig(entity, name, icon, state_color, hide_if_unavailable) {
    strlcpy(this->type, ENTITES_ROW_TYPE_CLIMATE, sizeof(this->type));
}

ClimateRowConfig::ClimateRowConfig(const char * entity) : DefaultRowConfig(entity) {
    strlcpy(this->type, ENTITES_ROW_TYPE_CLIMATE, sizeof(this->type));
}