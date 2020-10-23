#ifndef __HORIZONTALSTACKCARDCONFIG_H__
#define __HORIZONTALSTACKCARDCONFIG_H__

#include "BaseCardConfig.hpp"
#include "CardDefines.hpp"
#include <stdlib.h>
#include <vector>
#include <stdint.h>

class HorizontalStackCardConfig : public BaseCardConfig {
    public:
        HorizontalStackCardConfig();
        HorizontalStackCardConfig(const char * title, const char * icon);
        void addCard(BaseCardConfig * card);
        uint16_t getNumCards();
        BaseCardConfig * getCard(uint16_t index);
    protected:
        std::vector<BaseCardConfig*> cards;
};

#endif // __HORIZONTALSTACKCARDCONFIG_H__