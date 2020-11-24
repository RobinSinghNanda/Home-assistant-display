#ifndef __LOCK_H__
#define __LOCK_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Lock : public Entity {
        public:
            Lock(const char * entity);
            virtual ~Lock();
            static constexpr const char * ENTITY_DOMAIN = "lock";

            //States
            static constexpr State STATE_LOCKED = "locked";
            static constexpr State STATE_UNLOCKED = "unlocked";

            //Features
            static constexpr Feature SUPPORT_OPEN = 1;

            //Services
            static constexpr ServiceName SERVICE_LOCK = "lock";
            static constexpr ServiceName SERVICE_UNLOCK = "unlock";
            static constexpr ServiceName SERVICE_OPEN = "open";
            
            //Attributes
            static constexpr AttributeName ATTR_CHANGED_BY = "changed_by";
            static constexpr AttributeName ATTR_CODE = "code";
            static constexpr AttributeName ATTR_CODE_FORMAT = "code_format";

        protected:
            virtual void setIcon();
    };
};

#endif // __LOCK_H__