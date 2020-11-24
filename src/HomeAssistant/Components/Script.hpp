#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Script : public Entity {
        public:
            Script(const char * entity);
            virtual ~Script();
            static constexpr const char * ENTITY_DOMAIN = "script";

            //Attributes
            static constexpr AttributeName ATTR_LAST_ACTION = "last_action";
            static constexpr AttributeName ATTR_LAST_TRIGGERED = "last_triggered";
            static constexpr AttributeName ATTR_VARIABLES = "variables";
            static constexpr AttributeName ATTR_CUR = "current";
            static constexpr AttributeName ATTR_MAX = "max";
            static constexpr AttributeName ATTR_MODE = "mode";
    };
};


#endif // __SCRIPT_H__