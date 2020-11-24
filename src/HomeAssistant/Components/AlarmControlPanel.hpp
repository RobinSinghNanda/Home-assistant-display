#ifndef __ALARMCONTROLPANEL_H__
#define __ALARMCONTROLPANEL_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class AlarmControlPanel : public Entity {
        public:
            AlarmControlPanel(const char * entity);
            virtual ~AlarmControlPanel();
            static constexpr const char * ENTITY_DOMAIN = "alarm_control_panel";

            //States
            static constexpr State STATE_ALARM_DISARMED = "disarmed";
            static constexpr State STATE_ALARM_ARMED_HOME = "armed_home";
            static constexpr State STATE_ALARM_ARMED_AWAY = "armed_away";
            static constexpr State STATE_ALARM_ARMED_NIGHT = "armed_night";
            static constexpr State STATE_ALARM_ARMED_CUSTOM_BYPASS = "armed_custom_bypass";
            static constexpr State STATE_ALARM_PENDING = "pending";
            static constexpr State STATE_ALARM_ARMING = "arming";
            static constexpr State STATE_ALARM_DISARMING = "disarming";
            static constexpr State STATE_ALARM_TRIGGERED = "triggered";

            //Features
            static constexpr Feature SUPPORT_ALARM_ARM_HOME = 1;
            static constexpr Feature SUPPORT_ALARM_ARM_AWAY = 2;
            static constexpr Feature SUPPORT_ALARM_ARM_NIGHT = 4;
            static constexpr Feature SUPPORT_ALARM_TRIGGER = 8;
            static constexpr Feature SUPPORT_ALARM_ARM_CUSTOM_BYPASS = 16;

            //Services
            static constexpr ServiceName SERVICE_ALARM_DISARM = "alarm_disarm";
            static constexpr ServiceName SERVICE_ALARM_ARM_HOME = "alarm_arm_home";
            static constexpr ServiceName SERVICE_ALARM_ARM_AWAY = "alarm_arm_away";
            static constexpr ServiceName SERVICE_ALARM_ARM_NIGHT = "alarm_arm_night";
            static constexpr ServiceName SERVICE_ALARM_ARM_CUSTOM_BYPASS = "alarm_arm_custom_bypass";
            static constexpr ServiceName SERVICE_ALARM_TRIGGER = "alarm_trigger";

            //Attributes
            static constexpr AttributeName ATTR_CODE = "code";
            static constexpr AttributeName ATTR_CODE_FORMAT = "code_format";
            static constexpr AttributeName ATTR_CHANGED_BY = "changed_by";
            static constexpr AttributeName ATTR_CODE_ARM_REQUIRED = "code_arm_required";

            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
            inline void setCode(uint32_t code);
            inline void setCode(uint8_t * code, uint8_t length);
            inline void resetCode();
            inline bool isCodeArmRequired();
        protected:
            uint32_t code;
            bool codeArmRequired = true;
            string codeFormat = "number";
            bool codeSet = false;
            virtual void setIcon();
    };
    void AlarmControlPanel::setCode(uint32_t code) {
        this->code = code;
        codeSet = true;
    }

    void AlarmControlPanel::setCode(uint8_t * code, uint8_t length) {
        uint32_t multiplier = 1;
        this->code = 0;
        for (int8_t i=length-1;i>=0;i--) {
            this->code += code[i]*multiplier;
            multiplier*=10;
        }
        codeSet = true;
    }

    void AlarmControlPanel::resetCode() {
        codeSet = false;
    }

    bool AlarmControlPanel::isCodeArmRequired() {
        return this->codeArmRequired;
    }
};

#endif // __ALARMCONTROLPANEL_H__