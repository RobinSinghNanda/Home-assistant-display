#include "BinarySensor.hpp"
using namespace HomeAssistant;

BinarySensor::BinarySensor(const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_RADIOBOX_BLANK;
}

BinarySensor::~BinarySensor() {

}

void BinarySensor::setIcon() {
    bool isOff = this->isOff();
    if (this->deviceClass == DEVICE_CLASS_BATTERY) {
        this->icon = (isOff)?ICON_BATTERY:ICON_BATTERY_OUTLINE;
    } else if (this->deviceClass == DEVICE_CLASS_BATTERY_CHARGING) {
        this->icon = (isOff)?ICON_BATTERY:ICON_BATTERY_CHARGING;
    } else if (this->deviceClass == DEVICE_CLASS_COLD) {
        this->icon = (isOff)?ICON_THERMOMETER:ICON_SNOWFLAKE;
    } else if (this->deviceClass == DEVICE_CLASS_CONNECTIVITY) {
        this->icon = (isOff)?ICON_SERVER_NETWORK_OFF:ICON_SERVER_NETWORK;
    } else if (this->deviceClass == DEVICE_CLASS_DOOR) {
        this->icon = (isOff)?ICON_DOOR_CLOSED:ICON_DOOR_OPEN;
    } else if (this->deviceClass == DEVICE_CLASS_GARAGE_DOOR) {
        this->icon = (isOff)?ICON_GARAGE:ICON_GARAGE_OPEN;
    } else if (this->deviceClass == DEVICE_CLASS_POWER) {
        this->icon = (isOff)?ICON_POWER_OFF:ICON_POWER_ON;
    } else if (this->deviceClass == DEVICE_CLASS_GAS ||
        this->deviceClass == DEVICE_CLASS_PROBLEM ||
        this->deviceClass == DEVICE_CLASS_SAFETY ||
        this->deviceClass == DEVICE_CLASS_SMOKE) {
        this->icon = (isOff)?ICON_SHIELD_CHECK:ICON_ALERT;
    } else if (this->deviceClass == DEVICE_CLASS_HEAT) {
        this->icon = (isOff)?ICON_THERMOMETER:ICON_FIRE;
    } else if (this->deviceClass == DEVICE_CLASS_LIGHT) {
        this->icon = (isOff)?ICON_BRIGHTNESS_5:ICON_BRIGHTNESS_7;
    } else if (this->deviceClass == DEVICE_CLASS_LOCK) {
        this->icon = (isOff)?ICON_LOCK:ICON_LOCK_OPEN;
    } else if (this->deviceClass == DEVICE_CLASS_MOISTURE) {
        this->icon = (isOff)?ICON_WATER_OFF:ICON_WATER;
    } else if (this->deviceClass == DEVICE_CLASS_MOTION) {
        this->icon = (isOff)?ICON_WALK:ICON_RUN;
    } else if (this->deviceClass == DEVICE_CLASS_OCCUPANCY) {
        this->icon = (isOff)?ICON_HOME_OUTLINE:ICON_HOME;
    } else if (this->deviceClass == DEVICE_CLASS_OPENING) {
        this->icon = (isOff)?ICON_SQUARE:ICON_SQUARE_OUTLINE;
    } else if (this->deviceClass == DEVICE_CLASS_PLUG) {
        this->icon = (isOff)?ICON_POWER_OFF:ICON_POWER_PLUG;
    } else if (this->deviceClass == DEVICE_CLASS_PRESENCE) {
        this->icon = (isOff)?ICON_HOME_OUTLINE:ICON_HOME;
    } else if (this->deviceClass == DEVICE_CLASS_SOUND) {
        this->icon = (isOff)?ICON_MUSIC_NOTE_OFF:ICON_MUSIC_NOTE;
    } else if (this->deviceClass == DEVICE_CLASS_VIBRATION) {
        this->icon = (isOff)?ICON_CROP_PORTRAIT:ICON_VIBRATE;
    } else if (this->deviceClass == DEVICE_CLASS_WINDOW) {
        this->icon = (isOff)?ICON_WINDOW_CLOSED:ICON_WINDOW_OPEN;
    } else {
        this->icon = (isOff)?ICON_RADIOBOX_BLANK:ICON_CHECKBOX_MARKED_CIRCLE;
    }
}