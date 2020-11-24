#include "ButtonCardConfig.hpp"

CardActionConfig nullAction("", "", "", false);

ButtonCardConfig::ButtonCardConfig(const char * entity, const char * name, CardActionConfig * tapAction, CardActionConfig * holdAction, CardActionConfig * doubleTapAction, const char * title,  const char * icon) :
    BaseEntityCardConfig(TYPE, entity, title, icon, 2) {
        this->tapAction = tapAction;
        this->holdAction = holdAction;
        this->doubleTapAction = doubleTapAction;
}

ButtonCardConfig::ButtonCardConfig() :
    ButtonCardConfig("", "", &nullAction, &nullAction, &nullAction, "", "") {

}

CardActionConfig::CardActionConfig(const char * action, const char * service, const char * serviceData, bool confirmation) {
    this->action = action;
    this->service = service;
    this->serviceData = serviceData;
    this->confirmation = confirmation;
}