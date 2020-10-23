#include "HomeAssistant.hpp"
#include "ConfigHelper.hpp"




const char * attributes_white_list [] = {"friendly_name", "speed", "icon", "device_class", "brightness", "min_mireds", "max_mireds", "rgb_color", "white_value", "color_temp"};

void HomeAssistant::onEvent(HomeAssistantEventCallback callback) {
  this->_eventsCallback = callback;
}

void HomeAssistant::onSyncMessage(HomeAssistantSyncMessageCallback callback) {
  this->_syncMessageCallback = callback;
}

HomeAssistant::HomeAssistant(String wss_host, String auth_message):
  _eventsCallback([](HomeAssistant&, HomeAssistantEvent){}),
  _syncMessageCallback([](HomeAssistant&, String, HomeAssistantEntity&, HomeAssistantEntity&){}) {
  this->wss_host = wss_host;
  this->auth_message =  auth_message;
  requestMutex = xSemaphoreCreateMutex();
  pollingMutex = xSemaphoreCreateMutex();
}

void HomeAssistant::setup () {
  using namespace std::placeholders;
  this->client.onMessage(std::bind(&HomeAssistant::websocketsMessageCallback, this, _1));
  this->client.onEvent(std::bind(&HomeAssistant::websocketsEventCallback, this, _1, _2));
  loadCachedStateConfiguration("/state_cache.json", &homeassistantConfig);
}


void HomeAssistant::send(String message) {
  if(xSemaphoreTake(requestMutex, portMAX_DELAY) == pdTRUE) {
    this->client.send(message);
    xSemaphoreGive(requestMutex);
  } else {
    Serial.println("Could not obtain the mutex");
  }
}


void HomeAssistant::websocketsMessageCallback(WebsocketsMessage message) {
  if (DEBUG_HOME_ASSITANT) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
  }
  if (String(message.data()).indexOf("auth_required") >= 0) {
    this->send(this->auth_message);
  } else if (String(message.data()).indexOf("auth_ok") >= 0) {
    this->_eventsCallback(*this, HomeAssistantConnected);
    sync_states();
    subscribe_to_states_changed();
  } else if ((String(message.data()).indexOf("state_changed") >= 0) || (String(message.data()).indexOf("result") >= 0)) {
    parseMessage(String(message.data()));
  }
}

void HomeAssistant::websocketsEventCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
      Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
      Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
      client.pong();
    } else if(event == WebsocketsEvent::GotPong) {
    }
}

void HomeAssistant::sync_states () {
  for (uint8_t i=0;i<homeassistantConfig.num_entites;i++) {
    sync_entity_state(homeassistantConfig.entites[i].entity_id);
  }
}

void HomeAssistant::sync_entity_state (char * entity_id) {
  String message = String("{\"id\":")+String(websockets_id_num)
    +String(",\"type\":\"get_states\",\"entity_id\":\"")+String(entity_id)+String("\"}");
  if (DEBUG_HOME_ASSITANT) {
    Serial.println(message);
  }
  this->send(message);
  this->syncing = 1;
  this->lastRequestTime = millis();
  pendingMessageIds.push_front(websockets_id_num);
  this->_eventsCallback(*this, HomeAssistantSyncing);
  websockets_id_num++;
}

void HomeAssistant::subscribe_to_state_changed (char * entity_id) {
  String message= String("{\"id\":")+String(websockets_id_num)
    +String(",\"type\":\"subscribe_events\",\"event_type\":\"state_changed\", \"entity_id\":\"")
    +String(entity_id)+String("\"}");
  if (DEBUG_HOME_ASSITANT) {
    Serial.println(message);
  }
  this->syncing = 1;
  this->lastRequestTime = millis();
  pendingMessageIds.push_front(websockets_id_num);
  this->send(message);
  this->_eventsCallback(*this, HomeAssistantSyncing);
  websockets_id_num++;
}

void HomeAssistant::subscribe_to_states_changed () {
  for (uint8_t i=0;i<homeassistantConfig.num_entites;i++) {
    subscribe_to_state_changed(homeassistantConfig.entites[i].entity_id);
  }
}

