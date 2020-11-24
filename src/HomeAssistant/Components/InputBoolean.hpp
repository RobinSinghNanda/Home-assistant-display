#ifndef __INPUTBOOLEAN_H__
#define __INPUTBOOLEAN_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class InputBoolean : public Entity {
        public:
            InputBoolean(const char * entity);
            virtual ~InputBoolean();
            static constexpr const char * ENTITY_DOMAIN = "input_boolean";

            //Services
            static constexpr ServiceName SERVICE_RELOAD = "reload";
    };
};

#endif // __INPUTBOOLEAN_H__