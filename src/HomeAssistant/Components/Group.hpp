#ifndef __GROUP_H__
#define __GROUP_H__

#include "Entity.hpp"

namespace HomeAssistant {
    class Group : public Entity {
        public:
            Group(const char * entity);
            virtual ~Group();
            static constexpr const char * ENTITY_DOMAIN = "group";
            
            //Services
            static constexpr ServiceName SERVICE_SET = "set";
            static constexpr ServiceName SERVICE_REMOVE = "remove";

            //Attributes
            static constexpr AttributeName ATTR_ADD_ENTITIES = "add_entities";
            static constexpr AttributeName ATTR_AUTO = "auto";
            static constexpr AttributeName ATTR_ENTITIES = "entities";
            static constexpr AttributeName ATTR_OBJECT_ID = "object_id";
            static constexpr AttributeName ATTR_ORDER = "order";
            static constexpr AttributeName ATTR_ALL = "all";
    };
};

#endif // __GROUP_H__