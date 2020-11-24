#ifndef __PERSON_H__
#define __PERSON_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Person : public Entity {
        public:
            Person(const char * entity);
            virtual ~Person();
            static constexpr const char * ENTITY_DOMAIN = "person";

            //Attributes 
            static constexpr AttributeName ATTR_GPS_ACCURACY = "gps_accuracy";
            static constexpr AttributeName ATTR_LATITUDE = "latitude";
            static constexpr AttributeName ATTR_LONGITUDE = "longitude";
    };
};

#endif // __PERSON_H__