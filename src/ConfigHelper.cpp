#include <ArduinoJson.h>

#define FS_NO_GLOBALS
#include <FS.h>

#include "ConfigHelper.hpp"
#include "LovelaceConfig/CardTypes.hpp"
#include "LovelaceConfig/CardConfigParser.hpp"

StaticJsonDocument<6000> doc;

void printScreenConfiguration(ScreenConfig * screenConfig) {
  Serial.println("#Screen config start");
  Serial.println("screen_config:");
  // for (uint8_t page_index = 0;page_index<screenConfig->num_pages;page_index++) {
  //   if (screenConfig->pages[page_index] != NULL && (screenConfig->pages[page_index])->type == EntitiesCardConfig::TYPE) {
  //     Serial.print("  - type: ");
  //     Serial.println((screenConfig->pages[page_index])->type);
  //     Serial.print("    title: ");
  //     Serial.println(screenConfig->pages[page_index]->title);
  //     Serial.print("    icon: ");
  //     Serial.println(screenConfig->pages[page_index]->icon);
  //     Serial.println("    entites:");
  //     EntitesCardConfig * pageConfig = (EntitesCardConfig *) screenConfig->pages[page_index];
  //     if (pageConfig->numEntities == 0) {
  //       Serial.println(" ~");
  //     } else {
  //       Serial.println("");
  //     }
  //     for (uint8_t row_index = 0;row_index<pageConfig->numEntities;row_index++) {
  //       if (pageConfig->entities[row_index] != NULL && pageConfig->entities[row_index]->type == DefaultRowConfig::TYPE) {
  //         DefaultEntityConfig * rowConfig =  (DefaultEntityConfig *) pageConfig->entities[row_index];
  //         Serial.print("      - entity: ");
  //         Serial.println(rowConfig->entity);
  //         Serial.print("        name: ");
  //         Serial.println(rowConfig->name);
  //         Serial.print("        icon: ");
  //         Serial.println(rowConfig->icon);
  //         Serial.print("        state_color: ");
  //         Serial.println(rowConfig->state_color);
  //         Serial.print("        hide_if_unavailable: ");
  //         Serial.println(rowConfig->hide_if_unavailable);
  //       } else {
  //         Serial.println("      - ~");
  //       }
  //     }
  //   }
  // }
  Serial.println("#Screen config end");
}

void loadScreenConfiguration(const char *filename, ScreenConfig * screenConfig) {
  fs::File configFile = SPIFFS.open(filename, "r");

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
    Serial.println(error.c_str());
    return;
  }
  if (doc.is<JsonArray>()) {
    JsonArray screens = doc.as<JsonArray>();
    for(uint8_t screenIndex=0;screenIndex<screens.size();screenIndex++) {
      if (screens[screenIndex].is<JsonObject>()) {
        screenConfig->addCard(CardConfigParser::parseCardConfig(screens[screenIndex]));
      }
    }
  } else {
    Serial.println(F("The root is expected to array type"));
  }
  configFile.close();
}

#ifndef FIRMWARE_MINIMAL
void loadCachedStateConfiguration(const char *filename, HomeAssistantConfig * homeassistantConfig) {
  fs::File configFile = SPIFFS.open(filename, "r");

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    homeassistantConfig->num_entites = 0;
    Serial.println(F("Failed to read cache file discarding the cache"));
    Serial.println(error.c_str());
    return;
  }
  if (doc.is<JsonArray>()) {
    JsonArray states = doc.as<JsonArray>();
    homeassistantConfig->num_entites = states.size();
    if (homeassistantConfig->num_entites > MAX_NUM_ENTITES) {
      homeassistantConfig->num_entites = MAX_NUM_ENTITES;
      Serial.println(F("Num of states is more than 10. Trucating to 10"));
    }
    for(uint8_t i=0;i<homeassistantConfig->num_entites;i++) {
      JsonVariant state = states[i].as<JsonVariant>();
      HomeAssistantEntity * config_state = &(homeassistantConfig->entites[i]);
      if (state["entity_id"] && state["entity_id"].as<String>().length() != 0) {
        strlcpy(config_state->entity_id, state["entity_id"] | "", sizeof(config_state->entity_id));
        strlcpy(config_state->state, state["state"] | "unavailable", sizeof(config_state->state));
        config_state->num_attributes = 0;
        if ((doc[i]["attributes"].is<JsonArray>())) {
          JsonArray attributes = (doc[i]["attributes"]).as<JsonArray>();
          config_state->num_attributes = attributes.size();
          if (config_state->num_attributes > MAX_NUM_OF_ATTRIBUTES) {
            config_state->num_attributes = MAX_NUM_OF_ATTRIBUTES;
            Serial.println(F("Num of attributes is more than 10. Trucating to 10"));
          }
          for (uint8_t j=0;j<config_state->num_attributes;j++) {
            HomeAssistantEntityAttribute * config_attribute = &(config_state->attributes[j]);
            JsonVariant attribute = attributes[j].as<JsonVariant>();
            if (attribute["name"]) {
              strlcpy(config_attribute->name, attribute["name"] | "", sizeof(config_attribute->name));
              strlcpy(config_attribute->value, attribute["value"] | "", sizeof(config_attribute->value));
            }
          }
        } else {
          Serial.println("Attributes is not array");
        }
      } else {
        Serial.println(F("Entity ID not defined. Skipping."));
        homeassistantConfig->num_entites--;
        i--;
        continue;
      }
    }
  } else {
    Serial.println(F("The root is expected to array type"));
  }
  configFile.close();
}

void saveCachedStateConfiguration(const char *filename, HomeAssistantConfig * homeassistantConfig) {
  SPIFFS.remove(filename);
  fs::File configFile = SPIFFS.open(filename, "w");
  if (!configFile) {
    Serial.println(F("Failed to create file"));
    return;
  }
  JsonArray doc_states = doc.to<JsonArray>();
  for(uint8_t i=0;i<homeassistantConfig->num_entites;i++) {
    JsonObject doc_state = doc_states.createNestedObject(); 
    doc_state["entity_id"] = String(homeassistantConfig->entites[i].entity_id);
    doc_state["state"] = String(homeassistantConfig->entites[i].state);
    JsonArray doc_attributes = doc_state.createNestedArray("attributes");
    for (uint8_t j=0;j<homeassistantConfig->entites[i].num_attributes;j++) {
      JsonObject doc_attribute = doc_attributes.createNestedObject();
      doc_attribute["name"] = homeassistantConfig->entites[i].attributes[j].name;
      doc_attribute["value"] = homeassistantConfig->entites[i].attributes[j].value;
    }
  }
  if (serializeJson(doc, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  configFile.close();
}

void printCachedStateConfiguration(HomeAssistantConfig * homeassistantConfig) {
  Serial.println("#Home assistant state start");
  for(uint8_t i=0;i<homeassistantConfig->num_entites;i++) {
    Serial.print(homeassistantConfig->entites[i].entity_id);
    Serial.println(":");
    Serial.print("  state: ");
    Serial.println(homeassistantConfig->entites[i].state);
    Serial.print("  attributes:");
    if (homeassistantConfig->entites[i].num_attributes == 0) {
      Serial.println(" ~");
    } else {
      Serial.println("");
    }
    for (uint8_t j=0;j<homeassistantConfig->entites[i].num_attributes;j++) {
      Serial.print("    name: ");
      Serial.println(homeassistantConfig->entites[i].attributes[j].name);
      Serial.print("    value: ");
      Serial.println(homeassistantConfig->entites[i].attributes[j].value);
    }
  }
  Serial.println("#Home assistant state end");
}
#endif