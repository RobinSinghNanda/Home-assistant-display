#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Counter : public Entity {
        public:
            Counter(const char * entity);
            virtual ~Counter();
            static constexpr const char * ENTITY_DOMAIN = "counter";

            //Services
            static constexpr ServiceName SERVICE_INCREMENT = "increment";
            static constexpr ServiceName SERVICE_DECREMENT = "decrement";
            static constexpr ServiceName SERVICE_RESET     = "reset";
            static constexpr ServiceName SERVICE_CONFIGURE =  "configure";

            //Atrributes
            static constexpr AttributeName ATTR_INITIAL = "initial";
            static constexpr AttributeName ATTR_STEP = "step";
            static constexpr AttributeName ATTR_MINIMUM = "minimum";
            static constexpr AttributeName ATTR_MAXIMUM = "maximum";

            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
        protected:
            uint32_t min;
            uint32_t max;
            uint32_t step;
            uint32_t value;

            
    };
};

#endif // __COUNTER_H__