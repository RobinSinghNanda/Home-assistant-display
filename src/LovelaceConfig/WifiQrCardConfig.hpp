#ifndef __WIFIQRCARDCONFIG_H__
#define __WIFIQRCARDCONFIG_H__

#include "BaseCardConfig.hpp"

class WifiQrCardConfig : public BaseCardConfig {
    public:
        WifiQrCardConfig();
        WifiQrCardConfig(const char * title, const char * icon);
        static constexpr const char * TYPE = "wifi-qr";
};

#endif // __WIFIQRCARDCONFIG_H__