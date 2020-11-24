#ifndef __HUMIDIFIER_H__
#define __HUMIDIFIER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Humidifier : public Entity {
        public:
            typedef const char * Mode;
            Humidifier(const char * entity);
            virtual ~Humidifier();
            static constexpr const char * ENTITY_DOMAIN = "humidifier";

            //Features
            static constexpr Feature SUPPORT_MODES = 1;

            //Services
            static constexpr ServiceName SERVICE_SET_MODE = "set_mode";
            static constexpr ServiceName SERVICE_SET_HUMIDITY = "set_humidity";

            //Attributes
            static constexpr AttributeName ATTR_MODE = "mode";
            static constexpr AttributeName ATTR_AVAILABLE_MODES = "available_modes";
            static constexpr AttributeName ATTR_HUMIDITY = "humidity";
            static constexpr AttributeName ATTR_MAX_HUMIDITY = "max_humidity";
            static constexpr AttributeName ATTR_MIN_HUMIDITY = "min_humidity";

            //Modes
            static constexpr Mode MODE_NORMAL = "normal";
            static constexpr Mode MODE_ECO = "eco";
            static constexpr Mode MODE_AWAY = "away";
            static constexpr Mode MODE_BOOST = "boost";
            static constexpr Mode MODE_COMFORT = "comfort";
            static constexpr Mode MODE_HOME = "home";
            static constexpr Mode MODE_SLEEP = "sleep";
            static constexpr Mode MODE_AUTO = "auto";
            static constexpr Mode MODE_BABY = "baby";

            //API
            inline uint8_t getModeIndex();
            inline uint8_t getNumModes();
            inline void setMode(uint8_t index);
            inline void setMode(const char * mode);
            inline bool isMode(Mode mode);
            inline const char * getMode();
            inline float getHumidity();
            inline void setHumidity(float humidity);
            inline float getMaxHumidity();
            inline float getMinHumidity();
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);

        protected:
            string mode = "";
            vector<string> modes;
            float humidity = 0;
            float minHumidity = 0;
            float maxHumidity = 0;
            virtual void setIcon();
    };

    bool Humidifier::isMode(Mode mode) {
        return this->mode == mode;
    }

    const char * Humidifier::getMode() {
        return this->mode.c_str();
    }

    uint8_t Humidifier::getModeIndex() {
        for (uint8_t i=0;i<modes.size();i++) {
            if (this->mode == modes[i]) {
                return i;
            }
        }
        return -1;
    }

    uint8_t Humidifier::getNumModes() {
        return modes.size();
    }

    void Humidifier::setMode(uint8_t index) {
        if (index < modes.size()) {
            mode = modes[index];
        }
    }

    void Humidifier::setMode(const char * mode) {
        this->mode = mode;
    }

    float Humidifier::getHumidity() {
        return humidity;
    }

    void Humidifier::setHumidity(float humidity) {
        this->humidity = humidity;
    }
    
    float Humidifier::getMaxHumidity() {
        return this->maxHumidity;
    }

    float Humidifier::getMinHumidity() {
        return this->minHumidity;
    }
};

#endif // __HUMIDIFIER_H__