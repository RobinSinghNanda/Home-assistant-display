#include "CalendarCardConfig.hpp"

CalendarCardConfig::CalendarCardConfig(std::vector<string> entities) : CalendarCardConfig(entities, "", "", "") {

}

CalendarCardConfig::CalendarCardConfig(std::vector<string> entities, const char * initialView, const char * title, const char * icon) :
    BaseCardConfig(TYPE, title, icon) {
        this->entities = entities;
        this->initialView = initialView;
}