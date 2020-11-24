#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "HomeAssistant/HomeAssistantClient.hpp"
#include "HomeAssistant/ServiceData.hpp"
#include "Icons.hpp"
#include <stdint.h>
#include <string>
#include <ArduinoJson.h>
#include "../StateParser.hpp"
using namespace std;

#define setAndUpdate(x, y) if(x!=y){x=y;updated=true;}else{return;}
namespace HomeAssistant {
    typedef const char * AttributeName;
    typedef const char * ServiceName;
    typedef const char * DeviceClassName;
    typedef const char * State;
    typedef const char * Icon;
    typedef uint32_t Feature;
    
    class Entity {
        public:
            Entity(const char * entity);
            Entity(const char * entity, const char * domain);
            virtual ~Entity();
            static constexpr const char * ENTITY_DOMAIN = "";

            //States
            static constexpr State STATE = "state";
            static constexpr State ATTRIBUTES = "attributes";
            static constexpr State STATE_UNAVAILABLE = "unavailable";
            static constexpr State STATE_ON = "on";
            static constexpr State STATE_OFF = "off";
            static constexpr State STATE_UNKNOWN = "unknown";

            //Services
            static constexpr ServiceName SERVICE_TURN_ON = "turn_on";
            static constexpr ServiceName SERVICE_TURN_OFF = "turn_off";
            static constexpr ServiceName SERVICE_TOGGLE = "toggle";
            static constexpr ServiceName SERVICE_RELOAD = "reload";

            //Attributes
            static constexpr AttributeName ATTR_SUPPORTED_FEATURES = "supported_features";
            static constexpr AttributeName ATTR_FRIENDLY_NAME = "friendly_name";
            static constexpr AttributeName ATTR_ICON = "icon";
            static constexpr AttributeName ATTR_DEVICE_CLASS = "device_class";
            static constexpr AttributeName ATTR_ENTITY_ID = "entity_id";

            //Constants
            static constexpr const char * ENTITY_ID = "entity_id";
            static constexpr const char * S_TRUE = "true";
            static constexpr const char * S_FALSE = "false";

            //API
            operator const char* () const;
            inline const char * getEntityId();
            inline bool isAvailable();
            inline bool isUnavailable();
            inline bool isOff();
            inline bool isOn();
            inline bool isFeatureSupported(uint32_t feature);
            inline bool isState(const char * state);
            template <class EntityType>
            inline bool is(const char * entity);
            inline bool is(const char * entity);
            inline void setState(const char * state);
            inline void setState(bool state);
            inline const char * getState();
            inline const char * getDomain();
            template <class EntityType>
            inline bool is();
            inline static string getDomain(string entity);
            inline const char * getFriendlyName();
            inline const char * getIcon();
            inline const char * getDeviceClass();
            void updateState(JsonObject stateObject);
            virtual void updateAttributes(JsonObject stateAttributesObject);
            virtual void callService(HomeAssistantClient * client, const char * service);
        protected:
            string state = "";
            string domain = "";
            string entityId = "";
            string deviceClass = "";
            string friendlyName = "";
            string icon = "";
            uint32_t supportedFeatures = 0;
            virtual void setIcon();
            void callEntityService(HomeAssistantClient * client, const char * service);
    };

    const char * Entity::getEntityId() {
        return this->entityId.c_str();
    }

    bool Entity::isFeatureSupported(uint32_t feature) {
        return this->supportedFeatures&feature;
    }

    bool Entity::isState(const char * state) {
        return this->state.compare(state) == 0;
    }

    bool Entity::isAvailable() {
        return !isState(Entity::STATE_UNAVAILABLE);
    }

    bool Entity::isUnavailable() {
        return isState(Entity::STATE_UNAVAILABLE);
    }

    bool Entity::isOff() {
        return isState(Entity::STATE_OFF);
    }

    bool Entity::isOn() {
        return isState(Entity::STATE_ON);
    }

    template <class EntityType>
    bool Entity::is(const char * entity) {
        return (this->entityId.compare(entity) == 0) && is<EntityType>();
    }

    bool Entity::is(const char * entity) {
        return this->entityId.compare(entity) == 0;
    }

    const char * Entity::getDomain() {
        return domain.c_str();
    }

    void Entity::setState(const char * state) {
        if (this->state != state) {
            this->state = state;
            this->setIcon();
        }
    }

    void Entity::setState(bool state) {
        if (state) {
            setState(Entity::STATE_ON);
        } else {
            setState(Entity::STATE_OFF);
        }
    }

    const char * Entity::getState() {
        return this->state.c_str();
    }

    string Entity::getDomain(string entity) {
        size_t pos;
        if ((pos=entity.find('.')) != entity.npos) {
            return entity.substr(0, pos);
        }
        return "";
    }

    const char * Entity::getFriendlyName() {
        return this->friendlyName.c_str();
    }

    const char * Entity::getIcon() {
        return this->icon.c_str();
    }

    template <class EntityType>
    bool Entity::is() {
        return this->domain.compare(EntityType::ENTITY_DOMAIN) == 0;
    }

    const char * Entity::getDeviceClass() {
        return this->deviceClass.c_str();
    }
    
};

#endif // __ENTITY_H__