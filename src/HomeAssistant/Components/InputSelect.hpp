#ifndef __INPUTSELECT_H__
#define __INPUTSELECT_H__

#include "Entity.hpp"
#include "vector"

namespace HomeAssistant {
    class InputSelect : public Entity {
        public:
            InputSelect(const char * entity);
            virtual ~InputSelect();
            static constexpr const char * ENTITY_DOMAIN = "input_select";

            //Services
            static constexpr ServiceName SERVICE_RELOAD = "reload";
            static constexpr ServiceName SERVICE_SELECT_OPTION = "select_option";
            static constexpr ServiceName SERVICE_SELECT_NEXT = "select_next";
            static constexpr ServiceName SERVICE_SELECT_PREVIOUS = "select_previous";
            static constexpr ServiceName SERVICE_SET_OPTIONS = "set_options";

            //Attributes
            static constexpr AttributeName ATTR_OPTION = "option";
            static constexpr AttributeName ATTR_OPTIONS = "options";

            //API
            inline void setOption(const char * option);
            inline void setOption(uint8_t index);
            inline uint8_t getNumOptions();
            inline const char * getOption(uint8_t index);      
            inline const char * getOption(); 
            inline int8_t getOptionIndex();  
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            std::vector<string> options;
            string option = "";
    };

    void InputSelect::setOption(uint8_t index) {
        if (index < options.size()) {
            this->option = this->options[index];
        }
    }

    void InputSelect::setOption(const char * option) {
        this->option = option;
    }

    uint8_t InputSelect::getNumOptions() {
        return options.size();
    }

    const char * InputSelect::getOption(uint8_t index) {
        if (index < options.size()) {
            return options[index].c_str();
        } else {
            return "";
        }
    }

    int8_t InputSelect::getOptionIndex() {
        for (uint8_t index = 0;index<options.size();index++) {
            if (this->option == options[index]) {
                return index;
            }
        }
        return -1;
    }

    const char * InputSelect::getOption() {
        return option.c_str();
    }
};

#endif // __INPUTSELECT_H__