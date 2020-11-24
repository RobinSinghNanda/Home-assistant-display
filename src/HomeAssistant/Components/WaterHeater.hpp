#ifndef __WATERHEATER_H__
#define __WATERHEATER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class WaterHeater : public Entity {
        public:
            WaterHeater(const char * entity);
            virtual ~WaterHeater();
            static constexpr const char * ENTITY_DOMAIN = "water_heater";

            //Features
            static constexpr Feature SUPPORT_TARGET_TEMPERATURE = 1;
            static constexpr Feature SUPPORT_OPERATION_MODE = 2;
            static constexpr Feature SUPPORT_AWAY_MODE = 4;

            //States
            static constexpr State STATE_ECO = "eco";
            static constexpr State STATE_ELECTRIC = "electric";
            static constexpr State STATE_PERFORMANCE = "performance";
            static constexpr State STATE_HIGH_DEMAND = "high_demand";
            static constexpr State STATE_HEAT_PUMP = "heat_pump";
            static constexpr State STATE_GAS = "gas";

            //Services
            static constexpr ServiceName SERVICE_SET_AWAY_MODE = "set_away_mode";
            static constexpr ServiceName SERVICE_SET_TEMPERATURE = "set_temperature";
            static constexpr ServiceName SERVICE_SET_OPERATION_MODE = "set_operation_mode";

            //Attributes
            static constexpr AttributeName ATTR_TEMPERATURE = "temperature";
            static constexpr AttributeName ATTR_MAX_TEMP = "max_temp";
            static constexpr AttributeName ATTR_MIN_TEMP = "min_temp";
            static constexpr AttributeName ATTR_AWAY_MODE = "away_mode";
            static constexpr AttributeName ATTR_OPERATION_MODE = "operation_mode";
            static constexpr AttributeName ATTR_OPERATION_LIST = "operation_list";
            static constexpr AttributeName ATTR_TARGET_TEMP_HIGH = "target_temp_high";
            static constexpr AttributeName ATTR_TARGET_TEMP_LOW = "target_temp_low";
            static constexpr AttributeName ATTR_CURRENT_TEMPERATURE = "current_temperature";

            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            float currentTemperature = 0;
            float temperature = 0;
            float maxTemperature = 0;
            float minTemperature = 0;
            string awayMode = "";
            string operationMode = "";
            vector<string> operationModes;
    };
};

#endif // __WATERHEATER_H__