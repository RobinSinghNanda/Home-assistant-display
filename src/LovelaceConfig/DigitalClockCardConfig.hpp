#ifndef __DIGITALCLOCKCARDCONFIG_H__
#define __DIGITALCLOCKCARDCONFIG_H__

#include "BaseCardConfig.hpp"

class DigitalClockCardConfig : public BaseCardConfig {
    public:
        DigitalClockCardConfig();
        DigitalClockCardConfig(const char * title, const char * icon);
        static constexpr const char * TYPE = "digital-clock";
};

#endif // __DIGITALCLOCKCARDCONFIG_H__