#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Sensor : public Entity {
        public:
            Sensor(const char * entity);
            virtual ~Sensor();
            static constexpr const char * ENTITY_DOMAIN = "sensor";
            
            //device classes
            static constexpr DeviceClassName DEVICE_CLASS_BATTERY = "battery";
            static constexpr DeviceClassName DEVICE_CLASS_HUMIDITY = "humidity";
            static constexpr DeviceClassName DEVICE_CLASS_ILLUMINANCE = "illuminance";
            static constexpr DeviceClassName DEVICE_CLASS_SIGNAL_STRENGTH = "signal_strength";
            static constexpr DeviceClassName DEVICE_CLASS_TEMPERATURE = "temperature";
            static constexpr DeviceClassName DEVICE_CLASS_TIMESTAMP = "timestamp";
            static constexpr DeviceClassName DEVICE_CLASS_PRESSURE = "pressure";
            static constexpr DeviceClassName DEVICE_CLASS_POWER = "power";
            static constexpr DeviceClassName DEVICE_CLASS_CURRENT = "current";
            static constexpr DeviceClassName DEVICE_CLASS_ENERGY = "energy";
            static constexpr DeviceClassName DEVICE_CLASS_POWER_FACTOR = "power_factor";
            static constexpr DeviceClassName DEVICE_CLASS_VOLTAGE = "voltage";

            //Units
            static constexpr const char * UNIT_C = "°C";
            static constexpr const char * UNIT_F = "°F";
        protected:
            string unitOfMeasurement = "";
            virtual void setIcon();
    };
};

#endif // __SENSOR_H__