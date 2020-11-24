#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Switch : public Entity {
        public:
            Switch(const char * entityId);
            virtual ~Switch();
            static constexpr const char * ENTITY_DOMAIN = "switch";

            //Device classes
            static constexpr DeviceClassName DEVICE_CLASS_OUTLET = "outlet";
            static constexpr DeviceClassName DEVICE_CLASS_SWITCH = "switch";
            
            //Attributes
            static constexpr AttributeName ATTR_TODAY_ENERGY_KWH = "today_energy_kwh";
            static constexpr AttributeName ATTR_CURRENT_POWER_W = "current_power_w";

            inline void setTodayEnergy(float totalEnergy);
            inline float getTodayEnergy();
            inline void setCurrentPower(float currentPower);
            inline float getCurrenPower();
            virtual void updateAttributes(JsonObject stateAttributesObject);
        protected:
            float todayEnergy = 0;
            float currentPower = 0;

    };

    void Switch::setTodayEnergy(float totalEnergy) {
        this->todayEnergy = todayEnergy;
    }

    float Switch::getTodayEnergy() {
        return this->todayEnergy;
    }

    void Switch::setCurrentPower(float currentPower) {
        this->currentPower = currentPower;
    }

    float Switch::getCurrenPower() {
        return this->currentPower;
    }
};

#endif // __SWITCH_H__