#ifndef __NOTIFY_H__
#define __NOTIFY_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Notify : public Entity {
        public:
            Notify(const char * entity);
            virtual ~Notify();
            static constexpr const char * ENTITY_DOMAIN = "notify";

            //Attributes
            static constexpr AttributeName ATTR_DATA = "data";
            static constexpr AttributeName ATTR_MESSAGE = "message";
            static constexpr AttributeName ATTR_TARGET = "target";
            static constexpr AttributeName ATTR_TITLE = "title";

            //Services
            static constexpr ServiceName SERVICE_NOTIFY = "notify";
            static constexpr ServiceName SERVICE_PERSISTENT_NOTIFICATION = "persistent_notification";
    };
};

#endif // __NOTIFY_H__