#include "ConfigHelper.hpp"
#include "Log.hpp"
#include "HomeAssistantClient.hpp"


using namespace HomeAssistant;

void HomeAssistantClient::onEvent(HomeAssistantEventCallback callback) {
  this->_eventsCallback = callback;
}

void HomeAssistantClient::onSyncMessage(HomeAssistantSyncMessageCallback callback) {
  this->_syncMessageCallback = callback;
}

HomeAssistantClient::HomeAssistantClient(String wss_host, String auth_message):
  _eventsCallback([](HomeAssistantClient&, HomeAssistantEvent){}),
  _syncMessageCallback([](HomeAssistantClient&, String, HomeAssistantEntity&, HomeAssistantEntity&){}),
  stateChangeCallback([](HomeAssistantClient*, const char *, JsonObject){}) {
  this->wss_host = wss_host;
  this->auth_message =  auth_message;
  requestMutex = xSemaphoreCreateMutex();
  pollingMutex = xSemaphoreCreateMutex();
}

void HomeAssistantClient::setup () {
  using namespace std::placeholders;
  this->client.onEvent(std::bind(&HomeAssistantClient::websocketsEventCallback, this, _1, _2));
  this->client.onMessage(std::bind(&HomeAssistantClient::websocketsMessageCallback, this, _1));
}

void HomeAssistantClient::send(String message) {
  if(xSemaphoreTake(requestMutex, portMAX_DELAY) == pdTRUE) {
    this->client.send(message);
    xSemaphoreGive(requestMutex);
  }
}

void HomeAssistantClient::websocketsMessageCallback(WebsocketsMessage message) {
  Log::log(LOG_LEVEL_DEBUG_MORE, D_LOG_HA "Message received %s", message.data().c_str());
  if (String(message.data()).indexOf("auth_required") >= 0) {
    this->send(this->auth_message);
  } else if (String(message.data()).indexOf("auth_ok") >= 0) {
    this->_eventsCallback(*this, HomeAssistantConnected);
  } else if ((String(message.data()).indexOf("state_changed") >= 0) || (String(message.data()).indexOf("result") >= 0)) {
    parseMessage(String(message.data()));
  }
}

void HomeAssistantClient::websocketsEventCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
      Log::log(LOG_LEVEL_DEBUG, D_LOG_HA "Home assistant connected");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
      Log::log(LOG_LEVEL_DEBUG, D_LOG_HA "Home assistant disconnected");
    } else if(event == WebsocketsEvent::GotPing) {
      client.pong();
    } else if(event == WebsocketsEvent::GotPong) {
    }
}

void HomeAssistantClient::parseMessage(String message) {
  DeserializationError error = deserializeJson(json_doc, message);
  if (error) {
    Log::log(LOG_LEVEL_ERROR, D_LOG_HA "Deserialazation of message failed");
    return;
  }
  if (json_doc.containsKey("result")) {
    if (json_doc.containsKey("id")) {
      uint32_t id = json_doc["id"];
      pendingMessageIds.remove(id);
      if (pendingMessageIds.empty()) {
        syncing = 0;
        this->_eventsCallback(*this, HomeAssistantConnected);
      }
    }
  }
  String message_entity_id;
  String message_type = "";
  if (json_doc.containsKey("event")) {
    message_entity_id = json_doc["event"]["data"]["entity_id"].as<String>();
    message_type = "event";
  } else if (json_doc.containsKey("result") and json_doc["result"].is<JsonArray>()) {
    message_entity_id = json_doc["result"][0]["entity_id"].as<String>();
    message_type = "result";
  } else {
    message_entity_id = "";
  }
  JsonObject stateObject;
  String state;
  if (message_type == "event") {
    stateObject = json_doc["event"]["data"]["new_state"].as<JsonObject>();
  } else {
    stateObject = json_doc["result"][0].as<JsonObject>();
  }
  stateChangeCallback(this, message_entity_id.c_str(), stateObject);
}

void HomeAssistantClient::connect() {
  pendingMessageIds.clear();
  this->connected = client.connect(this->wss_host);
  if (!this->connected) {
    this->_eventsCallback(*this, HomeAssistantConnectionFailed);
  }
}

