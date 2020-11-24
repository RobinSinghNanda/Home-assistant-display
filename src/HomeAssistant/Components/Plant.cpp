#include "Plant.hpp"
using namespace HomeAssistant;

Plant::Plant (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_FLOWER;
}

Plant::~Plant() {

}