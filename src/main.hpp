#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include "ConfigHelper.hpp"

#include <WiFi.h>
#include <HTTPClient.h>


#include "TouchScreen.h"
#include "TFT_LCD.h"
#include "Touch.h"
#include "WebServer.hpp"
#include "HomeAssistant/HomeAssistantManager.hpp"

#define SCREEN_SERVER_ENABLE 1


//Function prototypes
void setup_wifi();
void wifi_loop();

void add_entities (ScreenConfig * screenConfig);
void lcdTaskCode( void * pvParameters);
void touchTaskCode( void * pvParameters);
void WifiManager_start();
#ifndef FIRMWARE_MINIMAL
void homeAssistantEventCallback (HomeAssistantClient &assistant, HomeAssistantEvent event);
void homeAssistantSyncMessageCallback (HomeAssistantClient &assistant, String entity_id, HomeAssistantEntity entity, HomeAssistantEntity  prevEntity);
void homeAssistantTaskCode(void * pvParameters);
void getSettingsScreenConfig(ScreenConfig * settingsScreenConfig);
void updateEntityStates(bool forceSync = false);
#endif
#endif