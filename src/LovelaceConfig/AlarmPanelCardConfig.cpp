#include "AlarmPanelCardConfig.hpp"

AlarmPanelCardConfig::AlarmPanelCardConfig(const char * entity) :
    AlarmPanelCardConfig(entity, "", nullptr) {
}

AlarmPanelCardConfig::AlarmPanelCardConfig(const char * entity, const char * name, std::list<string> * states) :
    BaseCardConfig(TYPE) {
    this->entity = entity;
    this->name = name;
    if (states != nullptr) {
        for (string state : *states) {
            this->states.push_back(state);
        }
    }
}