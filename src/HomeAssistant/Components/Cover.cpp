#include "Cover.hpp"
using namespace HomeAssistant;

Cover::Cover (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_WINDOW_OPEN;
}

Cover::~Cover() {

}

void Cover::setIcon() {
    bool open = (state != STATE_CLOSED);
    if (this->deviceClass == DEVICE_CLASS_GARAGE) {
        if (this->state == STATE_OPENING) {
            this->icon = ICON_ARROW_UP_BOX;
        } else if (this->state == STATE_CLOSING) {
            this->icon = ICON_ARROW_DOWN_BOX;
        } else if (this->state == STATE_CLOSED) {
            this->icon = ICON_GARAGE;
        } else {
            this->icon = ICON_GARAGE_OPEN;
        }
    } else if (this->deviceClass == DEVICE_CLASS_GATE) {
        if (this->state == STATE_OPENING ||
            this->state == STATE_CLOSING) {
            this->icon = ICON_GATE_ARROW_RIGHT;
        } else if (this->state == STATE_CLOSED) {
            this->icon = ICON_GATE;
        } else {
            this->icon = ICON_GATE_OPEN;
        }
    } else if (this->deviceClass == DEVICE_CLASS_DOOR) {
        this->icon = (open)?ICON_DOOR_OPEN:ICON_DOOR_CLOSED;
    } else if (this->deviceClass == DEVICE_CLASS_DAMPER) {
        this->icon = (open)?ICON_CIRCLE:ICON_CIRCLE_SLICE_8;
    } else if (this->deviceClass == DEVICE_CLASS_SHUTTER) {
        if (this->state == STATE_OPENING) {
            this->icon = ICON_ARROW_UP_BOX;
        } else if (this->state == STATE_CLOSING) {
            this->icon = ICON_ARROW_DOWN_BOX;
        } else if (this->state == STATE_CLOSED) {
            this->icon = ICON_WINDOW_SHUTTER;
        } else {
            this->icon = ICON_WINDOW_SHUTTER_OPEN;
        }
    } else if (this->deviceClass == DEVICE_CLASS_BLIND ||
        this->deviceClass == DEVICE_CLASS_CURTAIN) {
        if (this->state == STATE_OPENING) {
            this->icon = ICON_ARROW_UP_BOX;
        } else if (this->state == STATE_CLOSING) {
            this->icon = ICON_ARROW_DOWN_BOX;
        } else if (this->state == STATE_CLOSED) {
            this->icon = ICON_BLINDS;
        } else {
            this->icon = ICON_BLINDS_OPEN;
        }
    } else if (this->deviceClass == DEVICE_CLASS_WINDOW) {
        if (this->state == STATE_OPENING) {
            this->icon = ICON_ARROW_UP_BOX;
        } else if (this->state == STATE_CLOSING) {
            this->icon = ICON_ARROW_DOWN_BOX;
        } else if (this->state == STATE_CLOSED) {
            this->icon = ICON_WINDOW_CLOSED;
        } else {
            this->icon = ICON_WINDOW_OPEN;
        }
    } else {
        if (this->state == STATE_OPENING) {
            this->icon = ICON_ARROW_UP_BOX;
        } else if (this->state == STATE_CLOSING) {
            this->icon = ICON_ARROW_DOWN_BOX;
        } else if (this->state == STATE_CLOSED) {
            this->icon = ICON_WINDOW_CLOSED;
        } else {
            this->icon = ICON_WINDOW_OPEN;
        }
    }
}

void Cover::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_CURRENT_POSITION, position);
    StateParser::parse(stateAttributesObject, ATTR_CURRENT_TILT_POSITION, tiltPosition);
}