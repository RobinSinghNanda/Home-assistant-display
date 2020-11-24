#include "Scene.hpp"
using namespace HomeAssistant;

Scene::Scene(const char * entity) :Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_PALETTE;
}

Scene::~Scene() {

}