#include "DigitalClockCardConfig.hpp"

DigitalClockCardConfig::DigitalClockCardConfig() : DigitalClockCardConfig("", "") {

}

DigitalClockCardConfig::DigitalClockCardConfig(const char * title, const char * icon) : BaseCardConfig(TYPE, title, icon) {

}