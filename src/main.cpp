#include "main.hpp"
#include "Config.hpp"
#include "Settings.hpp"
#include "GlobalParams.hpp"
#include "i18n.h"

HTTPClient http;

#define FS_NO_GLOBALS
#include <FS.h>
#if SCREEN_SERVER_ENABLE
#include "ScreenServer.hpp"
#endif
#include "ESPAsyncWebServer.h"
#include "DNSServer.h"
#include <ESPmDNS.h>
#include "WifiManager.hpp"
#include "RtcTime.hpp"
ScreenConfig mainScreenConfig;
ScreenConfig settingsScreenConfig;

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
#ifndef FIRMWARE_MINIMAL
TaskHandle_t HomeAssistantTaskHandle = NULL;
#endif
extern Settings settings;
void setup(void) {
  Serial.begin(115200L);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  uint64_t chipid=ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid>>32);
  snprintf(device_name,sizeof(device_name),"had%04x",chip);
  snprintf(haplate_dark_mode_entity, sizeof(haplate_dark_mode_entity), "input_boolean.%s_dark_mode", device_name);
  snprintf(haplate_clear_cache_entity, sizeof(haplate_clear_cache_entity), "input_boolean.%s_clear_cache", device_name);
  snprintf(haplate_bottom_bar_entity, sizeof(haplate_bottom_bar_entity), "input_boolean.%s_bottom_bar", device_name);
  snprintf(haplate_restart, sizeof(haplate_restart), "input_boolean.%s_restart", device_name);
  snprintf(haplate_rotation, sizeof(haplate_rotation), "input_select.%s_rotation", device_name);
  snprintf(haplate_page_num, sizeof(haplate_page_num), "input_number.%s_page_num", device_name);
  snprintf(haplate_settings_page, sizeof(haplate_settings_page), "input_boolean.%s_settings_page", device_name);
  loadScreenConfiguration("/Main.json",&mainScreenConfig);
  getSettingsScreenConfig(&settingsScreenConfig);
  //loadScreenConfiguration("/Settings.json",&settingsScreenConfig);
  analogReadResolution(10);
  tft_lcd_setup(&mainScreenConfig, &settingsScreenConfig);
  touch_begin();
  touch_add_callback(&handle_touch);
  #ifndef FIRMWARE_MINIMAL
  homeAssistant.setup();
  #endif
  //get state of dark mode
  String dark_mode_state = homeAssistant.getState(haplate_dark_mode_entity);
  if (dark_mode_state == "on") {
    tft_set_dark_mode(1);
  } else if (dark_mode_state == "off") {
    tft_set_dark_mode(0);
  }
  drawScreen();
  #ifndef FIRMWARE_MINIMAL
  homeAssistant.onEvent(homeAssistantEventCallback);
  homeAssistant.onSyncMessage(homeAssistantSyncMessageCallback);
  
  xTaskCreate(
                    homeAssistantTaskCode,       /* Function that implements the task. */
                    "homeAssistant task",          /* Text name for the task. */
                    5000,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    0,/* Priority at which the task is created. */
                    &HomeAssistantTaskHandle );      /* Used to pass out the created task's handle. */
  //setup_wifi();
  add_entities(&mainScreenConfig);
  add_entities(&settingsScreenConfig);
  #endif
  MDNS.addService("http","tcp",80);
  settings.load();
  wifiManager.connect();
  RtcInit();
  Serial.printf("Dark mode is %d\n", settings.isDarkMode());
  // tft_set_dark_mode(settings.isDarkMode());
  // tft_set_bottom_bar(settings.isBottomBar());
  if (strlen(settings.getStaWifiSsid1()) == 0 && strlen(settings.getStaWifiSsid2()) == 0) {
    globalParams.setScreenPageType(ScreenPageSetup);
  }
  updateEntityStates(1);
  homeAssistant.addEntity(String(haplate_page_num));
  homeAssistant.addEntity(String(haplate_settings_page));
  homeAssistant.addEntity(String(haplate_rotation));
  homeAssistant.addEntity(String(haplate_restart));
}
#ifndef FIRMWARE_MINIMAL
void homeAssistantTaskCode( void * pvParameters ) {
  for( ;; ) {
    #ifdef DISPLAY_INFO_STATS
    uint32_t timeBeforeTask = millis();
    #endif
    if (globalParams.isWifiConnected()) {
      homeAssistant.loop();
    }
    #ifdef DISPLAY_INFO_STATS
    uint32_t timeAfterTask = millis();
    uint32_t timeTaken = (timeAfterTask - timeBeforeTask);
    if (timeTaken != 0) {
      Serial.print("Time taken to home assistant: ");
      Serial.println(timeTaken);
    }
    #endif
    touch_poll();
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
}
#endif

bool justConnected = true;
uint8_t counter = 0;
void startwebServer(int type, IPAddress ipweb);

void Every50mSeconds () {

}

void Every100mSeconds () {
  touch_poll();
}

void Every250mSeconds () {
  if ((millis() - globalParams.getLastTouchTimeMillis()) > settings.getScreenSaverTime() && settings.isScreenSaverEnabled()) {
    if (globalParams.getScreenPageType() != ScreenPageSetup &&
        globalParams.getScreenPageType() != ScreenPageBoot &&
        globalParams.getScreenPageType() != ScreenPageScreenSaver &&
        rtcTime.valid) {
      globalParams.setScreenPageType(ScreenPageScreenSaver);
    }
  }
  if (globalParams.getScreenPageType() == ScreenPageScreenSaver && !settings.isScreenSaverEnabled()) {
    globalParams.setScreenPageType(ScreenPageMain);
  }
  drawScreen();
  #if SCREEN_SERVER_ENABLE
  screenServer();
  #endif
  if (!globalParams.getRestartFlag()) { settings.save();}
}

void Every500mSeconds () {
  wifiManager.check(globalParams.getWifiState());
  globalParams.setWifiState(WIFI_RESTART);
  Webserver_loop();
  if (globalParams.isWifiConnected()) {
    startwebServer(2, WiFi.localIP());
  }
}


void PerformEverySecond () {
  globalParams.incrUptime();;
  if (globalParams.getScreenPageType() == ScreenPageScreenSaver) {
    globalParams.setScreenRedraw(true);
  }
  if (globalParams.getRestartFlag()) {
    if (2 == globalParams.getRestartFlag()) {
        settings.save();
    }
    Support::EspRestart();
  }
  updateEntityStates();
}

void loop() {
  uint32_t my_sleep = millis();

  static uint32_t state_50msecond = 0;               // State 50msecond timer
  if (Support::timeReached(state_50msecond)) {
    Support::setNextTimeInterval(state_50msecond, 50);
    Every50mSeconds();
  }

  static uint32_t state_100msecond = 0;              // State 100msecond timer
  if (Support::timeReached(state_100msecond)) {
    Support::setNextTimeInterval(state_100msecond, 100);
    Every100mSeconds();
  }

  static uint32_t state_250msecond = 0;              // State 250msecond timer
  if (Support::timeReached(state_250msecond)) {
    Support::setNextTimeInterval(state_250msecond, 250);
    Every250mSeconds();
  }

  static uint32_t state_500msecond = 0;              // State 250msecond timer
  if (Support::timeReached(state_500msecond)) {
    Support::setNextTimeInterval(state_500msecond, 500);
    Every500mSeconds();
  }

  static uint32_t state_second = 0;                  // State second timer
  if (Support::timeReached(state_second)) {
    Support::setNextTimeInterval(state_second, 1000);
    PerformEverySecond();
  }

  uint32_t my_activity = millis() - my_sleep;
  if (my_activity < (uint32_t)globalParams.getSleepTime()) {
    // if (my_activity != 0) {
    //   Serial.printf("Sleeping for %d\n", (globalParams.getSleepTime() - my_activity));
    // }
    delay(globalParams.getSleepTime() - my_activity);  // Provide time for background tasks like wifi
  } else {
    if (globalParams.isWifiDown()) {
      delay(my_activity /2);                // If wifi down and my_activity > setoption36 then force loop delay to 1/3 of my_activity period
    }
  }
  // if (!my_activity) { my_activity++; }             // We cannot divide by 0
  // uint32_t loop_delay = TasmotaGlobal.sleep;
  // if (!loop_delay) { loop_delay++; }               // We cannot divide by 0
  // uint32_t loops_per_second = 1000 / loop_delay;   // We need to keep track of this many loops per second
  // uint32_t this_cycle_ratio = 100 * my_activity / loop_delay;
  // TasmotaGlobal.loop_load_avg = TasmotaGlobal.loop_load_avg - (TasmotaGlobal.loop_load_avg / loops_per_second) + (this_cycle_ratio / loops_per_second); // Take away one loop average away and add the new one
}

void add_entities (ScreenConfig * screenConfig) {
  for (uint8_t cardIndex = 0;cardIndex<screenConfig->getNumCards();cardIndex++) {
    if (screenConfig->getCard(cardIndex) != NULL && (String(screenConfig->getCard(cardIndex)->getType()) == PAGE_TYPE_ENTITES)) {
      EntitesCardConfig * pageConfig = (EntitesCardConfig *) screenConfig->getCard(cardIndex);
      for (uint8_t entityIndex =0;entityIndex<pageConfig->getNumEntites();entityIndex++) {
        if (pageConfig->getEntityRow(entityIndex) != NULL) {
          if (String(pageConfig->getEntityRow(entityIndex)->getType()) == ENTITES_ROW_TYPE_DEFAULT) {
            DefaultRowConfig * rowConfig =  (DefaultRowConfig *) pageConfig->getEntityRow(entityIndex);
            homeAssistant.addEntity(rowConfig->getEntityId());
          } else if (String(pageConfig->getEntityRow(entityIndex)->getType()) == ENTITES_ROW_TYPE_BUTTONS) {
            ButtonsRowConfig * rowConfig = (ButtonsRowConfig *) pageConfig->getEntityRow(entityIndex);
            for (uint8_t button_index = 0;button_index<rowConfig->getNumButtons();button_index++) {
              DefaultRowConfig * buttonConfig =  (DefaultRowConfig *) rowConfig->getButton(button_index);
              homeAssistant.addEntity(buttonConfig->getEntityId());
            }
          }
        }
      }
    }
  }
}

void homeAssistantEventCallback (HomeAssistant &assistant, HomeAssistantEvent event) {
  if (event == HomeAssistantConnected) {
    globalParams.setHomeAssistantConnected(true);
    globalParams.setHomeAssistantSyncing(false);
    globalParams.setHomeAssistantConnectionFailed(false);
  } else if (event == HomeAssistantConnecting) {
    globalParams.setHomeAssistantConnected(true);
    globalParams.setHomeAssistantSyncing(true);
    globalParams.setHomeAssistantConnectionFailed(false);
  } else if (event == HomeAssistantDisconnected) {
    globalParams.setHomeAssistantConnected(false);
    globalParams.setHomeAssistantSyncing(false);
    globalParams.setHomeAssistantConnectionFailed(false);
  } else if (event == HomeAssistantSyncing) {
    globalParams.setHomeAssistantConnected(true);
    globalParams.setHomeAssistantSyncing(true);
    globalParams.setHomeAssistantConnectionFailed(false);
  } else if (event == HomeAssistantConnectionFailed) {
    globalParams.setHomeAssistantConnected(false);
    globalParams.setHomeAssistantSyncing(false);
    globalParams.setHomeAssistantConnectionFailed(true);
  } else if (event == HomeAssistantRequestEnitityList) {
    add_entities(&mainScreenConfig);
  }
}

void homeAssistantSyncMessageCallback (HomeAssistant &assistant, String entity_id, HomeAssistantEntity entity, HomeAssistantEntity  prevEntity) {
  char device_name[8];
  uint64_t chipid=ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid>>32);
  snprintf(device_name,sizeof(device_name),"had%04x",chip);
  String inputBoolean = "input_boolean."+String(device_name);
  String inputSelect = "input_select."+String(device_name);
  String inputNumber = "input_number."+String(device_name);
  if (entity_id == inputBoolean+"_dark_mode") {
    if (String(entity.state) == "on") {
      settings.setDarkMode(true);
    } else if (String(entity.state) == "off") {
      settings.setDarkMode(false);
    }
  } else if (entity_id == inputSelect+"_rotation") {
    if (String(entity.state) == "landscape") {
      settings.setScreenOrientation(ScreenLandscape);
    } else if (String(entity.state) == "portrait") {
      settings.setScreenOrientation(ScreenPortrait);
    }
  } else if (entity_id == inputBoolean+"_bottom_bar") {
    if (String(entity.state) == "on") {
      settings.setBottomBar(true);
    } else {
      settings.setBottomBar(false);
    }
  } else if (entity_id == inputNumber+"_page_num") {
    uint32_t page_num = String(entity.state).toInt();
    tft_set_page_num(page_num-1);
  } else if (entity_id == inputBoolean+"_settings_page") {
    #ifndef FIRMWARE_MINIMAL
    if (String(entity.state) == "on") {
      globalParams.setScreenPageType(ScreenPageSettings);
    } else {
      globalParams.setScreenPageType(ScreenPageMain);
    }
    #endif
  } else if (entity_id == inputBoolean+"_screen_saver") {
    if (String(entity.state) == "on") {
      settings.setScreenSaverEnable(true);
    } else {
      settings.setScreenSaverEnable(false);
    }
  }
  globalParams.setScreenRedraw(true);
}

