#ifndef __DEVICETRACKER_H__
#define __DEVICETRACKER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class DeviceTracker : public Entity {
        public:
            typedef const char * SourceType;
            DeviceTracker(const char * entity);
            virtual ~DeviceTracker();
            static constexpr const char * ENTITY_DOMAIN = "device_tracker";

            //States
            static constexpr State STATE_HOME = "home";
            static constexpr State STATE_NOT_HOME = "not_home";

            //Attributes
            static constexpr AttributeName ATTR_ATTRIBUTES = "attributes";
            static constexpr AttributeName ATTR_BATTERY = "battery";
            static constexpr AttributeName ATTR_DEV_ID = "dev_id";
            static constexpr AttributeName ATTR_GPS = "gps";
            static constexpr AttributeName ATTR_HOST_NAME = "host_name";
            static constexpr AttributeName ATTR_LOCATION_NAME = "location_name";
            static constexpr AttributeName ATTR_MAC = "mac";
            static constexpr AttributeName ATTR_SOURCE_TYPE = "source_type";
            static constexpr AttributeName ATTR_CONSIDER_HOME = "consider_home";

            //Source type
            static constexpr SourceType SOURCE_TYPE_GPS = "gps";
            static constexpr SourceType SOURCE_TYPE_ROUTER = "router";
            static constexpr SourceType SOURCE_TYPE_BLUETOOTH = "bluetooth";
            static constexpr SourceType SOURCE_TYPE_BLUETOOTH_LE = "bluetooth_le";


            //API
            inline bool isSourceType(SourceType type);

        protected:
            string sourceType = "";
            string locationName = "";
            float latitude = 0.0;
            float longitude = 0.0;
            float gpsAccuracy = 0.0;
            float altitude = 0.0;
            float course = 0.0;
            float speed = 0.0;
            float verticalAccuracy = 0.0;

    };

    bool DeviceTracker::isSourceType(SourceType type) {
        return sourceType.compare(type) == 0;
    }
};



#endif // __DEVICETRACKER_H__