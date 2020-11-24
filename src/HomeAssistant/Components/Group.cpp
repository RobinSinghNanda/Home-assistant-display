#include "Group.hpp"
using namespace HomeAssistant;

Group::Group (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_GOOGLE_CIRCLES_COMMUNITIES;
}

Group::~Group() {

}