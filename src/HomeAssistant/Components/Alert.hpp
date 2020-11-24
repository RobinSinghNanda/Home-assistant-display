#ifndef __ALERT_H__
#define __ALERT_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Alert : public Entity {
        public:
            Alert(const char * entity);
            virtual ~Alert();
            static constexpr const char * ENTITY_DOMAIN = "alert";

            //Attributes
            static constexpr AttributeName ATTR_DATA = "data";
            static constexpr AttributeName ATTR_MESSAGE = "message";
            static constexpr AttributeName ATTR_TARGET = "target";
            static constexpr AttributeName ATTR_TITLE = "title";

            //States
            static constexpr const char STATE_IDLE[] = "idle";

            //API
            inline bool isIdle();
    };

    bool Alert::isIdle() {
        return this->isState(STATE_IDLE);
    }
};

#endif // __ALERT_H__