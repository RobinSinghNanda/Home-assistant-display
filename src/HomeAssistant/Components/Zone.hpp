#ifndef __ZONE_H__
#define __ZONE_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Zone : public Entity {
        public:
            Zone(const char * entity);
            virtual ~Zone();
            static constexpr const char * ENTITY_DOMAIN = "zone";
    };
};

#endif // __ZONE_H__