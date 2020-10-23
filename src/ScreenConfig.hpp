#ifndef __SCREENCONFIG_H__
#define __SCREENCONFIG_H__

#include "LovelaceConfig/Cards.hpp"

class ScreenConfig {
  protected:
    std::vector<BaseCardConfig*> cards;
  public:
    void addCard(BaseCardConfig * baseCardConfig);
    BaseCardConfig * getCard(uint16_t index);
    uint16_t getNumCards();
    void clearCards ();
};
#endif // __SCREENCONFIG_H__