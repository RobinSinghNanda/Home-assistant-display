#include "EntitiesCardConfig.hpp"
#include "Log.hpp"

EntitiesCardConfig::EntitiesCardConfig(): BaseCardConfig(TYPE){
}

EntitiesCardConfig::EntitiesCardConfig(const char * title, const char * icon): BaseCardConfig(TYPE, title, icon){
}

void EntitiesCardConfig::addEntity(BaseRowConfig * rowConfig) {
    if (entities.size() < MAX_ENTITIES) {
        entities.insert(entities.end() , rowConfig);
    } else {
        Log::log(LOG_LEVEL_INFO, D_LOG_HA "Max %d entites allowed per card\n", MAX_ENTITIES);
    }
}
uint16_t EntitiesCardConfig::getNumEntites() {
    return entities.size();
}

BaseRowConfig * EntitiesCardConfig::getEntityRow(uint16_t index) {
    return entities[index];
}