void HomeAssistant::parseMessage(String message) {
  DeserializationError error = deserializeJson(json_doc, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    return;
  }
  if (json_doc.containsKey("result")) {
    if (json_doc.containsKey("id")) {
      uint32_t id = json_doc["id"];
      pendingMessageIds.remove(id);
      if (pendingMessageIds.empty()) {
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
  JsonObject state_object;
  String state;
  int8_t index = this->get_entity_index(message_entity_id);
  if (index !=-1) {
    prevEntity = homeassistantConfig.entites[index];
    if (message_type == "event") {
      state_object = json_doc["event"]["data"]["new_state"].as<JsonObject>();
    } else {
      state_object = json_doc["result"][0].as<JsonObject>();
    }
    if (state_object.containsKey("state")) {
      this->set_state(message_entity_id, state_object["state"].as<String>());
    }
    if (state_object.containsKey("attributes")) {
      JsonObject attributes_object = state_object["attributes"];
      for (JsonPair kv : attributes_object) {
        String attribute_name = String(kv.key().c_str());
        for(const char * whitelist_attribute: attributes_white_list) {
          if (attribute_name == String(whitelist_attribute)) {
            String prev_attribute_value = this->get_attribute(message_entity_id, attribute_name);
            this->set_attribute(message_entity_id, attribute_name, kv.value().as<String>());
            break;
          }
        }
      }
    }
    _syncMessageCallback(*this, message_entity_id, homeassistantConfig.entites[index], prevEntity);
    saveCachedStateConfiguration("/state_cache.json", &homeassistantConfig);
  }
}

void HomeAssistant::connect() {
  pendingMessageIds.clear();
  this->connected = client.connect(this->wss_host);
  if (!this->connected) {
    this->_eventsCallback(*this, HomeAssistantConnectionFailed);
  }
}

void HomeAssistant::sync_state (String entity_id, String state) {
  if (this->get_entity_index(entity_id)!=-1) {
    String domain = entity_id.substring(0, entity_id.indexOf("."));
    String message;
    if (domain == "fan") {
      if (state == "off") {
        message = String("{\"id\":")+String(websockets_id_num)
            +String(",\"type\":\"call_service\",\"domain\":\"")+domain+String("\",\"service\":\"turn_")
            +state+String("\",\"service_data\":{\"entity_id\":\"")
            +entity_id+String("\"}}");
      } else {
        String speed = get_attribute(entity_id, "speed");
        if (speed != "") {
          message = String("{\"id\":")+String(websockets_id_num)
            +String(",\"type\":\"call_service\",\"domain\":\"")+domain+String("\",\"service\":\"set_speed\"")+
            String(",\"service_data\":{\"entity_id\":\"")
            +entity_id+String("\", \"speed\":\"")+speed+String("\"}}");
        } else {
          message = String("{\"id\":")+String(websockets_id_num)
            +String(",\"type\":\"call_service\",\"domain\":\"")+domain+String("\",\"service\":\"turn_")
            +state+String("\",\"service_data\":{\"entity_id\":\"")
            +entity_id+String("\"}}");
        }
      }
    } else if (domain == "light") {
      if (state == "off") {
        message = String("{\"id\":")+String(websockets_id_num)
            +String(",\"type\":\"call_service\",\"domain\":\"")+domain+String("\",\"service\":\"turn_")
            +state+String("\",\"service_data\":{\"entity_id\":\"")
            +entity_id+String("\"}}");
      } else {
        String brightness = get_attribute(entity_id, "brightness");
        String rgb_color = get_attribute(entity_id, "rgb_color");
        String color_temp = get_attribute(entity_id, "color_temp");
        String white_value = get_attribute(entity_id, "white_value");
        String arributesStr = "";
        if (brightness != "" && brightness != "0") {
          arributesStr = ",\"brightness\":"+brightness;
        }
        if (color_temp != "") {
          arributesStr = arributesStr+",\"color_temp\":"+color_temp;
        }
        if (white_value != "" && white_value != "0") {
          arributesStr = arributesStr+",\"white_value\":"+white_value;
        } else if (rgb_color != "") {
          arributesStr = arributesStr+",\"rgb_color\":"+rgb_color;
        }
        
        message = String("{\"id\":")+String(websockets_id_num)
            +String(",\"type\":\"call_service\",\"domain\":\"")+domain+String("\",\"service\":\"turn_")
            +state+String("\",\"service_data\":{\"entity_id\":\"")
            +entity_id+String("\"")+arributesStr+"}}";
      }
    } else {
      message = String("{\"id\":")+String(websockets_id_num)
        +String(",\"type\":\"call_service\",\"domain\":\"")+domain+String("\",\"service\":\"turn_")
        +state+String("\",\"service_data\":{\"entity_id\":\"")
        +entity_id+String("\"}}");
    }
    if (DEBUG_HOME_ASSITANT) {
      Serial.println(message);
    }
    this->syncing = 1;
    this->lastRequestTime = millis();
    pendingMessageIds.push_front(websockets_id_num);
    this->_eventsCallback(*this, HomeAssistantSyncing);
    if(xSemaphoreTake(pollingMutex, portMAX_DELAY) == pdTRUE) {
      this->send(message);
      xSemaphoreGive(pollingMutex);
    } else {
      Serial.println("Could not obtain the mutex");
    }
    websockets_id_num++;
    int8_t index = this->get_entity_index(entity_id);
    if (index != -1) {
      strlcpy(homeassistantConfig.entites[index].state, state.c_str(), sizeof(homeassistantConfig.entites[index].state));
    }
  } else {
    if (DEBUG_HOME_ASSITANT) {
      Serial.println(entity_id+" does not exists");
    }
  }
}


void HomeAssistant::set_state (String entity_id, String state) {
  if (this->get_entity_index(entity_id)!=-1) {
    int8_t index = this->get_entity_index(entity_id);
    if (index != -1) {
      strlcpy(homeassistantConfig.entites[index].state, state.c_str(), sizeof(homeassistantConfig.entites[index].state));
    }
  } else {
    if (DEBUG_HOME_ASSITANT) {
      Serial.println(entity_id+" does not exists");
    }
  }
}

String HomeAssistant::get_state (String entity_id) {
  int8_t index = this->get_entity_index(entity_id);
  if (index != -1) {
    return homeassistantConfig.entites[index].state;
  } else {
    if (DEBUG_HOME_ASSITANT) {
      Serial.println(entity_id+" does not exists");
    }
  }
  return "";
}

String HomeAssistant::get_state (HomeAssistantEntity &entity) {
  return entity.state;
}

void HomeAssistant::set_attribute (String entity_id, String attribute_name, String attribute_value) {
  int8_t index = this->get_entity_index(entity_id);
  if (index != -1) {
    int8_t attribute_index = this->get_attribute_index(index, attribute_name);
    if (attribute_index != -1) {
       strlcpy(homeassistantConfig.entites[index].attributes[attribute_index].value, 
         attribute_value.c_str(),
         sizeof(homeassistantConfig.entites[index].attributes[attribute_index].value));
    } else {
      strlcpy(homeassistantConfig.entites[index].attributes[homeassistantConfig.entites[index].num_attributes].name
        , attribute_name.c_str(),
        sizeof(homeassistantConfig.entites[index].attributes[homeassistantConfig.entites[index].num_attributes].name));
      strlcpy(homeassistantConfig.entites[index].attributes[homeassistantConfig.entites[index].num_attributes].value
        , attribute_value.c_str(),
        sizeof(homeassistantConfig.entites[index].attributes[homeassistantConfig.entites[index].num_attributes].value));
      homeassistantConfig.entites[index].num_attributes++;
    }
  } else {
    if (DEBUG_HOME_ASSITANT) {
      Serial.println(entity_id+" does not exists");
    }
  }
}

String HomeAssistant::get_attribute (String entity_id, String attribute_name) {
  int8_t index = this->get_entity_index(entity_id);
  if (index != -1) {
    int8_t attribute_index = this->get_attribute_index(index, attribute_name);
    if (attribute_index != -1) {
      return homeassistantConfig.entites[index].attributes[attribute_index].value;
    } else {
      return "";
    }
  } else {
    if (DEBUG_HOME_ASSITANT) {
      Serial.println(entity_id+" does not exists");
    }
  }
  return "";
}

String HomeAssistant::get_attribute (HomeAssistantEntity &entity, String attribute_name) {
  int8_t attribute_index = this->get_attribute_index(entity, attribute_name);
  if (attribute_index != -1) {
    return entity.attributes[attribute_index].value;
  } else {
    return "";
  }
}

int8_t HomeAssistant::get_entity_index (String entity_id) {
  for (uint8_t i=0;i<homeassistantConfig.num_entites;i++) {
    if (entity_id == homeassistantConfig.entites[i].entity_id) {
      return i;
    }
  }
  return -1;
}

int8_t HomeAssistant::get_attribute_index (int8_t index, String attribute_name) {
  HomeAssistantEntity * entity_state = &(homeassistantConfig.entites[index]);
  for (uint8_t i=0;i<entity_state->num_attributes;i++) {
    if (attribute_name == entity_state->attributes[i].name) {
      return i;
    }
  }
  return -1;
}

int8_t HomeAssistant::get_attribute_index (HomeAssistantEntity &entity, String attribute_name) {
  for (uint8_t i=0;i<entity.num_attributes;i++) {
    if (attribute_name == entity.attributes[i].name) {
      return i;
    }
  }
  return -1;
}

void HomeAssistant::add_entity (String entity_id) {
  if (this->get_entity_index(entity_id) == -1) {
    if (homeassistantConfig.num_entites == MAX_NUM_ENTITES) {
      Serial.println("Max number of states stored");
      return;
    }
    strlcpy(homeassistantConfig.entites[homeassistantConfig.num_entites].entity_id,
      entity_id.c_str(),sizeof(homeassistantConfig.entites[homeassistantConfig.num_entites].entity_id));
    strlcpy(homeassistantConfig.entites[homeassistantConfig.num_entites].state,
      "",sizeof(homeassistantConfig.entites[homeassistantConfig.num_entites].state));
    homeassistantConfig.entites[homeassistantConfig.num_entites].num_attributes = 0;
    homeassistantConfig.num_entites++;
  } else {
    if (DEBUG_HOME_ASSITANT) {
    
    }
  }
}

void HomeAssistant::loop () {
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
      this->syncing = 0;
      this->_eventsCallback(*this, HomeAssistantDisconnected);
      this->client.close();
      this->connect();
    }
    xSemaphoreGive(pollingMutex);
  } else {
    Serial.println("Could not obtain the mutex");
  }
}

//TODO: not working
void HomeAssistant::clear_cache() {
  this->homeassistantConfig.num_entites = 0;
  this->_eventsCallback(*this, HomeAssistantSyncing);
  if (this->connected) {
    sync_states();
  }
}