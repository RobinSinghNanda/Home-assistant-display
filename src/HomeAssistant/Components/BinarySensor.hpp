#ifndef __BINARYSENSOR_H__
#define __BINARYSENSOR_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class BinarySensor : public Entity {
        public:
            BinarySensor(const char * entity);
            virtual ~BinarySensor();
            static constexpr const char * ENTITY_DOMAIN = "binary_sensor";

            //Device classes
            static constexpr DeviceClassName DEVICE_CLASS_BATTERY = "battery";
            static constexpr DeviceClassName DEVICE_CLASS_BATTERY_CHARGING = "battery_charging";
            static constexpr DeviceClassName DEVICE_CLASS_COLD = "cold";
            static constexpr DeviceClassName DEVICE_CLASS_CONNECTIVITY = "connectivity";
            static constexpr DeviceClassName DEVICE_CLASS_DOOR = "door";
            static constexpr DeviceClassName DEVICE_CLASS_GARAGE_DOOR = "garage_door";
            static constexpr DeviceClassName DEVICE_CLASS_GAS = "gas";
            static constexpr DeviceClassName DEVICE_CLASS_HEAT = "heat";
            static constexpr DeviceClassName DEVICE_CLASS_LIGHT = "light";
            static constexpr DeviceClassName DEVICE_CLASS_LOCK = "lock";
            static constexpr DeviceClassName DEVICE_CLASS_MOISTURE = "moisture";
            static constexpr DeviceClassName DEVICE_CLASS_MOTION = "motion";
            static constexpr DeviceClassName DEVICE_CLASS_MOVING = "moving";
            static constexpr DeviceClassName DEVICE_CLASS_OCCUPANCY = "occupancy";
            static constexpr DeviceClassName DEVICE_CLASS_OPENING = "opening";
            static constexpr DeviceClassName DEVICE_CLASS_PLUG = "plug";
            static constexpr DeviceClassName DEVICE_CLASS_POWER = "power";
            static constexpr DeviceClassName DEVICE_CLASS_PRESENCE = "presence";
            static constexpr DeviceClassName DEVICE_CLASS_PROBLEM = "problem";
            static constexpr DeviceClassName DEVICE_CLASS_SAFETY = "safety";
            static constexpr DeviceClassName DEVICE_CLASS_SMOKE = "smoke";
            static constexpr DeviceClassName DEVICE_CLASS_SOUND = "sound";
            static constexpr DeviceClassName DEVICE_CLASS_VIBRATION = "vibration";
            static constexpr DeviceClassName DEVICE_CLASS_WINDOW = "window";

            //API
        protected:
            virtual void setIcon();
    };
};

#endif // __BINARYSENSOR_H__