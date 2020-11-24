#ifndef __MEDIAPLAYER_H__
#define __MEDIAPLAYER_H__

#include "Entity.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
namespace HomeAssistant {
    class MediaPlayer : public Entity{
        public:
            MediaPlayer (const char * entity);
            virtual ~MediaPlayer();
            //domain name
            static constexpr const char * ENTITY_DOMAIN = "media_player";

            //Features
            static constexpr Feature SUPPORT_PAUSE = 1;
            static constexpr Feature SUPPORT_SEEK = 2;
            static constexpr Feature SUPPORT_VOLUME_SET = 4;
            static constexpr Feature SUPPORT_VOLUME_MUTE = 8;
            static constexpr Feature SUPPORT_PREVIOUS_TRACK = 16;
            static constexpr Feature SUPPORT_NEXT_TRACK = 32;
            static constexpr Feature SUPPORT_TURN_ON = 128;
            static constexpr Feature SUPPORT_TURN_OFF = 256;
            static constexpr Feature SUPPORT_PLAY_MEDIA = 512;
            static constexpr Feature SUPPORT_VOLUME_STEP = 1024;
            static constexpr Feature SUPPORT_SELECT_SOURCE = 2048;
            static constexpr Feature SUPPORT_STOP = 4096;
            static constexpr Feature SUPPORT_CLEAR_PLAYLIST = 8192;
            static constexpr Feature SUPPORT_PLAY = 16384;
            static constexpr Feature SUPPORT_SHUFFLE_SET = 32768;
            static constexpr Feature SUPPORT_SELECT_SOUND_MODE = 65536;
            static constexpr Feature SUPPORT_BROWSE_MEDIA = 131072;
            static constexpr Feature SUPPORT_REPEAT_SET = 262144;

            //Services
            static constexpr ServiceName SERVICE_TOGGLE = "toggle";
            static constexpr ServiceName SERVICE_VOLUME_UP = "volume_up";
            static constexpr ServiceName SERVICE_VOLUME_DOWN = "volume_down";
            static constexpr ServiceName SERVICE_VOLUME_SET = "volume_set";
            static constexpr ServiceName SERVICE_VOLUME_MUTE = "volume_mute";
            static constexpr ServiceName SERVICE_MEDIA_PLAY_PAUSE = "media_play_pause";
            static constexpr ServiceName SERVICE_MEDIA_PLAY = "media_play";
            static constexpr ServiceName SERVICE_MEDIA_PAUSE = "media_pause";
            static constexpr ServiceName SERVICE_MEDIA_STOP = "media_stop";
            static constexpr ServiceName SERVICE_MEDIA_NEXT_TRACK = "media_next_track";
            static constexpr ServiceName SERVICE_MEDIA_PREVIOUS_TRACK = "media_previous_track";
            static constexpr ServiceName SERVICE_CLEAR_PLAYLIST = "clear_playlist";
            static constexpr ServiceName SERVICE_SUFFLE_SET = "shuffle_set";
            static constexpr ServiceName SERVICE_REPEAT_SET = "repeat_set";
            static constexpr ServiceName SERVICE_PLAY_MEDIA = "play_media";
            static constexpr ServiceName SERVICE_SELECT_SOURCE = "select_source";
            static constexpr ServiceName SERVICE_SELECT_SOUND_MODE = "select_sound_mode";
            static constexpr ServiceName SERVICE_MEDIA_SEEK = "media_seek";

