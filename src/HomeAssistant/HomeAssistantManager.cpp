#include "HomeAssistantManager.hpp"
#include "Components/Components.hpp"
#include "Log.hpp"
using namespace HomeAssistant;

HomeAssistantManager::HomeAssistantManager(HomeAssistantClient * client) {
    this->client = client; 
    client->onStateChange([this](HomeAssistantClient*, const char * entityId, JsonObject stateObject){
            Entity * entity = this->getEntity(entityId);
            if (entity) {
                entity->updateState(stateObject);
            }
        });
}

HomeAssistantManager::~HomeAssistantManager() {
    for(auto entity : entities) {
        delete entity;
    }
}

void HomeAssistantManager::addEntity(const char * entityId) {
    if (!entityExists(entityId)) {
        Entity * entity = nullptr;
        string domain = Entity::getDomain(entityId);
        if (domain == Switch::ENTITY_DOMAIN) {
            entity = new Switch(entityId);
        } else if (domain == Light::ENTITY_DOMAIN) {
            entity = new Light(entityId);
        } else if (domain == Fan::ENTITY_DOMAIN) {
            entity = new Fan(entityId);
        } else if (domain == InputBoolean::ENTITY_DOMAIN) {
            entity = new InputBoolean(entityId);
        } else if (domain == InputDateTime::ENTITY_DOMAIN) {
            entity = new InputDateTime(entityId);
        } else if (domain == InputNumber::ENTITY_DOMAIN) {
            entity = new InputNumber(entityId);
        } else if (domain == InputSelect::ENTITY_DOMAIN) {
            entity = new InputSelect(entityId);
        } else if (domain == MediaPlayer::ENTITY_DOMAIN) {
            entity = new MediaPlayer(entityId);
        } else if (domain == Sensor::ENTITY_DOMAIN) {
            entity = new Sensor(entityId);
        } else if (domain == BinarySensor::ENTITY_DOMAIN) {
            entity = new BinarySensor(entityId);
        } else if (domain == Climate::ENTITY_DOMAIN) {
            entity = new Climate(entityId);
        } else if (domain == Cover::ENTITY_DOMAIN) {
            entity = new Cover(entityId);
        } else if (domain == Group::ENTITY_DOMAIN) {
            entity = new Group(entityId);
        } else if (domain == Scene::ENTITY_DOMAIN) {
            entity = new Scene(entityId);
        } else if (domain == Script::ENTITY_DOMAIN) {
            entity = new Script(entityId);
        } else if (domain == Automation::ENTITY_DOMAIN) {
            entity = new Automation(entityId);
        } else if (domain == DeviceTracker::ENTITY_DOMAIN) {
            entity = new DeviceTracker(entityId);
        } else if (domain == Person::ENTITY_DOMAIN) {
            entity = new Person(entityId);
        } else if (domain == Remote::ENTITY_DOMAIN) {
            entity = new Remote(entityId);
        } else if (domain == AlarmControlPanel::ENTITY_DOMAIN) {
            entity = new AlarmControlPanel(entityId);
        } else if (domain == Alert::ENTITY_DOMAIN) {
            entity = new Alert(entityId);
        } else if (domain == Counter::ENTITY_DOMAIN) {
            entity = new Counter(entityId);
        } else if (domain == Lock::ENTITY_DOMAIN) {
            entity = new Lock(entityId);
        } else if (domain == Timer::ENTITY_DOMAIN) {
            entity = new Timer(entityId);
        } else if (domain == Vacuum::ENTITY_DOMAIN) {
            entity = new Vacuum(entityId);
        } else if (domain == WaterHeater::ENTITY_DOMAIN) {
            entity = new WaterHeater(entityId);
        } else if (domain == Camera::ENTITY_DOMAIN) {
            entity = new Camera(entityId);
        } else if (domain == AirQuality::ENTITY_DOMAIN) {
            entity = new AirQuality(entityId);
        } else if (domain == Calendar::ENTITY_DOMAIN) {
            entity = new Calendar(entityId);
        } else if (domain == Humidifier::ENTITY_DOMAIN) {
            entity = new Humidifier(entityId);
        } else if (domain == InputText::ENTITY_DOMAIN) {
            entity = new InputText(entityId);
        } else if (domain == Plant::ENTITY_DOMAIN) {
            entity = new Plant(entityId);
        } else if (domain == Proximity::ENTITY_DOMAIN) {
            entity = new Proximity(entityId);
        } else if (domain == Sun::ENTITY_DOMAIN) {
            entity = new Sun(entityId);
        } else if (domain == Updater::ENTITY_DOMAIN) {
            entity = new Updater(entityId);
        } else if (domain == Weather::ENTITY_DOMAIN) {
            entity = new Weather(entityId);
        } else if (domain == Zone::ENTITY_DOMAIN) {
            entity = new Zone(entityId);
        } else {
            Log::log(LOG_LEVEL_ERROR, D_LOG_HA "%s Domain not supported", domain.c_str());
        }
        if (entity != nullptr) {
            this->entities.insert(this->entities.end(), entity);
        }
    }
}

void HomeAssistantManager::removeEntity(const char * entityId) {
    Entity * entity;
    if ((entity=getEntity(entityId))!=nullptr) {
        this->entities.remove(entity);
        delete entity;
    }
}

Entity * HomeAssistantManager::getEntity(const char * entityId) {
    for (auto entity : entities) {
        if (entity->is(entityId)) {
            return entity;
        }
    }
    return nullptr;
}

Entity * HomeAssistantManager::getEntity(uint16_t index) {
    if (index<entities.size()) {
        return *(std::next(entities.begin(), index));
    } else {
        return nullptr;
    }
}

void HomeAssistantManager::poll() {
}

void HomeAssistantManager::callStateService(const char * entityId) {
    Entity * entity = getEntity(entityId);
    if (entity) {
        if (entity->isOn()) {
            entity->callService(this->client, Entity::SERVICE_TURN_ON);
        } else {
            entity->callService(this->client, Entity::SERVICE_TURN_OFF);
        }
    }
}

void HomeAssistantManager::callService(const char * entityId, const char * service) {
    Entity * entity = getEntity(entityId);
    if (entity) {
        entity->callService(this->client, service);
    }
}

            