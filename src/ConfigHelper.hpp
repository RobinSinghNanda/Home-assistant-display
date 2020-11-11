#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include "ScreenConfig.hpp"
#ifndef FIRMWARE_MINIMAL
#include "HomeAssistant.hpp"
#endif
BaseRowConfig * parseRowConfiguration (JsonVariant rowVariant);
ButtonsRowConfig * parseButtonRowConfigurtion(JsonObject entityObject);
EntitesCardConfig * parseEntitiesCardConfiguration(JsonObject cardObject);
HorizontalStackCardConfig * parseHorizontalStackCardConfiguration(JsonObject cardObject);
VerticalStackCardConfig * parseVerticalStackCardConfiguration(JsonObject cardObject);
BaseCardConfig * parseCardConfiguration(JsonVariant cardVariant);
void loadScreenConfiguration(const char *filename, ScreenConfig * screenConfig);
void printScreenConfiguration(ScreenConfig * screenConfig);
#ifndef FIRMWARE_MINIMAL
void loadCachedStateConfiguration(const char *filename, HomeAssistantConfig * homeassistantConfig);
void saveCachedStateConfiguration(const char *filename, HomeAssistantConfig * homeassistantConfig);
void printCachedStateConfiguration(HomeAssistantConfig * homeassistantConfig);
#endif
ButtonsRowConfig * parseButtonRowConfigurtion(JsonObject entityObject);

#endif
