#ifndef __AUTOMATION_H__
#define __AUTOMATION_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Automation : public Entity {
        public:
            Automation(const char * entity);
            virtual ~Automation();
            static constexpr const char * ENTITY_DOMAIN = "automation";

            //Attributes
            static constexpr AttributeName ATTR_CUR = "current";
            static constexpr AttributeName ATTR_MAX = "max";
            static constexpr AttributeName ATTR_MODE = "mode";
            static constexpr AttributeName ATTR_LAST_TRIGGERED = "last_triggered";
            static constexpr AttributeName ATTR_SOURCE = "source";
            static constexpr AttributeName ATTR_VARIABLES = "variables";

            //Modes
            static constexpr const char * SCRIPT_MODE_PARALLEL = "parallel";
            static constexpr const char * SCRIPT_MODE_QUEUED = "queued";
            static constexpr const char * SCRIPT_MODE_RESTART = "restart";
            static constexpr const char * SCRIPT_MODE_SINGLE = "single";

            //Services
            static constexpr ServiceName SERVICE_TRIGGER = "trigger";
    };
};

#endif // __AUTOMATION_H__