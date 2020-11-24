#ifndef __PROXIMITY_H__
#define __PROXIMITY_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Proximity : public Entity {
        public:
            Proximity(const char * entity);
            virtual ~Proximity();
            static constexpr const char * ENTITY_DOMAIN = "proximity";
    };
};


#endif // __PROXIMITY_H__