#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Remote : public Entity {
        public:
            Remote(const char * entity);
            virtual ~Remote();
            static constexpr const char * ENTITY_DOMAIN = "remote";

            //Features
            static constexpr Feature SUPPORT_LEARN_COMMAND = 1;
            static constexpr Feature SUPPORT_DELETE_COMMAND = 2;

            //Services
            static constexpr ServiceName SERVICE_SEND_COMMAND = "send_command";
            static constexpr ServiceName SERVICE_LEARN_COMMAND = "learn_command";
            static constexpr ServiceName SERVICE_DELETE_COMMAND = "delete_command";
            static constexpr ServiceName SERVICE_SYNC = "sync";
            
            //Attributes
            static constexpr AttributeName ATTR_ACTIVITY = "activity";
            static constexpr AttributeName ATTR_COMMAND = "command";
            static constexpr AttributeName ATTR_COMMAND_TYPE = "command_type";
            static constexpr AttributeName ATTR_DEVICE = "device";
            static constexpr AttributeName ATTR_NUM_REPEATS = "num_repeats";
            static constexpr AttributeName ATTR_DELAY_SECS = "delay_secs";
            static constexpr AttributeName ATTR_HOLD_SECS = "hold_secs";
            static constexpr AttributeName ATTR_ALTERNATIVE = "alternative";
            static constexpr AttributeName ATTR_TIMEOUT = "timeout";
    };
};

#endif // __REMOTE_H__