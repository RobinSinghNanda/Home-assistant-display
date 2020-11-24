#ifndef __BUTTONCARDCONFIG_H__
#define __BUTTONCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class CardActionConfig {
    public:
        CardActionConfig(const char * action, const char * service, const char * serviceData, bool confirmation);
        inline const char * getAction();
        inline const char * getService();
        inline const char * getServiceData();
        inline bool getConfirmation();
    protected:
        string action;
        string service;
        string serviceData;
        bool confirmation;
};

class ButtonCardConfig : public BaseEntityCardConfig {
    public:
        ButtonCardConfig();
        ButtonCardConfig(const char * entity, const char * name, CardActionConfig * tapAction, CardActionConfig * holdAction, CardActionConfig * doubleTapAction, const char * title,  const char * icon);
        static constexpr const char * TYPE = "button";
    protected:
        bool showName = true;
        bool showState = true;
        bool showIcon = true;
        bool stateColor = 2;
        CardActionConfig * tapAction;
        CardActionConfig * holdAction;
        CardActionConfig * doubleTapAction;
};

const char * CardActionConfig::getAction() {
    return this->action.c_str();
}

const char * CardActionConfig::getService() {
    return this->service.c_str();
}

const char * CardActionConfig::getServiceData() {
    return this->serviceData.c_str();
}

bool CardActionConfig::getConfirmation() {
    return this->confirmation;
}

#endif // __BUTTONCARDCONFIG_H__