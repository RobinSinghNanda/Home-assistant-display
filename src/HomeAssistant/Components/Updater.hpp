#ifndef __UPDATER_H__
#define __UPDATER_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Updater : public Entity {
        public:
            Updater(const char * entity);
            virtual ~Updater();
            static constexpr const char * ENTITY_DOMAIN = "updater";
    };
};

#endif // __UPDATER_H__