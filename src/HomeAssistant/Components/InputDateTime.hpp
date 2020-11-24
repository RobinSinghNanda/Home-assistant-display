#ifndef __INPUTDATETIME_H__
#define __INPUTDATETIME_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class InputDateTime : public Entity {
        public:
            InputDateTime(const char * entity);
            virtual ~InputDateTime();
            static constexpr const char * ENTITY_DOMAIN = "input_datetime";

            //Services
            static constexpr ServiceName SERVICE_RELOAD = "reload";
            static constexpr ServiceName SERVICE_SET_DATETIME = "set_datetime";

            //Attributes
            static constexpr AttributeName ATTR_DATETIME = "datetime";
            static constexpr AttributeName ATTR_DATE = "date";
            static constexpr AttributeName ATTR_TIME = "time";
            static constexpr AttributeName ATTR_TIMESTAMP = "timestamp";
            static constexpr AttributeName ATTR_HAS_DATE = "has_date";
            static constexpr AttributeName ATTR_HAS_TIME = "has_time";
            static constexpr AttributeName ATTR_HOUR = "hour";
            static constexpr AttributeName ATTR_MINUTE = "minute";
            static constexpr AttributeName ATTR_SECOND = "second";
            static constexpr AttributeName ATTR_YEAR = "year";
            static constexpr AttributeName ATTR_MONTH = "month";
            static constexpr AttributeName ATTR_DAY = "day";
            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            bool hasDate = false;
            bool hasTime = false;
            uint16_t year = 1970;
            uint8_t month = 0;
            uint8_t day = 0; 
            uint8_t hour = 0;
            uint8_t minute = 0;
            uint8_t second = 0;
            uint32_t timestamp = 0;
            virtual void setIcon();
    };
};

#endif // __INPUTDATETIME_H__