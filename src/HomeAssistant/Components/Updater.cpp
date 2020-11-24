#include "Updater.hpp"
using namespace HomeAssistant;

Updater::Updater (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_CLOUD_UPLOAD;
}

Updater::~Updater() {

}