#ifndef __MEDIAPLAYERCARDCONFIG_H__
#define __MEDIAPLAYERCARDCONFIG_H__

#include "BaseEntityCardConfig.hpp"

class MediaControlCardConfig : public BaseEntityCardConfig {
    public: 
        MediaControlCardConfig(const char * entity);
        MediaControlCardConfig(const char * entity, const char * title, const char * icon);
        MediaControlCardConfig(const char * entity, const char * title, const char * icon, const char * rowIcon);
        ~MediaControlCardConfig();
        static constexpr const char * TYPE = "media-control";
};

#endif // __MEDIAPLAYERCARDCONFIG_H__