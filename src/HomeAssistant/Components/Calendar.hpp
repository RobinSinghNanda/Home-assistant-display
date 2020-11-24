#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Calendar : public Entity {
        public:
            Calendar(const char * entity);
            virtual ~Calendar();
            static constexpr const char * ENTITY_DOMAIN = "calendar";
    };
};

#endif // __CALENDAR_H__