#ifndef __FAN_H__
#define __FAN_H__

#include "Entity.hpp"
#include <vector>
namespace HomeAssistant {
    enum FanDirection : bool {
        Clockwise = false,
        AntiClockwise = true 
    };

    class Fan: public Entity {
        public:
            Fan(const char * entity);
            virtual ~Fan();
            static constexpr const char * ENTITY_DOMAIN = "fan";

            //Features
            static constexpr Feature SUPPORT_SET_SPEED = 1;
            static constexpr Feature SUPPORT_OSCILLATE = 2;
            static constexpr Feature SUPPORT_DIRECTION = 4;

            //Services
            static constexpr ServiceName SERVICE_SET_SPEED = "set_speed";
            static constexpr ServiceName SERVICE_SET_DIRECTION = "set_direction";
            static constexpr ServiceName SERVICE_OSCILLATE = "oscillate";

            //Attributes
            static constexpr AttributeName ATTR_SPEED = "speed";
            static constexpr AttributeName ATTR_SPEED_LIST = "speed_list";
            static constexpr AttributeName ATTR_OSCILLATING = "oscillating";
            static constexpr AttributeName ATTR_DIRECTION = "direction";

            //Constants
            static constexpr const char SPEED_OFF[] = "off";
            static constexpr const char SPEED_LOW[] = "low";
            static constexpr const char SPEED_MEDIUM[] = "medium";
            static constexpr const char SPEED_HIGH[] = "high";
            static constexpr const char DIRECTION_FORWARD[] = "forward";
            static constexpr const char DIRECTION_REVERSE[] = "reverse";

            //API
            inline const char * getSpeedString(uint8_t index);
            inline int8_t getSpeedIndex(const char * speed);
            inline int8_t getSpeedIndex();
            inline uint8_t getNumSpeeds();
            inline void setSpeed(const char * speed);
            inline void setSpeed(uint8_t speedIndex);
            inline const char * getSpeed();
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            string speed;
            vector<string> speedList;
            float direction = 0;
            bool osscillate = false;
    };

    void Fan::setSpeed(const char * speed) {
        this->speed = speed;
    }
    
    void Fan::setSpeed(uint8_t speedIndex) {
        this->speed = getSpeedString(speedIndex);
    }

    const char * Fan::getSpeed() {
        return this->speed.c_str();
    }

    const char * Fan::getSpeedString(uint8_t index) {
        if (index < speedList.size()) {
            return speedList[index].c_str();
        }
        return "";
    }

    int8_t Fan::getSpeedIndex(const char * speed) {
        for (uint8_t i=0;i<speedList.size();i++) {
            if (speedList[i].compare(speed)) {
                return i;
            }
        }
        return -1;
    }

    int8_t Fan::getSpeedIndex() {
        for (uint8_t i=0;i<speedList.size();i++) {
            if (speedList[i].compare(this->speed)) {
                return i;
            }
        }
        return -1;
    }

    uint8_t Fan::getNumSpeeds() {
        return this->speedList.size();
    }
};

#endif // __FAN_H__