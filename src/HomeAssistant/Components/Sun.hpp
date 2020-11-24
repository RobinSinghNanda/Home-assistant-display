#ifndef __SUN_H__
#define __SUN_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Sun : public Entity {
        public:
            Sun(const char * entity);
            virtual ~Sun();
            static constexpr const char * ENTITY_DOMAIN = "sun";
    };
};

#endif // __SUN_H__