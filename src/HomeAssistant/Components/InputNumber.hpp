#ifndef __INPUTNUMBER_H__
#define __INPUTNUMBER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class InputNumber : public Entity {
        public:
            InputNumber(const char * entity);
            virtual ~InputNumber();
            static constexpr const char * ENTITY_DOMAIN = "input_number";

            //Services
            static constexpr ServiceName SERVICE_RELOAD = "reload";
            static constexpr ServiceName SERVICE_SET_VALUE = "set_value";
            static constexpr ServiceName SERVICE_INCREMENT = "increment";
            static constexpr ServiceName SERVICE_DECREMENT = "decrement";

            //Attributes
            static constexpr AttributeName ATTR_INITIAL = "initial";
            static constexpr AttributeName ATTR_VALUE = "value";
            static constexpr AttributeName ATTR_MIN = "min";
            static constexpr AttributeName ATTR_MAX = "max";
            static constexpr AttributeName ATTR_STEP = "step";


            //API
            inline void setValue(float value);
            inline float getValue();
            inline float getMin();
            inline float getMax();
            inline float getStep();
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            float value = 0;
            float min = 0;
            float max = 0;
            float step = 0;
    };

    void InputNumber::setValue(float value) {
        this->value = value;
    }

    float InputNumber::getValue() {
        return this->value;
    }

    float InputNumber::getMin(){
        return this->min;
    }

    float InputNumber::getMax() {
        return this->max;
    }

    float InputNumber::getStep() {
        return this->step;
    }
};

#endif // __INPUTNUMBER_H__