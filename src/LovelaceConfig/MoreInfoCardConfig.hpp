#ifndef __MOREINFOCARDCONFIG_H__
#define __MOREINFOCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class MoreInfoCardConfig : public BaseEntityCardConfig {
    public:
        MoreInfoCardConfig(const char * entity);
        MoreInfoCardConfig(const char * entity, const char * name, const char * title,  const char * icon);
        MoreInfoCardConfig(const char * entity, const char * name, const char * title,  const char * icon, const char * rowIcon);
        static constexpr const char * TYPE = "more-info";
};

#endif // __MOREINFOCARDCONFIG_H__