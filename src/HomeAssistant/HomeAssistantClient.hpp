#ifndef HOME_ASSISTANT_H_
#define HOME_ASSISTANT_H_

#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <string>
using namespace std;

#include <list>

using namespace websockets;

#define MAX_NUM_ENTITES 40
#define MAX_NUM_OF_ATTRIBUTES 10
#define DEBUG_HOME_ASSITANT 0
#define REQUEST_TIMOUT 10000
#define RETRY_CONNECT_TIMEOUT 2000

namespace HomeAssistant {
    struct HomeAssistantEntityAttribute {
      char name[30];
      char value[50];
    };
    struct HomeAssistantEntity {
      char entity_id[40];
      char state[20];
      uint8_t num_attributes;
      uint8_t valid = 1;
      HomeAssistantEntityAttribute attributes[MAX_NUM_OF_ATTRIBUTES];
    };
    struct HomeAssistantConfig {
      HomeAssistantEntity entites[MAX_NUM_ENTITES];
      uint8_t num_entites=0;
    };
    enum HomeAssistantEvent {
      HomeAssistantDisconnected,
      HomeAssistantConnected,
      HomeAssistantConnecting,
      HomeAssistantSyncing,
      HomeAssistantConnectionFailed,
      HomeAssistantRequestEnitityList
    };
    class HomeAssistantClient;
    typedef std::function<void(HomeAssistantClient&, HomeAssistantEvent)> HomeAssistantEventCallback;
    typedef std::function<void(HomeAssistantClient&, String, HomeAssistantEntity&, HomeAssistantEntity&)> HomeAssistantSyncMessageCallback;
    typedef std::function<void(HomeAssistantClient*, const char * entity, JsonObject stateObject)> HomeAssistantStateChangeCallback;
    class HomeAssistantClient {
      public:
          HomeAssistantClient(String wss_host, String auth_message);
          void setup ();
          void connect();
          void onEvent(const HomeAssistantEventCallback callback);
          void onSyncMessage(const HomeAssistantSyncMessageCallback callback);
          void addEntity (String entity_id);
          void setState (String entity_id, String state);
          String getState (String entity_id);
          String get_state (HomeAssistantEntity &entity);
          void set_attribute (String entity_id, String attribute_name, String attribute_value);
          String get_attribute (String entity_id, String attribute_name);
          String get_attribute (HomeAssistantEntity &entity, String attribute_name);
          void sync_state (String entity_id, String state);
          void loop ();
          void clearCache();
          //New API
          void sync (const char * entityId);
          void subscribe(const char * entityId);
          void onStateChange(HomeAssistantStateChangeCallback callback);
          void callEntityService(const char * domain, const char * service, const char * entity);
          void callEntityService(const char * domain, const char * service, const char * entity, const char * data);
          void callService(const char * domain, const char * service, const char * data);
          void sendMessage(const char * message);
      private:
          String wss_host;
          String auth_message;
          std::list<int> pendingMessageIds;
          StaticJsonDocument<4096> json_doc;
          uint32_t websockets_id_num = 5;
          uint32_t lastRequestTime = 0;
          uint32_t lastConnectRetryTime = 0;
          uint32_t syncing = 0;
          uint32_t connected = 0;
          SemaphoreHandle_t requestMutex;
          SemaphoreHandle_t pollingMutex;
          HomeAssistantConfig homeassistantConfig;
          HomeAssistantEntity prevEntity;
          HomeAssistantEventCallback _eventsCallback;
          HomeAssistantSyncMessageCallback _syncMessageCallback;
          WebsocketsClient client;
          void websocketsMessageCallback(WebsocketsMessage message);
          void websocketsEventCallback(WebsocketsEvent event, String data);
          void sync ();
          void sync (char * entity_id);
          void subscribe ();
          void subscribe (char * entity_id);
          int8_t get_entity_index (String entity_id);
          int8_t get_attribute_index (int8_t index, String attribute_name);
          int8_t get_attribute_index (HomeAssistantEntity &entity, String attribute_name);
          void parseMessage(String message);
          void send(String message);
          //new API
          HomeAssistantStateChangeCallback stateChangeCallback;
    };
}

#endif
