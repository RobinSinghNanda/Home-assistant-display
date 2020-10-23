#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Free_Fonts.h"

#include <SPI.h>
#include "ConfigHelper.hpp"

#include <WiFi.h>
#include <HTTPClient.h>


#include "TouchScreen.h"
#include "TFT_LCD.h"
#include "Touch.h"

#define SCREEN_SERVER_ENABLE 0


//Function prototypes
void setup_wifi();
void wifi_loop();
void homeAssistantEventCallback (HomeAssistant &assistant, HomeAssistantEvent event);
void homeAssistantSyncMessageCallback (HomeAssistant &assistant, String entity_id, HomeAssistantEntity entity, HomeAssistantEntity  prevEntity);
void add_entities (ScreenConfig * screenConfig);
void lcdTaskCode( void * pvParameters);
void touchTaskCode( void * pvParameters);
#endif