#include "Camera.hpp"
using namespace HomeAssistant;

Camera::Camera (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_VIDEO;
}

Camera::~Camera() {

}