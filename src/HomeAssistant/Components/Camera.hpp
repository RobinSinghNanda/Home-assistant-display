#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Camera : public Entity {
        public:
            Camera(const char * entity);
            virtual ~Camera();
            static constexpr const char * ENTITY_DOMAIN = "camera";

            //States
            static constexpr State STATE_RECORDING = "recording";
            static constexpr State STATE_STREAMING = "streaming";
            static constexpr State STATE_IDLE = "idle";

            //Features
            static constexpr Feature SUPPORT_ON_OFF = 1;
            static constexpr Feature SUPPORT_STREAM = 2;

            //Attributes
            static constexpr AttributeName ATTR_MEDIA_CONTENT_ID = "media_content_id";
            static constexpr AttributeName ATTR_MEDIA_CONTENT_TYPE = "media_content_type";
            static constexpr AttributeName ATTR_MEDIA_EXTRA = "extra";
            static constexpr AttributeName ATTR_FILENAME = "filename";
            static constexpr AttributeName ATTR_MEDIA_PLAYER = "media_player";
            static constexpr AttributeName ATTR_FORMAT = "format";

            //Services
            static constexpr ServiceName SERVICE_PLAY_MEDIA = "play_media";
            static constexpr ServiceName SERVICE_RECORD = "record";
            static constexpr ServiceName SERVICE_ENABLE_MOTION = "enable_motion_detection";
            static constexpr ServiceName SERVICE_DISABLE_MOTION = "disable_motion_detection";
            static constexpr ServiceName SERVICE_SNAPSHOT = "snapshot";
            static constexpr ServiceName SERVICE_PLAY_STREAM = "play_stream";
    };
};


#endif // __CAMERA_H__