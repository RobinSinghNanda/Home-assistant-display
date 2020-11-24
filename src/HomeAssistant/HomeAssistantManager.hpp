#ifndef __HOMEASSISTANTMANAGER_H__
#define __HOMEASSISTANTMANAGER_H__

#include "Components/Components.hpp"
#include "list"
using namespace std;

namespace HomeAssistant {
    class HomeAssistantManager {
        public:
            HomeAssistantManager(HomeAssistantClient * client);
            ~HomeAssistantManager();
            template <class EntityType>
            EntityType * getEntity(const char * entity);
            Entity * getEntity(const char * entity);
            template <class EntityType>
            EntityType * getEntity(uint16_t index);
            Entity * getEntity(uint16_t index);
            template <class EntityType>
            inline bool entityExists(const char * entity);
            inline bool entityExists(const char * entity);
            inline uint16_t getNumEntities();
            void addEntity(const char * entity);
            template <class EntityType>
            void removeEntity(const char * entity);
            void removeEntity(const char * entity);
            void poll();
            void callService(const char * entity_id, const char * service);
            inline void callService(Entity * entity, const char * service);
            void callStateService(const char * entity_id);
            inline void callStateService(Entity * entity);
            inline void subscribe();
            inline void subscribe(Entity * entity);
            inline void subscribe(const char * entityId);
            inline void sync();
            inline void sync(Entity * entity);
            inline void sync(const char * entityId);
        protected:
            list<Entity*> entities;
            HomeAssistantClient * client;
    };

    uint16_t HomeAssistantManager::getNumEntities() {
        return this->entities.size();
    }

    template <class EntityType>
    bool HomeAssistantManager::entityExists(const char * entity) {
        return getEntity<EntityType>(entity) != nullptr;
    }

    bool HomeAssistantManager::entityExists(const char * entity) {
        return getEntity(entity) != nullptr;
    }

    void HomeAssistantManager::callService(Entity * entity, const char * service) {
        entity->callService(this->client, service);
    }

    void HomeAssistantManager::callStateService(Entity * entity) {
        if (entity->isOn())
            entity->callService(this->client, Entity::SERVICE_TURN_ON);
        else
            entity->callService(this->client, Entity::SERVICE_TURN_OFF);
    }

    void HomeAssistantManager::subscribe(Entity * entity) {
        client->subscribe(entity->getEntityId());
    }

    void HomeAssistantManager::subscribe(const char * entityId) {
        client->subscribe(entityId);
    }

    void HomeAssistantManager::subscribe() {
        for (auto entity: entities) {
            this->subscribe(entity->getEntityId());
        }
    }

    void HomeAssistantManager::sync(Entity * entity) {
        client->sync(entity->getEntityId());
    }

    void HomeAssistantManager::sync(const char * entityId) {
        client->sync(entityId);
    }

    void HomeAssistantManager::sync() {
        for (auto entity: entities) {
            this->sync(entity->getEntityId());
        }
    }

    template <class EntityType>
    EntityType* HomeAssistantManager::getEntity(const char * entityId) {
        for (Entity * entity : entities) {
            if (entity->is<EntityType>(entityId)) {
                return (EntityType*) entity;
            }
        }
        return nullptr;
    }


    template <class EntityType>
    EntityType* HomeAssistantManager::getEntity(uint16_t index) {
        if (index<entities.size()) {
            Entity * entity = *(std::next(entities.begin(), index));
            if (entity->is<EntityType>()) {
                return (EntityType*) entity;
            }
            return nullptr;
        } else {
            return nullptr;
        }
    }

    template <class EntityType>
    void HomeAssistantManager::removeEntity(const char * entityId) {
        Entity * entity;
        if ((entity=getEntity<EntityType>(entityId))!=nullptr) {
            this->entities.remove(entity);
            delete entity;
        }
    }
};

#endif // __HOMEASSISTANTMANAGER_H__