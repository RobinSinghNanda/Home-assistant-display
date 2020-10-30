#include "main.hpp"
#include "Secrets.hpp"

HTTPClient http;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

#define FS_NO_GLOBALS
#include <FS.h>
#if SCREEN_SERVER_ENABLE
#include "ScreenServer.hpp"
#endif
#include "WifiManager.hpp"
ScreenConfig mainScreenConfig;
ScreenConfig settingsScreenConfig;
HomeAssistant homeAssistant(HOME_ASSITANT_WEBSOCKET, HOME_ASSITANT_AUTH_MESSAGE);
WifiManager wifiManager;

char device_name[20];

void touch_add_callback(CallbackFunction func) ;

char haplate_dark_mode_entity [50];

char haplate_clear_cache_entity [50];

char haplate_bottom_bar_entity [50];

char haplate_restart [50];

char haplate_rotation [50];

char haplate_page_num [50];

char haplate_settings_page [51];

TaskHandle_t lcdTaskHandle = NULL;
TaskHandle_t touchTaskHandle = NULL;
TaskHandle_t homeAssitantTaskHandle = NULL;





void setup(void) {
  Serial.begin(115200L);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  uint64_t chipid=ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid>>32);
  snprintf(device_name,sizeof(device_name),"haplate_%04x",chip);
  snprintf(haplate_dark_mode_entity, sizeof(haplate_dark_mode_entity), "input_boolean.%s_dark_mode", device_name);
  snprintf(haplate_clear_cache_entity, sizeof(haplate_clear_cache_entity), "input_boolean.%s_clear_cache", device_name);
  snprintf(haplate_bottom_bar_entity, sizeof(haplate_bottom_bar_entity), "input_boolean.%s_bottom_bar", device_name);
  snprintf(haplate_restart, sizeof(haplate_restart), "input_boolean.%s_restart", device_name);
  snprintf(haplate_rotation, sizeof(haplate_rotation), "input_select.%s_rotation", device_name);
  snprintf(haplate_page_num, sizeof(haplate_page_num), "input_number.%s_page_num", device_name);
  snprintf(haplate_settings_page, sizeof(haplate_settings_page), "input_boolean.%s_settings_page", device_name);
  loadScreenConfiguration("/Main.json",&mainScreenConfig);
  loadScreenConfiguration("/Settings.json",&settingsScreenConfig);
  
  analogReadResolution(10);
  tft_lcd_setup(&mainScreenConfig, &settingsScreenConfig);
  touch_begin();
  touch_add_callback(&handle_touch);
  homeAssistant.setup();
  //get state of dark mode
  String dark_mode_state = homeAssistant.get_state(haplate_dark_mode_entity);
  if (dark_mode_state == "on") {
    tft_set_dark_mode(1);
  } else if (dark_mode_state == "off") {
    tft_set_dark_mode(0);
  }
  homeAssistant.onEvent(homeAssistantEventCallback);
  homeAssistant.onSyncMessage(homeAssistantSyncMessageCallback);
  drawScreen();
  xTaskCreate(
                    lcdTaskCode,       /* Function that implements the task. */
                    "LCD task",          /* Text name for the task. */
                    20000,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &lcdTaskHandle );      /* Used to pass out the created task's handle. */
  xTaskCreate(
                    touchTaskCode,       /* Function that implements the task. */
                    "Touch task",          /* Text name for the task. */
                    20000,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &touchTaskHandle );      /* Used to pass out the created task's handle. */
  setup_wifi();
  add_entities(&mainScreenConfig);
}

void lcdTaskCode( void * pvParameters ) {
    for( ;; ) {
      #ifdef DISPLAY_INFO_STATS
      uint32_t timeBeforeTask = millis();
      #endif
      drawScreen();
      #if SCREEN_SERVER_ENABLE
      screenServer();
      #endif
      #ifdef DISPLAY_INFO_STATS
      uint32_t timeAfterTask = millis();
      uint32_t timeTaken = (timeAfterTask - timeBeforeTask);
      if (timeTaken != 0) {
        Serial.print("Time taken to draw frame: ");
        Serial.println(timeTaken);
      }
      #endif
      vTaskDelay(50/portTICK_PERIOD_MS);
    }
}

