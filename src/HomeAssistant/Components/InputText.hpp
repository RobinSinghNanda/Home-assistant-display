#ifndef __INPUTTEXT_H__
#define __INPUTTEXT_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class InputText : public Entity {
        public:
            typedef const char * Mode;
            InputText(const char * entity);
            virtual ~InputText();
            static constexpr const char * ENTITY_DOMAIN = "input_text";

            //Services
            static constexpr ServiceName SERVICE_SET_VALUE = "set_value";

            //Attributes
            static constexpr AttributeName ATTR_VALUE = "value";
            static constexpr AttributeName ATTR_MIN = "min";
            static constexpr AttributeName ATTR_MAX = "max";
            static constexpr AttributeName ATTR_PATTERN = "pattern";
            static constexpr AttributeName ATTR_MODE = "mode";

            //Modes
            static constexpr Mode MODE_TEXT = "text";
            static constexpr Mode MODE_PASSWORD = "password";

            //API
            inline bool isMode(Mode mode);
            inline Mode getMode();
            inline bool isValue(const char * value);
            inline const char * getValue();
            inline void setValue(const char * value);
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            string value = "";
            uint8_t min;
            uint8_t max;
            string mode = "text";
    };
    const char * InputText::getValue() {
        return this->value.c_str();
    }
    
    void InputText::setValue(const char * value) {
        this->value = value;
    }

    bool InputText::isValue(const char * value) {
        return this->value.compare(value) == 0;
    }

    bool InputText::isMode(Mode mode) {
        return this->mode.compare(mode) == 0;
    }

    InputText::Mode InputText::getMode() {
        return this->mode.c_str();
    }
};

#endif // __INPUTTEXT_H__