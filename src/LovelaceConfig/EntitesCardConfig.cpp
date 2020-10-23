#include "EntitesCardConfig.hpp"

EntitesCardConfig::EntitesCardConfig(): BaseCardConfig(PAGE_TYPE_ENTITES){
}

EntitesCardConfig::EntitesCardConfig(const char * title, const char * icon): BaseCardConfig(PAGE_TYPE_ENTITES, title, icon){
}

void EntitesCardConfig::addEntity(BaseRowConfig * rowConfig) {
    if (entities.size() < 5) {
        entities.insert(entities.end() , rowConfig);
    } else {
        Serial.println("Max 5 entites allowed per card");
    }
}
uint16_t EntitesCardConfig::getNumEntites() {
    return entities.size();
}

BaseRowConfig * EntitesCardConfig::getEntityRow(uint16_t index) {
    return entities[index];
}