void getSettingsScreenConfig(ScreenConfig * settingsScreenConfig) {
  uint64_t chipid=ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid>>32);
  char device_name[8];
  char entity[40];
  snprintf(device_name,sizeof(device_name),"had%04x",chip);
  EntitesCardConfig * cardConfig = new EntitesCardConfig("Display settings", "mdi:chevron-left");
  settingsScreenConfig->addCard(cardConfig);
  snprintf(entity, sizeof(entity), "input_boolean.%s_dark_mode", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Darkmode", "mdi:theme-light-dark", true, false));
  snprintf(entity, sizeof(entity), "input_boolean.%s_bottom_bar", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Bottombar", "mdi:page-layout-footer", true, false));
  snprintf(entity, sizeof(entity), "input_boolean.%s_screen_saver", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Sreensaver", "mdi:clock-digital", true, false));
  snprintf(entity, sizeof(entity), "input_boolean.%s_auto_brightness", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Auto brightness", "mdi:brightness-auto", true, false));
  snprintf(entity, sizeof(entity), "input_boolean.%s_time_format", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "24Hour format", "mdi:hours-24", true, false));

  cardConfig = new EntitesCardConfig("Conn settings", "mdi:chevron-left");
  settingsScreenConfig->addCard(cardConfig);
  snprintf(entity, sizeof(entity), "binary_sensor.%s_wifi_connected", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Wifi", "mdi:wifi", true, false));
  snprintf(entity, sizeof(entity), "sensor.%s_wifi_ssid", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Wifi ssid", "mdi:wifi", false, false));
  snprintf(entity, sizeof(entity), "binary_sensor.%s_ha_connected", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "HA", "mdi:home-assistant", true, false));
  snprintf(entity, sizeof(entity), "sensor.%s_ip", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "IP", "mdi:ip", true, false));
  snprintf(entity, sizeof(entity), "sensor.%s_uptime", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Uptime", "mdi:timer", true, false));

  cardConfig = new EntitesCardConfig("Other settings", "mdi:chevron-left");
  settingsScreenConfig->addCard(cardConfig);
  snprintf(entity, sizeof(entity), "input_boolean.%s_clear_cache", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Clear cache", "mdi:cached", true, false));
  snprintf(entity, sizeof(entity), "input_boolean.%s_restart", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Restart", "mdi:restart", true, false));
  snprintf(entity, sizeof(entity), "input_boolean.%s_reset", device_name);
  cardConfig->addEntity(new DefaultRowConfig(entity, "Factory reset", "mdi:factory", true, false));
}

void updateEntityStates(bool forceSync) {
  static bool prevDarkMode = settings.isDarkMode();
  static bool prevWifiConnected = globalParams.isWifiConnected();
  static bool prevHomeAssistantConencted = globalParams.isHomeAssistantConnected();
  static String prevIp = WiFi.localIP().toString();
  static String prevSSID = WiFi.SSID();
  static uint32_t prevUptime = globalParams.getUptime();
  static bool prevScreenSaverEnabled = settings.isScreenSaverEnabled();
  uint64_t chipid=ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid>>32);
  char device_name[8];
  char entity[40];
  snprintf(device_name,sizeof(device_name),"had%04x",chip);
  if (prevDarkMode != settings.isDarkMode() || forceSync) {
    snprintf(entity, sizeof(entity), "input_boolean.%s_dark_mode", device_name);
    homeAssistant.setState(String(entity), String(settings.isDarkMode()?S_ON:S_OFF));
    globalParams.setScreenRedraw(true);
    prevDarkMode = settings.isDarkMode();
  }
  if (prevWifiConnected != globalParams.isWifiConnected() || forceSync) {
    snprintf(entity, sizeof(entity), "binary_sensor.%s_wifi_connected", device_name);
    homeAssistant.setState(String(entity), String(globalParams.isWifiConnected()?S_ON:S_OFF));
    globalParams.setScreenRedraw(true);
    prevWifiConnected = globalParams.isWifiConnected();
  }
  if (prevHomeAssistantConencted != globalParams.isHomeAssistantConnected() || forceSync) {
    snprintf(entity, sizeof(entity), "binary_sensor.%s_ha_connected", device_name);
    homeAssistant.setState(String(entity), String(globalParams.isHomeAssistantConnected()?S_ON:S_OFF));
    globalParams.setScreenRedraw(true);
    prevHomeAssistantConencted = globalParams.isHomeAssistantConnected();
  }
  if (prevHomeAssistantConencted != globalParams.isHomeAssistantConnected() || forceSync) {
    snprintf(entity, sizeof(entity), "binary_sensor.%s_ha_connected", device_name);
    homeAssistant.setState(String(entity), String(globalParams.isHomeAssistantConnected()?S_ON:S_OFF));
    globalParams.setScreenRedraw(true);
    prevHomeAssistantConencted = globalParams.isHomeAssistantConnected();
  }
  if (prevIp != WiFi.localIP().toString() || forceSync) {
    snprintf(entity, sizeof(entity), "sensor.%s_ip", device_name);
    homeAssistant.setState(String(entity), WiFi.localIP().toString());
    globalParams.setScreenRedraw(true);
    prevIp = WiFi.localIP().toString();
  }
  if (prevSSID != WiFi.SSID() || forceSync) {
    snprintf(entity, sizeof(entity), "sensor.%s_wifi_ssid", device_name);
    homeAssistant.setState(String(entity), WiFi.SSID());
    globalParams.setScreenRedraw(true);
    prevSSID = WiFi.SSID();
  }
  if (prevUptime != globalParams.getUptime() || forceSync) {
    snprintf(entity, sizeof(entity), "sensor.%s_uptime", device_name);
    homeAssistant.setState(String(entity), String(globalParams.getUptime()));
    globalParams.setScreenRedraw(true);
    prevUptime = globalParams.getUptime();
  }
  if (prevScreenSaverEnabled != settings.isScreenSaverEnabled() || forceSync) {
    snprintf(entity, sizeof(entity), "input_boolean.%s_screen_saver", device_name);
    homeAssistant.setState(String(entity), globalParams.getUpTimeString());
    globalParams.setScreenRedraw(true);
    prevScreenSaverEnabled = settings.isScreenSaverEnabled();
  }
}