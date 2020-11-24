#include "CardTypes.hpp"
#include "MediaControlCardConfig.hpp"
#include <Arduino.h>

MediaControlCardConfig::MediaControlCardConfig(const char * entity) :
    MediaControlCardConfig(entity, "", "") {
}

MediaControlCardConfig::MediaControlCardConfig(const char * entity, const char * title, const char * icon) :
    MediaControlCardConfig(entity, title, icon, icon) {
}

MediaControlCardConfig::MediaControlCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon) :
    BaseEntityCardConfig(TYPE, entity, title, title, icon, rowIcon, 1) {

} 

MediaControlCardConfig::~MediaControlCardConfig() {

}