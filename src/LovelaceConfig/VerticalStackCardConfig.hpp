#ifndef __VERTICALSTACKCARDCONFIG_H__
#define __VERTICALSTACKCARDCONFIG_H__

#include "BaseCardConfig.hpp"
#include "CardDefines.hpp"
#include <stdlib.h>
#include <vector>
#include <stdint.h>

class VerticalStackCardConfig : public BaseCardConfig {
    public:
        VerticalStackCardConfig();
        VerticalStackCardConfig(const char * title, const char * icon);
        void addCard(BaseCardConfig * card);
        uint16_t getNumCards();
        BaseCardConfig * getCard(uint16_t index);
    protected:
        std::vector<BaseCardConfig*> cards;
};

#endif // __VERTICALSTACKCARDCONFIG_H__