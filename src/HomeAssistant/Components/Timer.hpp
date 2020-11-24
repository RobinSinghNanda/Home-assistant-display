#ifndef __TIMER_H__
#define __TIMER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Timer : public Entity {
        public:
            Timer(const char * entity);
            virtual ~Timer();
            static constexpr const char * ENTITY_DOMAIN = "timer";

            //Services
            static constexpr ServiceName SERVICE_START = "start";
            static constexpr ServiceName SERVICE_PAUSE = "pause";
            static constexpr ServiceName SERVICE_CANCEL = "cancel";
            static constexpr ServiceName SERVICE_FINISH = "finish";

            //Attributes
            static constexpr AttributeName ATTR_DURATION = "duration";
            static constexpr AttributeName ATTR_REMAINING = "remaining";
            static constexpr AttributeName ATTR_FINISHES_AT = "finishes_at";

            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            string duration = "";
            string remaining = "";
            uint32_t finishesAt = 0; 
    };
};

#endif // __TIMER_H__