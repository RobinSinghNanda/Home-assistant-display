#ifndef HOME_ASSISTANT_H_
#define HOME_ASSISTANT_H_

#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

#include <list>

using namespace websockets;

#define MAX_NUM_ENTITES 40
#define MAX_NUM_OF_ATTRIBUTES 10
#define DEBUG_HOME_ASSITANT 0
#define REQUEST_TIMOUT 10000

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

class HomeAssistant;

typedef std::function<void(HomeAssistant&, HomeAssistantEvent)> HomeAssistantEventCallback;
typedef std::function<void(HomeAssistant&, String, HomeAssistantEntity&, HomeAssistantEntity&)> HomeAssistantSyncMessageCallback;

class HomeAssistant {
  public:
    HomeAssistant(String wss_host, String auth_message);
    void setup ();
    void connect();
    void onEvent(const HomeAssistantEventCallback callback);
    void onSyncMessage(const HomeAssistantSyncMessageCallback callback);
    void add_entity (String entity_id);
    void set_state (String entity_id, String state);
    String get_state (String entity_id);
    String get_state (HomeAssistantEntity &entity);
    void set_attribute (String entity_id, String attribute_name, String attribute_value);
    String get_attribute (String entity_id, String attribute_name);
    String get_attribute (HomeAssistantEntity &entity, String attribute_name);
    void sync_state (String entity_id, String state);
    void loop ();
    void clear_cache();
  private:
    String wss_host;
    String auth_message;
    std::list<int> pendingMessageIds;
    StaticJsonDocument<2048> json_doc;
    uint32_t websockets_id_num = 5;
    uint32_t lastRequestTime = 0;
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
    void sync_states ();
    void sync_entity_state (char * entity_id);
    void subscribe_to_states_changed ();
    void subscribe_to_state_changed (char * entity_id);
    int8_t get_entity_index (String entity_id);
    int8_t get_attribute_index (int8_t index, String attribute_name);
    int8_t get_attribute_index (HomeAssistantEntity &entity, String attribute_name);
    void parseMessage(String message);
    void send(String message);
};



#endif
