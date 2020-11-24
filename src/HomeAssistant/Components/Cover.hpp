#ifndef __COVER_H__
#define __COVER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Cover : public Entity {
        public:
            Cover(const char * entity);
            virtual ~Cover();
            static constexpr const char * ENTITY_DOMAIN = "cover";

            //States
            static constexpr State STATE_CLOSED = "closed";
            static constexpr State STATE_OPEN = "open";
            static constexpr State STATE_OPENING = "opening";
            static constexpr State STATE_CLOSING = "closing";

            //Device classes
            static constexpr DeviceClassName DEVICE_CLASS_AWNING = "awning";
            static constexpr DeviceClassName DEVICE_CLASS_BLIND = "blind";
            static constexpr DeviceClassName DEVICE_CLASS_CURTAIN = "curtain";
            static constexpr DeviceClassName DEVICE_CLASS_DAMPER = "damper";
            static constexpr DeviceClassName DEVICE_CLASS_DOOR = "door";
            static constexpr DeviceClassName DEVICE_CLASS_GARAGE = "garage";
            static constexpr DeviceClassName DEVICE_CLASS_GATE = "gate";
            static constexpr DeviceClassName DEVICE_CLASS_SHADE = "shade";
            static constexpr DeviceClassName DEVICE_CLASS_SHUTTER = "shutter";
            static constexpr DeviceClassName DEVICE_CLASS_WINDOW = "window";

            //Features
            static constexpr Feature SUPPORT_OPEN = 1;
            static constexpr Feature SUPPORT_CLOSE = 2;
            static constexpr Feature SUPPORT_SET_POSITION = 4;
            static constexpr Feature SUPPORT_STOP = 8;
            static constexpr Feature SUPPORT_OPEN_TILT = 16;
            static constexpr Feature SUPPORT_CLOSE_TILT = 32;
            static constexpr Feature SUPPORT_STOP_TILT = 64;
            static constexpr Feature SUPPORT_SET_TILT_POSITION = 128;

            //Services
            static constexpr ServiceName SERVICE_CLOSE_COVER = "close_cover";
            static constexpr ServiceName SERVICE_CLOSE_COVER_TILT = "close_cover_tilt";
            static constexpr ServiceName SERVICE_OPEN_COVER = "open_cover";
            static constexpr ServiceName SERVICE_OPEN_COVER_TILT = "open_cover_tilt";
            static constexpr ServiceName SERVICE_SET_COVER_POSITION = "set_cover_position";
            static constexpr ServiceName SERVICE_SET_COVER_TILT_POSITION = "set_cover_tilt_position";
            static constexpr ServiceName SERVICE_STOP_COVER = "stop_cover";
            static constexpr ServiceName SERVICE_STOP_COVER_TILT = "stop_cover_tilt";
            static constexpr ServiceName SERVICE_TOGGLE_COVER_TILT = "toggle_cover_tilt";

            //Attributes
            static constexpr AttributeName ATTR_CURRENT_POSITION = "current_position";
            static constexpr AttributeName ATTR_CURRENT_TILT_POSITION = "current_tilt_position";
            static constexpr AttributeName ATTR_POSITION = "position";
            static constexpr AttributeName ATTR_TILT_POSITION = "tilt_position";

            //API
            virtual void updateAttributes(JsonObject stateAttributesObject);
        
        protected:
            uint32_t position;
            uint32_t tiltPosition;
            virtual void setIcon();
    };
};

#endif // __COVER_H__