#ifndef __ENTITESCARDCONFIG_H__
#define __ENTITESCARDCONFIG_H__

#include "BaseRowConfig.hpp"
#include "BaseCardConfig.hpp"
#include <stdint.h>
#include <string.h>
#include "Cards.hpp"
#include <vector>

class EntitiesCardConfig : public BaseCardConfig {
  public:
    EntitiesCardConfig();
    EntitiesCardConfig(const char * title, const char * icon);
    static constexpr const char * TYPE = "entities";
    static constexpr int MAX_ENTITIES = 5;

    void addEntity(BaseRowConfig * rowConfig);
    uint16_t getNumEntites();
    BaseRowConfig * getEntityRow(uint16_t index);
  protected:
    std::vector<BaseRowConfig*> entities;
};

#endif // __ENTITESCARDCONFIG_H__