            //Attributes
            static constexpr AttributeName ATTR_APP_ID = "app_id";
            static constexpr AttributeName ATTR_APP_NAME = "app_name";
            static constexpr AttributeName ATTR_INPUT_SOURCE = "source";
            static constexpr AttributeName ATTR_INPUT_SOURCE_LIST = "source_list";
            static constexpr AttributeName ATTR_MEDIA_ALBUM_ARTIST = "media_album_artist";
            static constexpr AttributeName ATTR_MEDIA_ALBUM_NAME = "media_album_name";
            static constexpr AttributeName ATTR_MEDIA_ARTIST = "media_artist";
            static constexpr AttributeName ATTR_MEDIA_CHANNEL = "media_channel";
            static constexpr AttributeName ATTR_MEDIA_CONTENT_ID = "media_content_id";
            static constexpr AttributeName ATTR_MEDIA_CONTENT_TYPE = "media_content_type";
            static constexpr AttributeName ATTR_MEDIA_DURATION = "media_duration";
            static constexpr AttributeName ATTR_MEDIA_ENQUEUE = "enqueue";
            static constexpr AttributeName ATTR_MEDIA_EXTRA = "extra";
            static constexpr AttributeName ATTR_MEDIA_EPISODE = "media_episode";
            static constexpr AttributeName ATTR_MEDIA_PLAYLIST = "media_playlist";
            static constexpr AttributeName ATTR_MEDIA_POSITION = "media_position";
            static constexpr AttributeName ATTR_MEDIA_POSITION_UPDATED_AT = "media_position_updated_at";
            static constexpr AttributeName ATTR_MEDIA_REPEAT = "repeat";
            static constexpr AttributeName ATTR_MEDIA_SEASON = "media_season";
            static constexpr AttributeName ATTR_MEDIA_SEEK_POSITION = "seek_position";
            static constexpr AttributeName ATTR_MEDIA_SERIES_TITLE = "media_series_title";
            static constexpr AttributeName ATTR_MEDIA_SHUFFLE = "shuffle";
            static constexpr AttributeName ATTR_MEDIA_TITLE = "media_title";
            static constexpr AttributeName ATTR_MEDIA_TRACK = "media_track";
            static constexpr AttributeName ATTR_MEDIA_VOLUME_LEVEL = "volume_level";
            static constexpr AttributeName ATTR_MEDIA_VOLUME_MUTED = "is_volume_muted";
            static constexpr AttributeName ATTR_SOUND_MODE = "sound_mode";
            static constexpr AttributeName ATTR_SOUND_MODE_LIST = "sound_mode_list";

            static constexpr State STATE_PLAYING = "playing";
            static constexpr State STATE_PAUSED = "paused";

            inline bool isPlaying();
            inline bool isPaused();
            inline void setVolumeLevel(float volumeLevel);
            inline float getVolumeLevel();
            inline void setMediaPosition(float position);
            inline float getMediaPosition();
            inline float getMediaPosition(uint32_t time);
            inline float getMediaDuration();
            inline const char * getMediaTitle();
            inline void setVolumeMute(bool mute);
            inline bool isVolumeMuted();
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            bool play = false;
            string mediaTitle = "";
            float volumeLevel = 1.0;
            float mediaPosition = 0.0;
            float mediaDuration = 0.0;
            uint32_t updatedAt = 0;
            bool volumeMuted = false;
            virtual void setIcon();
    };

    bool MediaPlayer::isPlaying() {
        return isState(STATE_PLAYING);
    }

    bool MediaPlayer::isPaused() {
        return isState(STATE_PAUSED);
    }

    void MediaPlayer::setVolumeLevel(float volumeLevel){
        this->volumeLevel = volumeLevel;
    }

    float MediaPlayer::getVolumeLevel() {
        return this->volumeLevel;
    }

    void MediaPlayer::setMediaPosition(float position) {
        this->mediaPosition = position;
    }

    float MediaPlayer::getMediaPosition() {
        return this->mediaPosition;
    }

    float MediaPlayer::getMediaPosition(uint32_t time) {
        if (isPlaying()) {
            int32_t diff = time - this->updatedAt;
            return this->mediaPosition + diff;
        } else {
            return this->mediaPosition;
        }
    }

    float MediaPlayer::getMediaDuration() {
        return this->mediaDuration;
    }

    const char * MediaPlayer::getMediaTitle() {
        return this->mediaTitle.c_str();
    }

    void MediaPlayer::setVolumeMute(bool mute) {
        this->volumeMuted = mute;
    }

    bool MediaPlayer::isVolumeMuted() {
        return volumeMuted;
    }
};

#endif // __MEDIAPLAYER_H__