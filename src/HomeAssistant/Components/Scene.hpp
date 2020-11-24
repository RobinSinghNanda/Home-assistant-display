#ifndef __SCENE_H__
#define __SCENE_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Scene : public Entity {
        public:
            Scene(const char * entity);
            virtual ~Scene();
            static constexpr const char * ENTITY_DOMAIN = "scene";

            //Attributes
            static constexpr AttributeName ATTR_TRANSITION = "transition";
    };
};

#endif // __SCENE_H__