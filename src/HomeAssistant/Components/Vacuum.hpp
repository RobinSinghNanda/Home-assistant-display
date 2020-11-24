#ifndef __VACUUM_H__
#define __VACUUM_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Vacuum : public Entity {
        public:
            Vacuum(const char * entity);
            virtual ~Vacuum();
            static constexpr const char * ENTITY_DOMAIN = "vacuum";

            //Features
            static constexpr Feature SUPPORT_TURN_ON = 1;
            static constexpr Feature SUPPORT_TURN_OFF = 2;
            static constexpr Feature SUPPORT_PAUSE = 4;
            static constexpr Feature SUPPORT_STOP = 8;
            static constexpr Feature SUPPORT_RETURN_HOME = 16;
            static constexpr Feature SUPPORT_FAN_SPEED = 32;
            static constexpr Feature SUPPORT_BATTERY = 64;
            static constexpr Feature SUPPORT_STATUS = 128;
            static constexpr Feature SUPPORT_SEND_COMMAND = 256;
            static constexpr Feature SUPPORT_LOCATE = 512;
            static constexpr Feature SUPPORT_CLEAN_SPOT = 1024;
            static constexpr Feature SUPPORT_MAP = 2048;
            static constexpr Feature SUPPORT_STATE = 4096;
            static constexpr Feature SUPPORT_START = 8192;

            //Services
            static constexpr ServiceName SERVICE_CLEAN_SPOT = "clean_spot";
            static constexpr ServiceName SERVICE_LOCATE = "locate";
            static constexpr ServiceName SERVICE_RETURN_TO_BASE = "return_to_base";
            static constexpr ServiceName SERVICE_SEND_COMMAND = "send_command";
            static constexpr ServiceName SERVICE_SET_FAN_SPEED = "set_fan_speed";
            static constexpr ServiceName SERVICE_START_PAUSE = "start_pause";
            static constexpr ServiceName SERVICE_START = "start";
            static constexpr ServiceName SERVICE_PAUSE = "pause";
            static constexpr ServiceName SERVICE_STOP = "stop";

            //Attributes
            static constexpr AttributeName ATTR_BATTERY_ICON = "battery_icon";
            static constexpr AttributeName ATTR_CLEANED_AREA = "cleaned_area";
            static constexpr AttributeName ATTR_FAN_SPEED = "fan_speed";
            static constexpr AttributeName ATTR_FAN_SPEED_LIST = "fan_speed_list";
            static constexpr AttributeName ATTR_PARAMS = "params";
            static constexpr AttributeName ATTR_STATUS = "status";
            static constexpr AttributeName ATTR_COMMAND = "command";

            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            string fanSpeed = "";
            vector<string> fanSpeeds;
            string command = "";
            string params = "";
    };
};

#endif // __VACUUM_H__