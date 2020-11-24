#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "ArduinoJson.h"
#include <Arduino.h>
#include <SPIFFS.h>
#include "ScreenConfig.hpp"
#ifndef FIRMWARE_MINIMAL
#include "HomeAssistant/HomeAssistantClient.hpp"
using namespace HomeAssistant;
#endif

void loadScreenConfiguration(const char *filename, ScreenConfig * screenConfig);
void printScreenConfiguration(ScreenConfig * screenConfig);
#ifndef FIRMWARE_MINIMAL
void loadCachedStateConfiguration(const char *filename, HomeAssistantConfig * homeassistantConfig);
void saveCachedStateConfiguration(const char *filename, HomeAssistantConfig * homeassistantConfig);
void printCachedStateConfiguration(HomeAssistantConfig * homeassistantConfig);
#endif
ButtonsRowConfig * parseButtonRowConfigurtion(JsonObject entityObject);

#endif
