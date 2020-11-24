#ifndef __PLANT_H__
#define __PLANT_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Plant : public Entity {
        public:
            Plant(const char * entity);
            virtual ~Plant();
            static constexpr const char * ENTITY_DOMAIN = "plant";
    };
};

#endif // __PLANT_H__