#include "MediaPlayer.hpp"
#include "time.h"
#include "RtcTime.hpp"
using namespace HomeAssistant;

MediaPlayer::MediaPlayer (const char * entity) : Entity(entity, ENTITY_DOMAIN) {
    icon = ICON_CAST;
}

MediaPlayer::~MediaPlayer() {

}

void MediaPlayer::updateAttributes(JsonObject stateAttributesObject) {
    Entity::updateAttributes(stateAttributesObject);
    StateParser::parse(stateAttributesObject, ATTR_MEDIA_VOLUME_LEVEL, volumeLevel);
    StateParser::parse(stateAttributesObject, ATTR_MEDIA_VOLUME_MUTED, volumeMuted);
    StateParser::parse(stateAttributesObject, ATTR_MEDIA_DURATION, volumeLevel);
    StateParser::parse(stateAttributesObject, ATTR_MEDIA_POSITION, mediaPosition);
    StateParser::parse(stateAttributesObject, ATTR_MEDIA_TITLE, mediaTitle);
    StateParser::parseTime(stateAttributesObject, ATTR_MEDIA_POSITION_UPDATED_AT, updatedAt);
}

void MediaPlayer::setIcon() {
    this->icon = (this->state == STATE_PLAYING)?ICON_CAST_CONNECTED:ICON_CAST;
}

void MediaPlayer::callService(HomeAssistantClient * client, const char * service) {
    serviceData->clear();
    if (strcmp(service, SERVICE_VOLUME_SET) == 0) {
        serviceData->add(ATTR_MEDIA_VOLUME_LEVEL, volumeLevel);
    } else if (strcmp(service, SERVICE_MEDIA_SEEK) == 0) {
        serviceData->add(ATTR_MEDIA_SEEK_POSITION, mediaPosition);
    } else if (strcmp(service, SERVICE_VOLUME_MUTE) == 0) {
        serviceData->add(ATTR_MEDIA_VOLUME_MUTED, volumeMuted);
    }
    callEntityService(client, service);
}