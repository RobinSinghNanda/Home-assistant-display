#include "Sensor.hpp"
#include <string>
using namespace HomeAssistant;

Sensor::Sensor(const char * entity) : Entity(entity, Sensor::ENTITY_DOMAIN) {
    icon = ICON_EYE;
}

Sensor::~Sensor() {

}

void Sensor::setIcon() {
    if (this->deviceClass != "") {
        if (this->deviceClass == DEVICE_CLASS_CURRENT) {
            this->icon = ICON_CURRENT_AC;
        } else if (this->deviceClass == DEVICE_CLASS_ENERGY) {
            this->icon = ICON_FLASH;
        } else if (this->deviceClass == DEVICE_CLASS_HUMIDITY) {
            this->icon = ICON_WATER_PERCENT;
        } else if (this->deviceClass == DEVICE_CLASS_ILLUMINANCE) {
            this->icon = ICON_BRIGHTNESS_5;
        } else if (this->deviceClass == DEVICE_CLASS_TEMPERATURE) {
            this->icon = ICON_THERMOMETER;
        } else if (this->deviceClass == DEVICE_CLASS_PRESSURE) {
            this->icon = ICON_GAUGE;
        } else if (this->deviceClass == DEVICE_CLASS_POWER) {
            this->icon = ICON_FLASH;
        } else if (this->deviceClass == DEVICE_CLASS_POWER_FACTOR) {
            this->icon = ICON_ANGLE_ACUTE;
        } else if (this->deviceClass == DEVICE_CLASS_SIGNAL_STRENGTH) { 
            this->icon = ICON_WIFI;
        } else if (this->deviceClass == DEVICE_CLASS_TIMESTAMP) {
            this->icon = ICON_CLOCK;
        } else if (this->deviceClass == DEVICE_CLASS_VOLTAGE) {
            this->icon = ICON_SINE_WAVE;
        } else if (this->deviceClass == DEVICE_CLASS_BATTERY) {
            if (this->state != "") {
                int32_t battery;
                sscanf(this->state.c_str(), "%d",&battery);
                int32_t batteryRound = (battery / 10) * 10;
                if (battery <= 5) {
                    this->icon = ICON_BATTERY_ALERT;
                } else if (battery > 5 && battery < 95) {
                    char icon[40];
                    snprintf(icon, sizeof(icon), "%s-%d", ICON_BATTERY, batteryRound);
                    this->icon = icon;
                } else {
                    this->icon = ICON_BATTERY;
                }
            } else {
                this->icon = ICON_BATTERY;
            }
        }
    } else if (this->unitOfMeasurement == UNIT_C ||
                this->unitOfMeasurement == UNIT_F) {
        this->icon = ICON_THERMOMETER;
    } else {
        this->icon = ICON_EYE;
    }
}