void touchTaskCode( void * pvParameters ) {
  for( ;; ) {
    #ifdef DISPLAY_INFO_STATS
    uint32_t timeBeforeTask = millis();
    #endif
    touch_poll();
    #ifdef DISPLAY_INFO_STATS
    uint32_t timeAfterTask = millis();
    uint32_t timeTaken = (timeAfterTask - timeBeforeTask);
    if (timeTaken != 0) {
      Serial.print("Time taken to touch: ");
      Serial.println(timeTaken);
    }
    #endif
    touch_poll();
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
}

void loop() {
  wifi_loop();
  vTaskDelay(100/portTICK_PERIOD_MS);
}

bool justConnected = true;

void wifi_loop() {
  if (WiFi.status() != WL_CONNECTED) {
    justConnected = 1;
    //tft_set_connection_state(TFT_WifiDisconnected);
  } else {
    if (justConnected) {
      tft_set_connection_state(TFT_WifiConnected);
      justConnected = false;
    }
    homeAssistant.loop();
  }
}

void setup_wifi() {
  wifiManager.setup();
}

void add_entities (ScreenConfig * screenConfig) {
  for (uint8_t cardIndex = 0;cardIndex<screenConfig->getNumCards();cardIndex++) {
    if (screenConfig->getCard(cardIndex) != NULL && (String(screenConfig->getCard(cardIndex)->getType()) == PAGE_TYPE_ENTITES)) {
      EntitesCardConfig * pageConfig = (EntitesCardConfig *) screenConfig->getCard(cardIndex);
      for (uint8_t entityIndex =0;entityIndex<pageConfig->getNumEntites();entityIndex++) {
        if (pageConfig->getEntityRow(entityIndex) != NULL) {
          if (String(pageConfig->getEntityRow(entityIndex)->getType()) == ENTITES_ROW_TYPE_DEFAULT) {
            DefaultRowConfig * rowConfig =  (DefaultRowConfig *) pageConfig->getEntityRow(entityIndex);
            homeAssistant.add_entity(rowConfig->getEntityId());
          } else if (String(pageConfig->getEntityRow(entityIndex)->getType()) == ENTITES_ROW_TYPE_BUTTONS) {
            ButtonsRowConfig * rowConfig = (ButtonsRowConfig *) pageConfig->getEntityRow(entityIndex);
            for (uint8_t button_index = 0;button_index<rowConfig->getNumButtons();button_index++) {
              DefaultRowConfig * buttonConfig =  (DefaultRowConfig *) rowConfig->getButton(button_index);
              homeAssistant.add_entity(buttonConfig->getEntityId());
            }
          }
        }
      }
    }
  }
  homeAssistant.add_entity(haplate_dark_mode_entity);
  homeAssistant.add_entity(haplate_restart);
  homeAssistant.add_entity(haplate_bottom_bar_entity);
  homeAssistant.add_entity(haplate_clear_cache_entity);
  homeAssistant.add_entity(haplate_rotation);
  homeAssistant.add_entity(haplate_page_num);
  homeAssistant.add_entity(haplate_settings_page);
}

void homeAssistantEventCallback (HomeAssistant &assistant, HomeAssistantEvent event) {
  if (event == HomeAssistantConnected) {
    tft_set_connection_state(TFT_HaConnected);
  } else if (event == HomeAssistantConnecting) {
    tft_set_connection_state(TFT_HaSyncing);
  } else if (event == HomeAssistantDisconnected) {
    tft_set_connection_state(TFT_WifiConnected);
  } else if (event == HomeAssistantSyncing) {
    tft_set_connection_state(TFT_HaSyncing);
  } else if (event == HomeAssistantConnectionFailed) {
    tft_set_connection_state(TFT_WifiAlert);
  } else if (event == HomeAssistantRequestEnitityList) {
    add_entities(&mainScreenConfig);
  }
}

void homeAssistantSyncMessageCallback (HomeAssistant &assistant, String entity_id, HomeAssistantEntity entity, HomeAssistantEntity  prevEntity) {
  String friendly_name = assistant.get_attribute(entity, "friendly_name");
  String prev_friendly_name = assistant.get_attribute(prevEntity, "friendly_name");
  if (prev_friendly_name != friendly_name) {
     tft_require_update();
  }
  if (entity_id == String(haplate_dark_mode_entity)) {
    if (String(entity.state) == "on")
      tft_set_dark_mode(1);
    else
      tft_set_dark_mode(0);
  } else if (entity_id == String(haplate_rotation)) {
    if (String(entity.state) == "landscape") {
      set_screen_rotation(3);
    } else {
      set_screen_rotation(2);
    }
  } else if (entity_id == String(haplate_bottom_bar_entity)) {
    if (String(entity.state) == "on") {
      tft_set_bottom_bar(1);
    } else {
      tft_set_bottom_bar(0);
    }
  } else if (entity_id == String(haplate_page_num)) {
    uint32_t page_num = String(entity.state).toInt();
    tft_set_page_num(page_num-1);
  } else if (entity_id == String(haplate_settings_page)) {
    if (String(entity.state) == "on") {
      tft_set_settings_page(1);
    } else {
      tft_set_settings_page(0);
    }
  }
  tft_require_update();
}
