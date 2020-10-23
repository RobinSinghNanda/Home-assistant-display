#ifndef __ENTITESCARDCONFIG_H__
#define __ENTITESCARDCONFIG_H__

#include "BaseRowConfig.hpp"
#include "BaseCardConfig.hpp"
#include <stdint.h>
#include <string.h>
#include "Cards.hpp"
#include <vector>

class EntitesCardConfig : public BaseCardConfig {
  public:
    EntitesCardConfig();
    EntitesCardConfig(const char * title, const char * icon);
    void addEntity(BaseRowConfig * rowConfig);
    uint16_t getNumEntites();
    BaseRowConfig * getEntityRow(uint16_t index);
  protected:
    std::vector<BaseRowConfig*> entities;
};

#endif // __ENTITESCARDCONFIG_H__