void HomeAssistantClient::loop () {
  if(xSemaphoreTake(pollingMutex, portMAX_DELAY) == pdTRUE) {
    
    this->connected = this->client.available();
    if (this->connected) {
      if (this->syncing) {
        if (millis() - this->lastRequestTime > REQUEST_TIMOUT) {
          this->syncing = 0;
          this->_eventsCallback(*this, HomeAssistantConnected);
        }
      }
      this->client.poll();
    } else {
      if (millis() - lastConnectRetryTime > RETRY_CONNECT_TIMEOUT) {
        this->syncing = 0;
        this->_eventsCallback(*this, HomeAssistantDisconnected);
        this->client.close();
        this->connect();
        lastConnectRetryTime = millis();
      }
    }
    xSemaphoreGive(pollingMutex);
  }
}

void HomeAssistantClient::clearCache() {
  for (uint8_t i=0;i<homeassistantConfig.num_entites;i++) {
    memset(homeassistantConfig.entites[i].state, 0, sizeof(homeassistantConfig.entites[i].state));
    homeassistantConfig.entites[i].num_attributes = 0;
  }
  this->_eventsCallback(*this, HomeAssistantSyncing);
  if (this->connected) {
    sync();
  }
}

//new API
void HomeAssistantClient::onStateChange(HomeAssistantStateChangeCallback callback) {
  this->stateChangeCallback = callback;
}

void HomeAssistantClient::callEntityService(const char * domain, const char * service, const char * entity) {
  char data[100];
  snprintf(data, sizeof(data), "\"entity_id\":\"%s\"", entity);
  callService(domain, service, data);
}

void HomeAssistantClient::callEntityService(const char * domain, const char * service, const char * entity, const char * data) {
  char serviceData[400];
  snprintf(serviceData, sizeof(serviceData), "\"entity_id\":\"%s\",%s", entity, data);
  callService(domain, service, serviceData);
}

void HomeAssistantClient::callService(const char * domain, const char * service, const char * data) {
    char message[400];
    snprintf(message, sizeof(message),
        "{"
            "\"id\":%d,"
            "\"type\":\"call_service\","
            "\"domain\":\"%s\","
            "\"service\":\"%s\","
            "\"service_data\":{"
                "%s"
            "}"
        "}", websockets_id_num, domain, service, data);
    Log::log(LOG_LEVEL_DEBUG, D_LOG_HA "Calling service %s", service);
    Log::log(LOG_LEVEL_DEBUG_MORE, D_LOG_HA "Service message %s", message);
    sendMessage(message);
}

void HomeAssistantClient::sendMessage(const char * message) {
    this->syncing = 1;
    this->lastRequestTime = millis();
    pendingMessageIds.push_front(websockets_id_num);
    this->_eventsCallback(*this, HomeAssistantSyncing);
    if(xSemaphoreTake(pollingMutex, portMAX_DELAY) == pdTRUE) {
      this->send(message);
      xSemaphoreGive(pollingMutex);
    }
    websockets_id_num++;
}

void HomeAssistantClient::sync (const char * entity_id) {
  String message = String("{\"id\":")+String(websockets_id_num)
    +String(",\"type\":\"get_states\",\"entity_id\":\"")+String(entity_id)+String("\"}");
  this->send(message);
  this->syncing = 1;
  this->lastRequestTime = millis();
  pendingMessageIds.push_front(websockets_id_num);
  this->_eventsCallback(*this, HomeAssistantSyncing);
  websockets_id_num++;
}

void HomeAssistantClient::subscribe (const char * entityId) {
  String message= String("{\"id\":")+String(websockets_id_num)
    +String(",\"type\":\"subscribe_events\",\"event_type\":\"state_changed\", \"entity_id\":\"")
    +String(entityId)+String("\"}");
  this->syncing = 1;
  this->lastRequestTime = millis();
  pendingMessageIds.push_front(websockets_id_num);
  this->send(message);
  this->_eventsCallback(*this, HomeAssistantSyncing);
  websockets_id_num++;
}