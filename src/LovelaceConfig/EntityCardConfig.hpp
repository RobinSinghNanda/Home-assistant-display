#ifndef __ENTITYCARDCONFIG_H__
#define __ENTITYCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class EntityCardConfig : public BaseEntityCardConfig {
    public:
        EntityCardConfig(const char * entity);
        EntityCardConfig(const char * entity, const char * name, const char * attribute, const char * unit, const char * title,  const char * icon);
        static constexpr const char * TYPE = "entity";
    protected:
        string attribute;
        string unit;
};

#endif // __ENTITYCARDCONFIG_H__