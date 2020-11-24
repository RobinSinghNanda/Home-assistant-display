#ifndef __CLIMATE_H__
#define __CLIMATE_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Climate : public Entity {
        public:
            typedef const char * HvacMode;
            Climate(const char * entity);
            virtual ~Climate();
            static constexpr const char * ENTITY_DOMAIN = "climate";
            
            //Features
            static constexpr Feature SUPPORT_TARGET_TEMPERATURE = 1;
            static constexpr Feature SUPPORT_TARGET_TEMPERATURE_RANGE = 2;
            static constexpr Feature SUPPORT_TARGET_HUMIDITY = 4;
            static constexpr Feature SUPPORT_FAN_MODE = 8;
            static constexpr Feature SUPPORT_PRESET_MODE = 16;
            static constexpr Feature SUPPORT_SWING_MODE = 32;
            static constexpr Feature SUPPORT_AUX_HEAT = 64;

            //States
            static constexpr State STATE_HEAT = "heat";
            static constexpr State STATE_COOL = "cool";
            static constexpr State STATE_HEAT_COOL = "heat_cool";
            static constexpr State STATE_AUTO = "auto";
            static constexpr State STATE_DRY = "dry";
            static constexpr State STATE_FAN_ONLY = "fan_only";

            //Services
            static constexpr ServiceName SERVICE_SET_AUX_HEAT = "set_aux_heat";
            static constexpr ServiceName SERVICE_SET_FAN_MODE = "set_fan_mode";
            static constexpr ServiceName SERVICE_SET_PRESET_MODE = "set_preset_mode";
            static constexpr ServiceName SERVICE_SET_HUMIDITY = "set_humidity";
            static constexpr ServiceName SERVICE_SET_HVAC_MODE = "set_hvac_mode";
            static constexpr ServiceName SERVICE_SET_SWING_MODE = "set_swing_mode";
            static constexpr ServiceName SERVICE_SET_TEMPERATURE = "set_temperature";

            //Attributes
            static constexpr AttributeName ATTR_AUX_HEAT = "aux_heat";
            static constexpr AttributeName ATTR_CURRENT_HUMIDITY = "current_humidity";
            static constexpr AttributeName ATTR_CURRENT_TEMPERATURE = "current_temperature";
            static constexpr AttributeName ATTR_FAN_MODES = "fan_modes";
            static constexpr AttributeName ATTR_FAN_MODE = "fan_mode";
            static constexpr AttributeName ATTR_PRESET_MODE = "preset_mode";
            static constexpr AttributeName ATTR_PRESET_MODES = "preset_modes";
            static constexpr AttributeName ATTR_HUMIDITY = "humidity";
            static constexpr AttributeName ATTR_MAX_HUMIDITY = "max_humidity";
            static constexpr AttributeName ATTR_MIN_HUMIDITY = "min_humidity";
            static constexpr AttributeName ATTR_TEMPERATURE = "temperature";
            static constexpr AttributeName ATTR_MAX_TEMP = "max_temp";
            static constexpr AttributeName ATTR_MIN_TEMP = "min_temp";
            static constexpr AttributeName ATTR_HVAC_ACTION = "hvac_action";
            static constexpr AttributeName ATTR_HVAC_MODES = "hvac_modes";
            static constexpr AttributeName ATTR_HVAC_MODE = "hvac_mode";
            static constexpr AttributeName ATTR_SWING_MODES = "swing_modes";
            static constexpr AttributeName ATTR_SWING_MODE = "swing_mode";
            static constexpr AttributeName ATTR_TARGET_TEMP_HIGH = "target_temp_high";
            static constexpr AttributeName ATTR_TARGET_TEMP_LOW = "target_temp_low";
            static constexpr AttributeName ATTR_TARGET_TEMP_STEP = "target_temp_step";

            //Modes
            static constexpr HvacMode HVAC_MODE_OFF = "off";
            static constexpr HvacMode HVAC_MODE_HEAT = "heat";
            static constexpr HvacMode HVAC_MODE_COOL = "cool";
            static constexpr HvacMode HVAC_MODE_HEAT_COOL = "heat_cool";
            static constexpr HvacMode HVAC_MODE_AUTO = "auto";
            static constexpr HvacMode HVAC_MODE_DRY = "dry";
            static constexpr HvacMode HVAC_MODE_FAN_ONLY = "fan_only";


            //API
            inline uint8_t getHvacModeIndex();
            inline uint8_t getNumHvacModes();
            inline void setHvacMode(uint8_t index);
            inline void setHvacMode(const char * mode);
            inline bool isHvacMode(HvacMode mode);
            inline const char * getHvacMode();
            inline float getTemperature();
            inline void setTemperature(float temperature);
            inline float getTemperatureStep();
            inline float getMaxTemperature();
            inline float getMinTemperature();
            inline int8_t getFanModeIndex();
            inline const char * getFanMode();
            inline uint8_t getNumFanModes();
            inline void setFanMode(const char * fanMode);
            inline void setFanMode(uint8_t index);
            inline int8_t getPresetModeIndex();
            inline const char * getPresetMode();
            inline uint8_t getNumPresetModes();
            inline void setPresetMode(const char * presetMode);
            inline void setPresetMode(uint8_t index);
            inline int8_t getSwingModeIndex();
            inline const char * getSwingMode();
            inline uint8_t getNumSwingModes();
            inline void setSwingMode(const char * swingMode);
            inline void setSwingMode(uint8_t index);
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            bool auxHeat = false;
            float humidity = 0;
            float currentHumidity = 0;
            float maxHumidity = 0;
            float minHumidity = 0;
            float temperature = 0;
            float targetHighTemperature = 0;
            float targetLowTemperature = 0;
            float currentTemperature = 0;
            float maxTemperature = 0;
            float minTemperature = 0;
            float temperatureStep = 1;
            std::vector<string> fanModes;
            string fanMode = "";
            std::vector<string> presetModes;
            string presetMode = "";
            std::vector<string> hvacModes;
            string hvacMode = "";
            std::vector<string> swingModes;
            string swingMode = "";
    };

    bool Climate::isHvacMode(HvacMode mode) {
        return this->hvacMode == mode;
    }

    const char * Climate::getHvacMode() {
        return this->hvacMode.c_str();
    }

    uint8_t Climate::getHvacModeIndex() {
        for (uint8_t i=0;i<hvacModes.size();i++) {
            if (this->hvacMode == hvacModes[i]) {
                return i;
            }
        }
        return -1;
    }

    uint8_t Climate::getNumHvacModes() {
        return hvacModes.size();
    }

    void Climate::setHvacMode(uint8_t index) {
        if (index < hvacModes.size()) {
            hvacMode = hvacModes[index];
        }
    }

    void Climate::setHvacMode(const char * mode) {
        this->hvacMode = mode;
    }

    float Climate::getTemperature() {
        return temperature;
    }

    void Climate::setTemperature(float temperature) {
        this->temperature = temperature;
    }

    float Climate::getTemperatureStep() {
        return this->temperatureStep;
    }

    float Climate::getMaxTemperature() {
        return this->maxTemperature;
    }

    float Climate::getMinTemperature() {
        return this->minTemperature;
    }

    int8_t Climate::getFanModeIndex() {
        for(uint8_t i=0;i<fanModes.size();i++) {
            if (fanMode == fanModes[i]) {
                return i;
            }
        }
        return -1;
    }

    const char * Climate::getFanMode() {
        return fanMode.c_str();
    }

    uint8_t Climate::getNumFanModes() {
        return this->fanModes.size();
    }

    void Climate::setFanMode(const char * fanMode) {
        this->fanMode = fanMode;
    }

    void Climate::setFanMode(uint8_t index) {
        if (index < fanModes.size()) {
            fanMode = fanModes[index];
        }
    }

    int8_t Climate::getPresetModeIndex() {
        for(uint8_t i=0;i<presetModes.size();i++) {
            if (presetMode == presetModes[i]) {
                return i;
            }
        }
        return -1;
    }

    const char * Climate::getPresetMode() {
        return presetMode.c_str();
    }

    uint8_t Climate::getNumPresetModes() {
        return this->presetModes.size();
    }

    void Climate::setPresetMode(const char * presetMode) {
        this->presetMode = presetMode;
    }

    void Climate::setPresetMode(uint8_t index) {
        if (index < presetModes.size()) {
            presetMode = presetModes[index];
        }
    }

    int8_t Climate::getSwingModeIndex() {
        for(uint8_t i=0;i<swingModes.size();i++) {
            if (swingMode == swingModes[i]) {
                return i;
            }
        }
        return -1;
    }

    const char * Climate::getSwingMode() {
        return swingMode.c_str();
    }

    uint8_t Climate::getNumSwingModes() {
        return this->swingModes.size();
    }

    void Climate::setSwingMode(const char * swingMode) {
        this->swingMode = swingMode;
    }

    void Climate::setSwingMode(uint8_t index) {
        if (index < swingModes.size()) {
            swingMode = swingModes[index];
        }
    }
};

#endif // __CLIMATE_H__