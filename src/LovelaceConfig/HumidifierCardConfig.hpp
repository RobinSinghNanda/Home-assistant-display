#ifndef __HUMIDIFIERCARDCONFIG_H__
#define __HUMIDIFIERCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class HumidifierCardConfig : public BaseEntityCardConfig {
    public:
        HumidifierCardConfig(const char * entity);
        HumidifierCardConfig(const char * entity, const char * name, const char * title,  const char * icon);
        static constexpr const char * TYPE = "humidifier";
};

#endif // __HUMIDIFIERCARDCONFIG_H__