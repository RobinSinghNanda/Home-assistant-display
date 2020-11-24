#include "WifiQrCardConfig.hpp"

WifiQrCardConfig::WifiQrCardConfig() : WifiQrCardConfig("", "") {

}

WifiQrCardConfig::WifiQrCardConfig(const char * title, const char * icon) : BaseCardConfig(TYPE, title, icon) {

}