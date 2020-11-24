#ifndef __ALARMPANELCARDCONFIG_H__
#define __ALARMPANELCARDCONFIG_H__

#include "BaseCardConfig.hpp"
#include <list>

typedef std::list<string> * listPtr;
class AlarmPanelCardConfig : public BaseCardConfig {
    public:
        AlarmPanelCardConfig(const char * entity);
        AlarmPanelCardConfig(const char * entity, const char * name, std::list<string> * states);
        static constexpr const char * TYPE = "alarm-panel";
        inline const char * getEntityId();
        inline const listPtr getStates();
    protected:
        string entity;
        string name;
        std::list<string> states;
};

const char * AlarmPanelCardConfig::getEntityId() {
    return this->entity.c_str();
}

const listPtr AlarmPanelCardConfig::getStates() {
    return &(this->states);
}

#endif // __ALARMPANELCARDCONFIG_H__