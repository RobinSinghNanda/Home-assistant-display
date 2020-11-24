#ifndef __CALENDARCARDCONFIG_H__
#define __CALENDARCARDCONFIG_H__

#include <vector>
#include "BaseCardConfig.hpp"

class CalendarCardConfig : public BaseCardConfig {
    public:
        CalendarCardConfig(std::vector<string> entities);
        CalendarCardConfig(std::vector<string> entities, const char * initialView, const char * title, const char * icon);
        static constexpr const char * TYPE = "calendar";
        inline uint8_t getNumEntities();
        inline const char * getEntityId(uint8_t index);
        inline const char * getInitialView();
    protected:
        std::vector<string> entities;
        string initialView;
};

uint8_t CalendarCardConfig::getNumEntities() {
    return entities.size();
}

const char * CalendarCardConfig::getEntityId(uint8_t index) {
    if (index < entities.size()) {
        return entities[index].c_str();
    } else {
        return "";
    }
}

const char * CalendarCardConfig::getInitialView() {
    return this->initialView.c_str();
}

#endif // __CALENDARCARDCONFIG_H__