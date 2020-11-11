#include "TFT_LCD.h"
#include "Settings.hpp"
#include "GlobalParams.hpp"
#include "RtcTime.hpp"
#include "Log.hpp"
TFTConfig * tftConfig ;
Canvas * screenCanvasG;
TFT_eSPI tft = TFT_eSPI(); 
SemaphoreHandle_t screenConfigMutex;

using namespace std::placeholders;

void tft_lcd_setup(ScreenConfig * mainScreenConfig, ScreenConfig * settingsScreenConfig) {
  tftConfig = (TFTConfig *) malloc(sizeof(TFTConfig));
  tftConfig->mainScreenConfig = mainScreenConfig;
  tftConfig->settingsScreenConfig = settingsScreenConfig;
  tftConfig->entityScreenConfig = new ScreenConfig();
  tftConfig->setupScreenConfig = new ScreenConfig();
  tftConfig->setupScreenConfig->addCard(new BaseCardConfig(PAGE_TYPE_WIFI_QR, "First setup", "mdi:home-assistant"));
  tftConfig->screenSaverScreenConfig = new ScreenConfig();
  tftConfig->screenSaverScreenConfig->addCard(new BaseCardConfig(PAGE_TYPE_DIGITAL_CLOCK, "", ""));
  screenCanvasG = new Canvas(&tft);
  screenConfigMutex = xSemaphoreCreateMutex();
  tft.init();
}

ScreenConfig * getScreenConfig (TFTConfig * tftConfig) {
  if (globalParams.getScreenPageType() == ScreenPageScreenSaver) {
    return tftConfig->screenSaverScreenConfig;
  } else if (globalParams.getScreenPageType() == ScreenPageSetup) {
    return tftConfig->setupScreenConfig;
  #ifndef FIRMWARE_MINIMAL
  } else if (globalParams.getScreenPageType() == ScreenPageSettings) {
    return tftConfig->settingsScreenConfig;
  } else if (globalParams.getScreenPageType() == ScreenPageEntity){
    return tftConfig->entityScreenConfig;
  } else if (globalParams.getScreenPageType() == ScreenPageMain){
    return tftConfig->mainScreenConfig;
  } else {
    return tftConfig->mainScreenConfig;
  }
  #else
  } else {
    return tftConfig->screenSaverScreenConfig;
  }
  #endif
  
}

#ifndef FIRMWARE_MINIMAL
uint32_t buildLightBodyCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  LightCardConfig * lightPageConfig = (LightCardConfig *) pageConfig;
  if (String(lightPageConfig->getEntityId()) != "") {
    String entity = lightPageConfig->getEntityId();
    LightEntityCanvas * lightEntityCanvas = new LightEntityCanvas(bodyCanvas, ID_LIGHT_SWITCH_ENTITY);
    lightEntityCanvas->onStateChange([entity](LightEntityCanvas* canvas, bool state)->bool{
        uint8_t brightness = canvas->getBrightness();
        uint32_t color = canvas->getColor();
        uint16_t color_temp = canvas->getColorTemperature();
        if (state && canvas->getBrighnessSliderEnabled()) 
          set_row_attribute(entity, "brightness", String(brightness));
        else
          set_row_attribute(entity, "brightness", "");
        if (state && canvas->getColorSliderEnabled()) {
          uint8_t red = color>>16;
          uint8_t green = color>>8;
          uint8_t blue = color;
          set_row_attribute(entity, "rgb_color", "["+String(red)+","+String(green)+","+String(blue)+"]");
        }  else {
          set_row_attribute(entity, "rgb_color", "");
        }
        if (state && canvas->getColorTemperatureSliderEnabled()) {
          set_row_attribute(entity, "color_temp", String(color_temp));
        } else {
          set_row_attribute(entity, "color_temp", "");
        }
        if (state)
          set_row_state(entity, "on");
        else
          set_row_state(entity, "off");
        return true;
      });
  } else {
    TextCanvas * bodyText = new TextCanvas(bodyCanvas, ID_LIGHT_SWITCH_ENTITY);
    bodyText->setHAlign(ALIGN_CENTER);
    bodyText->setVAlign(ALIGN_MIDDLE);
    bodyText->setText("Enitity can be null\nfor type \"light\"");
  }
  return 1;
}

bool setLightBodyCanvasState(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  LightCardConfig * lightPageConfig = (LightCardConfig *) pageConfig;
  if (String(lightPageConfig->getEntityId()) != "") {
    LightEntityCanvas * lightEntityCanvas = (LightEntityCanvas *) bodyCanvas->get(ID_LIGHT_SWITCH_ENTITY);
    String name;
    if (String(lightPageConfig->getTitle()) == "") {
      String name = get_row_attribute(lightPageConfig->getEntityId(), "friendly_name");
      if (name != "") {
        if (name.length()>15) {
          name = name.substring(0, 15);
        }
        name = name;
      } else {
        name = lightPageConfig->getEntityId();
      }
    } else {
      name = lightPageConfig->getTitle();
    }
    lightEntityCanvas->setName(name);
    lightEntityCanvas->setIconPath("mdi:lightbulb");
    String state = get_row_state(lightPageConfig->getEntityId());
    String entity = lightPageConfig->getEntityId();
    String brightness = get_row_attribute(entity, "brightness");
    String whiteValue = get_row_attribute(entity, "white_value");
    String rgbColor = get_row_attribute(entity, "rgb_color");
    String colorTemp = get_row_attribute(entity, "color_temp");
    String minMireds = get_row_attribute(lightPageConfig->getEntityId(), "min_mireds");
    String maxMireds = get_row_attribute(lightPageConfig->getEntityId(), "max_mireds");
    
    if (state == "on") {
      lightEntityCanvas->setState(true);
    } else if (state == "off") {
      lightEntityCanvas->setState(false);
    } else {
      lightEntityCanvas->setDisabled(true);
    }
    
    if (brightness == "") {
      lightEntityCanvas->setBrighnessSliderEnabled(false);
    } else {
      lightEntityCanvas->setBrighnessSliderEnabled(true);
      lightEntityCanvas->setBrightness(brightness.toInt());
    }
    
    if (whiteValue == "") {
      lightEntityCanvas->setWhiteValueSliderEnabled(false);
    } else {
      lightEntityCanvas->setWhiteValueSliderEnabled(true);
      lightEntityCanvas->setWhiteValue(whiteValue.toInt());
    }
   
    if (rgbColor == "") {
      lightEntityCanvas->setColorSliderEnabled(false);
    } else {
      lightEntityCanvas->setColorSliderEnabled(true);
      uint8_t red, blue, green;
      if (sscanf(rgbColor.c_str(), "[%hhu,%hhu,%hhu]", &red, &green, &blue) != EOF) {
        lightEntityCanvas->setColor(red, green, blue);
      }
    }
    if (colorTemp == "") {
      lightEntityCanvas->setColorTemperatureSliderEnabled(false);
    } else {
      lightEntityCanvas->setMinColorTemperature(minMireds.toInt());
      lightEntityCanvas->setMaxColorTemperature(maxMireds.toInt());
      lightEntityCanvas->setColorTemperatureSliderEnabled(true);
      lightEntityCanvas->setColorTemperature(colorTemp.toInt());
    }
  }
  return 1;
}

uint32_t buildSwitchBodyCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  SwitchCardConfig * switchPageConfig = (SwitchCardConfig *) pageConfig;
  if (String(switchPageConfig->getEntityId()) != "") {
    String entity = switchPageConfig->getEntityId();
    SwitchEntityCanvas * switchEntityCanvas = new SwitchEntityCanvas(bodyCanvas, ID_SWITCH_ENTITY);
    switchEntityCanvas->onStateChange([entity](SwitchCanvas* canvas, bool state)->bool{
        if (state)
          set_row_state(entity, "on");
        else
          set_row_state(entity, "off");
        return true;
      });
  } else {
    TextCanvas * bodyText = new TextCanvas(bodyCanvas, ID_SWITCH_ENTITY);
    bodyText->setHAlign(ALIGN_CENTER);
    bodyText->setVAlign(ALIGN_MIDDLE);
    bodyText->setText("Enitity can be null\nfor type \"switch\"");
  }
  return 1;
}

bool setSwitchBodyCanvasState(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  SwitchCardConfig * switchPageConfig = (SwitchCardConfig *) pageConfig;
  if (String(switchPageConfig->getEntityId()) != "") {
    SwitchEntityCanvas * switchEntityCanvas = (SwitchEntityCanvas *) bodyCanvas->get(ID_SWITCH_ENTITY);
    String name;
    if (String(switchPageConfig->getTitle()) == "") {
      String name = get_row_attribute(switchPageConfig->getEntityId(), "friendly_name");
      if (name != "") {
        name = name;
      } else {
        name = switchPageConfig->getEntityId();
      }
    } else {
      name = switchPageConfig->getTitle();
    }
    switchEntityCanvas->setName(name);
    switchEntityCanvas->setIconPath("mdi:lightning-bolt");
    String state = get_row_state(switchPageConfig->getEntityId());
    if (state == "on") {
      switchEntityCanvas->setState(true);
    } else if (state == "off") {
      switchEntityCanvas->setState(false);
    } else {
      switchEntityCanvas->setDisabled(true);
    }
  }
  return 1;
}

uint32_t buildFanBodyCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  FanCardConfig * fanPageConfig = (FanCardConfig *) pageConfig;
  if (String(fanPageConfig->getEntityId()) != "") {
    String entity = fanPageConfig->getEntityId();
    FanEntityCanvas * fanEntityCanvas = new FanEntityCanvas(bodyCanvas, ID_FAN_SWITCH_ENTITY);
    fanEntityCanvas->onStateChange([entity](FanEntityCanvas* canvas, bool state)->bool {
        uint16_t fanSpeed = canvas->getFanSpeed();
        if (state) {
          if (fanSpeed == 1) {
            set_row_attribute(entity, "speed", "low");
          } else if (fanSpeed == 2) {
            set_row_attribute(entity, "speed", "medium");
          } else if (fanSpeed == 3) {
            set_row_attribute(entity, "speed", "high");
          }
        }
        if (state)
          set_row_state(entity, "on");
        else
          set_row_state(entity, "off");
        return true;
      });
  } else {
    TextCanvas * bodyText = new TextCanvas(bodyCanvas, ID_FAN_SWITCH_ENTITY);
    bodyText->setHAlign(ALIGN_CENTER);
    bodyText->setVAlign(ALIGN_MIDDLE);
    bodyText->setText("Enitity can be null\nfor type \"fan\"");
  }
  return 1;
}
#endif

uint32_t buildDigitalClockCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  DigitalClockCanvas * digitalClockCanvas = new DigitalClockCanvas(bodyCanvas, 0);
  digitalClockCanvas->setHAlign(ALIGN_CENTER);
  digitalClockCanvas->setVAlign(ALIGN_MIDDLE);
  digitalClockCanvas->setMargin(10);
  digitalClockCanvas->onTouch([](Canvas*, TouchEvent, TouchEventData)->bool{
      globalParams.setScreenPageType(ScreenPageMain);
      return true;
    });
  return 1;
}

uint32_t buildWifiQrCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  QrCodeCanvas * qrCodeCanvas = new QrCodeCanvas(bodyCanvas, 0);
  qrCodeCanvas->setHAlign(ALIGN_CENTER);
  qrCodeCanvas->setVAlign(ALIGN_MIDDLE);
  qrCodeCanvas->setMargin(0);
  qrCodeCanvas->setPath("wifi");
  return 1;
}

bool setDigitalClockCanvasState(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  DigitalClockCanvas * digitalClockCanvas = (DigitalClockCanvas *) bodyCanvas->get(0);
  TIME_T ut;
  BreakTime(LocalTime(), ut);
  LcdTheme * theme = settings.getLcdSelectedTheme();
  digitalClockCanvas->setMinute(ut.minute);
  digitalClockCanvas->setHour(ut.hour);
  digitalClockCanvas->setSecond(ut.second);
  digitalClockCanvas->setTimeFormat(TimeFormat12Hour);
  digitalClockCanvas->setDisplaySecond(false);
  digitalClockCanvas->setDisplayDate(true);
  digitalClockCanvas->setBlink(true);
  digitalClockCanvas->setValid(ut.valid);
  char day[4];
  char dateString[13];
  day[3] = '\0';
  uint8_t index=ut.day_of_week*3;
  memcpy(day, (void *)(kDayNamesEngligh+index), 3);
  snprintf(dateString, sizeof(dateString)-1, "%s, %s %d", day, ut.name_of_month, ut.day_of_month);
  digitalClockCanvas->setDate(dateString);
  digitalClockCanvas->setBgColor(theme->colorScreenSaverBackground.getRGB656());
  digitalClockCanvas->setFgColor(theme->colorScreenSaverText.getRGB656());
  return true;
}

bool setAnalogClockCanvasState(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  // DigitalClockCanvas * digitalClockCanvas = (DigitalClockCanvas *) bodyCanvas->get(0);
  // digitalClockCanvas->setMinutes(rtcTime.minute);
  // digitalClockCanvas->setHour(rtcTime.hour);
  // digitalClockCanvas->setSeperatorVisible(rtcTime.second&0x01);
  // return true;
  return true;
}

#ifndef FIRMWARE_MINIMAL
bool setFanBodyCanvasState(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  FanCardConfig * fanPageConfig = (FanCardConfig *) pageConfig;
  if (String(fanPageConfig->getEntityId()) != "") {
    FanEntityCanvas * fanEntityCanvas = (FanEntityCanvas *) bodyCanvas->get(ID_FAN_SWITCH_ENTITY);
    String name;
    if (String(fanPageConfig->getTitle()) == "") {
      String name = get_row_attribute(fanPageConfig->getEntityId(), "friendly_name");
      if (name != "") {
        if (name.length()>15) {
          name = name.substring(0, 15);
        }
        name = name;
      } else {
        name = fanPageConfig->getEntityId();
      }
    } else {
      name = fanPageConfig->getTitle();
    }
    fanEntityCanvas->setName(name);
    fanEntityCanvas->setIconPath("mdi:fan");
    String state = get_row_state(fanPageConfig->getEntityId());
    String entity = fanPageConfig->getEntityId();
    String fanSpeed = get_row_attribute(entity, "speed");
    
    if (state == "on") {
      fanEntityCanvas->setState(true);
    } else if (state == "off") {
      fanEntityCanvas->setState(false);
    } else {
      fanEntityCanvas->setDisabled(true);
    }
    
    if (fanSpeed == "") {
      fanEntityCanvas->setFanSpeedSliderEnabled(false);
    } else {
      fanEntityCanvas->setFanSpeedSliderEnabled(true);
      if (fanSpeed == "off") {
        fanEntityCanvas->setFanSpeed(0);
      } else if (fanSpeed == "low") {
        fanEntityCanvas->setFanSpeed(1);
      } else if (fanSpeed == "medium") {
        fanEntityCanvas->setFanSpeed(2);
      } else if (fanSpeed == "high") {
        fanEntityCanvas->setFanSpeed(3);
      } else {
        fanEntityCanvas->setFanSpeed(0);
      }
    }
  }
  return 1;
}

bool setEntitiesBodyCanvasState (TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  EntitesCardConfig * rowPageConfig = (EntitesCardConfig *) pageConfig;
  LcdTheme * theme = settings.getLcdSelectedTheme();
  for (uint8_t i=0;i<rowPageConfig->getNumEntites();i++) {
    BaseEntityRowCanvas * baseEntityCanvas = (BaseEntityRowCanvas *) (*bodyCanvas)[i];
    if (baseEntityCanvas == NULL)
      continue;
    String entity_id = "";
    BaseRowConfig * row = rowPageConfig->getEntityRow(i);
    if (row == NULL || String(row->getType()) == ENTITES_ROW_TYPE_INVALID) {
      continue;
    }
    if ((String(row->getType()) == ENTITES_ROW_TYPE_DEFAULT) ||
      (String(row->getType()) == ENTITES_ROW_TYPE_FAN) ||
      (String(row->getType()) == ENTITES_ROW_TYPE_BUTTONS)) {
      if (String(row->getType()) == ENTITES_ROW_TYPE_DEFAULT) {
        DefaultRowConfig * default_row = (DefaultRowConfig *)row;
        if (String(default_row->getName()) == "") {
          String name = get_row_attribute(default_row->getEntityId(), "friendly_name");
          if (name != "") {
            if (name.length()>15) {
              name = name.substring(0, 15);
            }
            baseEntityCanvas->setName(name);
          } else {
            baseEntityCanvas->setName(default_row->getEntityId());
          }
        } else {
          baseEntityCanvas->setName(default_row->getName());
        }
        String entity = default_row->getEntityId();
        String domain = entity.substring(0, entity.indexOf("."));
        String state = get_row_state(default_row->getEntityId());
        if ((domain == "sensor")|| (domain == "binary_sensor")) {
          //stateText.text = state;
          //stateText.color = (tftConfig->dark_mode)?TFT_WHITE:TFT_BLACK;
          //row_icon.color = convert2rgb565(0x44739e);
        }
        if (String(default_row->getIcon()) != "") {
          baseEntityCanvas->setIconPath(default_row->getIcon());
        } else {
          String icon = get_row_attribute(default_row->getEntityId(), "icon");
          if (icon != "") {
            baseEntityCanvas->setIconPath(icon);
          } else {
            if (domain == "light") {
              baseEntityCanvas->setIconPath("mdi:lightbulb");
            } else if (domain == "fan") {
              baseEntityCanvas->setIconPath("mdi:fan");
            } else if (domain == "climate") {
              baseEntityCanvas->setIconPath("mdi:hvac");
            } else if (domain == "switch") {
              baseEntityCanvas->setIconPath("mdi:lightning-bolt");
            } else if (domain == "sensor") {
              String device_class = get_row_attribute(default_row->getEntityId(), "device_class");
              if (device_class == "temperature") {
                baseEntityCanvas->setIconPath("mdi:temperature-celsius");
              } else if (device_class == "humidity") {
                baseEntityCanvas->setIconPath("mdi:water-percent");
              } else if (device_class == "pressure") {
                baseEntityCanvas->setIconPath("mdi:water-percent");
              } else if (device_class == "illuminance") {
                baseEntityCanvas->setIconPath("mdi:brightness-5");
              } else {
                baseEntityCanvas->setIconPath("");
              }
            } else if (domain == "binary_sensor") {
              String device_class = get_row_attribute(default_row->getEntityId(), "device_class");
              if (device_class == "occupancy") {
                baseEntityCanvas->setIconPath("mdi:motion-sensor");
              } else {
                baseEntityCanvas->setIconPath("");
              }
            } else {
              baseEntityCanvas->setIconPath("");
            }
          }
        }
        if (domain == "switch" || domain == "climate" || domain == "media_player" || domain == "input_boolean") {
          SwitchEntityRowCanvas * switchEntityCanvas = (SwitchEntityRowCanvas *) baseEntityCanvas;
          switchEntityCanvas->setSecondaryColor(theme->colorSwitchSecondary.getRGB656());
          switchEntityCanvas->setSurfaceColor(theme->colorSwitchSurface.getRGB656());
          switchEntityCanvas->setOnSurfaceColor(theme->colorSwitchOnSurface.getRGB656());
          if (state == "on") {
            switchEntityCanvas->setState(true);
          } else if (state == "off") {
            switchEntityCanvas->setState(false);
          } else {
            switchEntityCanvas->setDisabled(true);
          }
        } else if (domain == "sensor" || domain == "binary_sensor") {
          SensorEntityRowCanvas * sensorEntityRowCanvas = (SensorEntityRowCanvas *) baseEntityCanvas;
          sensorEntityRowCanvas->setState(state);
        } else if (domain == "fan") {
          FanEntityRowCanvas * fanEntityCanvas = (FanEntityRowCanvas *) baseEntityCanvas;
          fanEntityCanvas->setSecondaryColor(theme->colorSwitchSecondary.getRGB656());
          fanEntityCanvas->setSurfaceColor(theme->colorSwitchSurface.getRGB656());
          fanEntityCanvas->setOnSurfaceColor(theme->colorSwitchOnSurface.getRGB656());
          String speed = get_row_attribute(entity, "speed");
          if (speed == "low") {
            fanEntityCanvas->setSpeed(1);
          } else if (speed == "medium") {
            fanEntityCanvas->setSpeed(2);
          } else if (speed == "high") {
            fanEntityCanvas->setSpeed(3);  
          } else {
            fanEntityCanvas->setSpeed(0);
          }
          if (state == "on") {
            fanEntityCanvas->setState(true);
          } else if (state == "off") {
            fanEntityCanvas->setState(false);
          } else {
            fanEntityCanvas->setDisabled(true);
          }
        } else if (domain == "light") {
          LightEntityRowCanvas * lightEntityCanvas = (LightEntityRowCanvas *) baseEntityCanvas;
          lightEntityCanvas->setSecondaryColor(theme->colorSwitchSecondary.getRGB656());
          lightEntityCanvas->setSurfaceColor(theme->colorSwitchSurface.getRGB656());
          lightEntityCanvas->setOnSurfaceColor(theme->colorSwitchOnSurface.getRGB656());
          String brightnesss = get_row_attribute(entity, "brightness");
          if (brightnesss != "") {
            lightEntityCanvas->setBrightness(brightnesss.toInt());
          }
          if (state == "on") {
            lightEntityCanvas->setState(true);
          } else if (state == "off") {
            lightEntityCanvas->setState(false);
          } else {
            lightEntityCanvas->setDisabled(true);
          }
        } else {
          SwitchEntityRowCanvas * switchEntityCanvas = (SwitchEntityRowCanvas *) baseEntityCanvas;
          switchEntityCanvas->setSecondaryColor(theme->colorSwitchSecondary.getRGB656());
          switchEntityCanvas->setSurfaceColor(theme->colorSwitchSurface.getRGB656());
          switchEntityCanvas->setOnSurfaceColor(theme->colorSwitchOnSurface.getRGB656());
          if (state == "on") {
            switchEntityCanvas->setState(true);
          } else if (state == "off") {
            switchEntityCanvas->setState(false);
          } else {
            switchEntityCanvas->setDisabled(true);
          }
        }
      } else if (String(row->getType()) == ENTITES_ROW_TYPE_BUTTONS) {
        ButtonsRowConfig * buttonsRow = (ButtonsRowConfig *)row;
        ButtonsEntityRowCanvas * buttonsEntityRowCanvas = (ButtonsEntityRowCanvas *) baseEntityCanvas;
        for (uint8_t buttonIndex = 0;buttonIndex <buttonsRow->getNumButtons();buttonIndex++) {
          DefaultRowConfig * buttonConfig = (DefaultRowConfig *) buttonsRow->getButton(buttonIndex);
          if (String(buttonConfig->getIcon()) != "") {
            buttonsEntityRowCanvas->setIconPath(buttonConfig->getIcon(), buttonIndex);
          } else {
            #ifndef FIRMWARE_MINIMAL
            String icon = homeAssistant.get_attribute(buttonConfig->getEntityId(), "icon");
            #else 
            String icon = "";
            #endif
            if (icon != "") {
              buttonsEntityRowCanvas->setIconPath(icon, buttonIndex);
            }
          }
        }
      }
    }
  }
  return 1;
}

uint32_t buildEntitesBodyCanvas (TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  EntitesCardConfig * rowPageConfig = (EntitesCardConfig *) pageConfig;
  uint16_t row_draw_y = bodyCanvas->getY();
  for (uint8_t i=0;i<rowPageConfig->getNumEntites();i++) {
    String entity_id = "";
    BaseRowConfig * row = rowPageConfig->getEntityRow(i);
    if (row == NULL || String(row->getType()) == ENTITES_ROW_TYPE_INVALID) {
      continue;
    }
    BaseEntityRowCanvas * rowCanvas;
    if (String(row->getType()) == ENTITES_ROW_TYPE_DEFAULT) {
        DefaultRowConfig * default_row = (DefaultRowConfig *)row;
        String entity = default_row->getEntityId();
        String domain = entity.substring(0, entity.indexOf("."));
        if (domain == "fan") {
          rowCanvas = new FanEntityRowCanvas(bodyCanvas, i);
          FanEntityRowCanvas * fanCanvas = (FanEntityRowCanvas *) rowCanvas;
          fanCanvas->onStateChange([default_row](FanEntityRowCanvas* canvas, bool state, uint16_t speed)->bool{
              if (speed == 1) {
                set_row_attribute(default_row->getEntityId(), "speed", "low");
              } else if (speed == 2) {
                set_row_attribute(default_row->getEntityId(), "speed", "medium");
              } else if (speed == 3) {
                set_row_attribute(default_row->getEntityId(), "speed", "high");
              }
              if (state) {
                set_row_state(default_row->getEntityId(), "on");
              } else {
                set_row_state(default_row->getEntityId(), "off");
              }
              return true;
            });
        } else if (domain == "light") {
          rowCanvas = new LightEntityRowCanvas(bodyCanvas, i);
          LightEntityRowCanvas * lightCanvas = (LightEntityRowCanvas *) rowCanvas;
          lightCanvas->onStateChange([default_row](LightEntityRowCanvas* canvas, bool state, uint8_t brightness)->bool{
              set_row_attribute(default_row->getEntityId(), "brightness", String(brightness));
              if (state) {
                set_row_state(default_row->getEntityId(), "on");
              } else {
                set_row_state(default_row->getEntityId(), "off");
              }
              return true;
            });
        } else if (domain == "sensor" || domain == "binary_sensor") {
          rowCanvas = new SensorEntityRowCanvas(bodyCanvas, i);
        } else {
          rowCanvas = new SwitchEntityRowCanvas(bodyCanvas, i);
        }
    } else if (String(row->getType()) == ENTITES_ROW_TYPE_BUTTONS) {
      ButtonsRowConfig * buttons_row = (ButtonsRowConfig *)row;
      rowCanvas = new ButtonsEntityRowCanvas(bodyCanvas, i, buttons_row->getNumButtons());
      ButtonsEntityRowCanvas * buttonsEntityRowCanvas = (ButtonsEntityRowCanvas *) rowCanvas;
      for (uint8_t buttonIndex =0;buttonIndex<buttons_row->getNumButtons();buttonIndex++) {
        DefaultRowConfig * buttonConfig = (DefaultRowConfig *) buttons_row->getButton(buttonIndex);
        String entity = buttonConfig->getEntityId();
        buttonsEntityRowCanvas->onTouch([entity](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
            if (isEventLosely(event, TouchActionTapped)) {
              set_row_state(entity, "on");
              return true;
            }
            return false;
          }, buttonIndex); 
      }
    } else {
      rowCanvas = new SwitchEntityRowCanvas(bodyCanvas, i);
    }
    rowCanvas->setHeight(ENTITIES_ROW_HEIGHT);
    rowCanvas->setY(row_draw_y);
    if (row == NULL || String(row->getType()) == ENTITES_ROW_TYPE_INVALID) {
      continue;
    }
    if ((String(row->getType()) == ENTITES_ROW_TYPE_DEFAULT) ||
      (String(row->getType()) == ENTITES_ROW_TYPE_FAN) ||
      (String(row->getType()) == ENTITES_ROW_TYPE_BUTTONS)) {
      if (String(row->getType()) == ENTITES_ROW_TYPE_DEFAULT) {
        DefaultRowConfig * default_row = (DefaultRowConfig *)row;
        String entity = default_row->getEntityId();
        String domain = entity.substring(0, entity.indexOf("."));
        if (domain == "light" || domain == "switch" || domain == "climate" || domain == "media_player" || domain == "input_boolean") {
          SwitchEntityRowCanvas * switchEntityRowCanvas = (SwitchEntityRowCanvas *) rowCanvas;
          switchEntityRowCanvas->setStateColor(default_row->getStateColor());
          switchEntityRowCanvas->onStateChange([default_row](SwitchCanvas* canvas, bool state)->bool{
            if (state) {
              set_row_state(default_row->getEntityId(), "on");
            } else {
              set_row_state(default_row->getEntityId(), "off");
            }
            return true;
          });
        }
        if (domain == "light") {
          SwitchEntityRowCanvas * switchEntityRowCanvas = (SwitchEntityRowCanvas *) rowCanvas;
          auto touchCallback = [tftConfig, entity, default_row](Canvas*, TouchEvent event, TouchEventData)->bool{
              if (isEvent(event, TouchActionTapped)) {
                tftConfig->entityScreenConfig->clearCards();
                String title = default_row->getName();
                title = (title == "")?"Fan":title;
                tftConfig->entityScreenConfig->addCard(new LightCardConfig(
                    entity.c_str(), title.c_str(), "mdi:chevron-left", 1
                  ));
                globalParams.setScreenPageType(ScreenPageEntity);
                return true;
              }
              return false;
            };
          switchEntityRowCanvas->onNameTouch(touchCallback);
          switchEntityRowCanvas->onIconTouch(touchCallback);
        } else if (domain == "fan") {
          SwitchEntityRowCanvas * switchEntityRowCanvas = (SwitchEntityRowCanvas *) rowCanvas;
          auto touchCallback = [tftConfig, entity, default_row](Canvas*, TouchEvent event, TouchEventData)->bool{
              if (isEvent(event, TouchActionTapped)) {
                tftConfig->entityScreenConfig->clearCards();
                String title = default_row->getName();
                title = (title == "")?"Fan":title;
                tftConfig->entityScreenConfig->addCard(new FanCardConfig(
                    entity.c_str(), title.c_str(), "mdi:chevron-left", 1
                  ));
                globalParams.setScreenPageType(ScreenPageEntity);
                return true;
              }
              return false;
            };
          switchEntityRowCanvas->onNameTouch(touchCallback);
          switchEntityRowCanvas->onIconTouch(touchCallback);
        } else if ((domain == "switch") || (domain == "media_player") || (domain == "climate")) {
          SwitchEntityRowCanvas * switchEntityRowCanvas = (SwitchEntityRowCanvas *) rowCanvas;
          auto touchCallback = [tftConfig, entity, default_row](Canvas*, TouchEvent event, TouchEventData)->bool{
              if (isEvent(event, TouchActionTapped)) {
                tftConfig->entityScreenConfig->clearCards();
                String title = default_row->getName();
                title = (title == "")?"Switch":title;
                tftConfig->entityScreenConfig->addCard(new SwitchCardConfig(
                    entity.c_str(), "Switch", "mdi:chevron-left", default_row->getIcon(), 1
                  ));
                globalParams.setScreenPageType(ScreenPageEntity);
                return true;
              }
              return false;
            };
          switchEntityRowCanvas->onNameTouch(touchCallback);
          switchEntityRowCanvas->onIconTouch(touchCallback);
        }
      } else if (String(row->getType()) == ENTITES_ROW_TYPE_FAN) {
        //DefaultRowConfig * fan_row = (DefaultRowConfig *) row;
      } else if (String(row->getType()) == ENTITES_ROW_TYPE_BUTTONS) {

      }
    }
    row_draw_y += ENTITIES_ROW_HEIGHT;
  }
  return 1;
}
#endif

uint32_t setBodyCanvasState (TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Log::log(LOG_LEVEL_INFO, D_LOG_LCD "No cards exists");
    return 0;
  }
  if (screenConfig->getNumCards() <= globalParams.getScreenPageNumber()) {
    globalParams.setScreenPageNumber(screenConfig->getNumCards()-1);
  }
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  if (pageConfig == NULL) {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Card is not defined");
    return 0;
  }
  LcdTheme * theme = settings.getLcdSelectedTheme();
  bodyCanvas->setFgColor(theme->colorBodyText.getRGB656());
  bodyCanvas->setBgColor(theme->colorBodyBackground.getRGB656());
  bodyCanvas->setDrawBackgroundEnable(true);
  if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_DIGITAL_CLOCK)) {
    return setDigitalClockCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_ANALOG_CLOCK)) {
    return setAnalogClockCanvasState(tftConfig, bodyCanvas);
  #ifndef FIRMWARE_MINIMAL
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_ENTITES)) {
    return setEntitiesBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_SWITCH)) {
    return setSwitchBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_LIGHT)) {
    return setLightBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_FAN)) {
    return setFanBodyCanvasState(tftConfig, bodyCanvas);
  #endif
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_WIFI_QR) && globalParams.getScreenPageType() == ScreenPageSetup) {
    return false;
  } else {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Unknown card type %s", pageConfig->getType());
    return 0;
  }
  return 0;
}

uint32_t buildBodyCanvas (TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Log::log(LOG_LEVEL_INFO, D_LOG_LCD "No cards exists");
    return 0;
  }
  if (screenConfig->getNumCards() <= globalParams.getScreenPageNumber()) {
    globalParams.setScreenPageNumber(screenConfig->getNumCards()-1);
  }
  BaseCardConfig * pageConfig = screenConfig->getCard(globalParams.getScreenPageNumber());
  if (pageConfig == NULL) {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Card is not defined");
    return 0;
  }
  if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_DIGITAL_CLOCK)) {
    return buildDigitalClockCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_ANALOG_CLOCK)) {
    return 0;
  #ifndef FIRMWARE_MINIMAL
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_ENTITES)) {
    return buildEntitesBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_SWITCH)) {
    return buildSwitchBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_LIGHT)) {
    return buildLightBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_FAN)) {
    return buildFanBodyCanvas(tftConfig, bodyCanvas);
  #endif
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_WIFI_QR) && globalParams.getScreenPageType() == ScreenPageSetup) {
    return buildWifiQrCanvas(tftConfig, bodyCanvas);
  } else {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Unknown card type %s", pageConfig->getType());
    return 0;
  }
}

uint32_t drawScreen() {
  if (globalParams.getScreenRebuild() || globalParams.getScreenRedraw()) {
    Canvas * _screenCanvas = screenCanvasG;
    if(xSemaphoreTake(screenConfigMutex, portMAX_DELAY) == pdTRUE) {
      if (globalParams.getScreenRebuild()) {
        _screenCanvas = new Canvas(&tft);
        if ((settings.getScreenOrientation() == ScreenPortrait) || (settings.getScreenOrientation() == ScreenPortraitInverted)) {
          _screenCanvas->setWidth(LCD_HEIGHT);
          _screenCanvas->setHeight(LCD_WIDTH);
        } else {
          _screenCanvas->setWidth(LCD_WIDTH);
          _screenCanvas->setHeight(LCD_HEIGHT);
        }
        buildScreenCanvas(_screenCanvas);
        _screenCanvas->setDarkMode(settings.isDarkMode());
      }
      setScreenCanvasState(_screenCanvas);
      _screenCanvas->redraw();
      if (globalParams.getScreenRebuild()) {
        
          Canvas * prevScreenCanvas = screenCanvasG;
          screenCanvasG = _screenCanvas;
          delete prevScreenCanvas;
      }
      xSemaphoreGive(screenConfigMutex);
      globalParams.setScreenRebuild(false);
      globalParams.setScreenRedraw(false);
    }
  }
  return 1;
}

uint32_t setScreenCanvasState(Canvas * screenCanvas) {
  Canvas * headerCanvas = (*screenCanvas)[ID_HEADER];
  if (headerCanvas != NULL) {
    setHeaderCanvasState(tftConfig, headerCanvas);
  }
  Canvas * bodyCanvas =  (*screenCanvas)[ID_BODY];
  if (bodyCanvas != NULL) {
    setBodyCanvasState(tftConfig, bodyCanvas);
  }
  return 0;
}

uint32_t buildScreenCanvas(Canvas * screenCanvas) {
  static ScreenOrientation prevOrientation = ScreenOrientationInvalid;
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Log::log(LOG_LEVEL_INFO, D_LOG_LCD "No cards exists");
    return 0;
  }
  if (globalParams.getScreenPageNumber() >= screenConfig->getNumCards()) {
    globalParams.setScreenPageNumber(screenConfig->getNumCards()-1);
  }
  if (prevOrientation != settings.getScreenOrientation()) {
    tft.setRotation(settings.getScreenOrientation());
    prevOrientation = settings.getScreenOrientation();
  }
  Canvas * headerCanvas = new Canvas(screenCanvas, ID_HEADER);
  headerCanvas->setHeight(HEADER_HEIGHT);
  headerCanvas->setY(screenCanvas->getY() + (settings.isBottomBar())?(screenCanvas->getHeight() - HEADER_HEIGHT):0);
  buildHeaderCanvas(tftConfig, headerCanvas);
  SwipeCanvas * bodyCanvas = new SwipeCanvas(screenCanvas, ID_BODY);
  bodyCanvas->setHeight(screenCanvas->getHeight()-HEADER_HEIGHT);
  bodyCanvas->setY(screenCanvas->getY() + ((settings.isBottomBar())?0:HEADER_HEIGHT));
  buildBodyCanvas(tftConfig, bodyCanvas);
  bodyCanvas->onSwipeComplete([](SwipeCanvas* canvas, SwipeEventData eventData)-> bool {
      uint8_t prevPageNumber = 0xFF;
      int16_t delta = 0;
      if (eventData.swipeDirection == SwipeInvalid) {
        Canvas * headerCanvas = (*screenCanvasG)[ID_HEADER];
        if (headerCanvas == NULL) {
          return false;
        }
        PageSelectorCanvas * pageSelectorCanvas = (PageSelectorCanvas *) (*headerCanvas)[ID_HEADER_PAGE_SELECTOR];
        if (pageSelectorCanvas == NULL)
          return false;
        pageSelectorCanvas->setTempPosition(pageSelectorCanvas->getSelected());
        pageSelectorCanvas->redraw();
        return true;
      }
      if (getHorizontalSwipe(eventData.swipeDirection) == SwipeLeft) {
        delta = 1;
      } else if (getHorizontalSwipe(eventData.swipeDirection) == SwipeRight) {
        delta = -1;
      }
      ScreenConfig * screenConfig = getScreenConfig(tftConfig);
      int16_t temp_page_num = globalParams.getScreenPageNumber() + delta;
      if (temp_page_num >= screenConfig->getNumCards()) {
        temp_page_num = 0;
      } else if (temp_page_num < 0) {
        temp_page_num = screenConfig->getNumCards()-1;
      }
      if (prevPageNumber != temp_page_num) {
        globalParams.setScreenPageNumber(temp_page_num);
        prevPageNumber = temp_page_num;
      }
      return true;
    });
  bodyCanvas->onSwipe([](SwipeCanvas* canvas, SwipeEventData eventData)-> bool {
      uint16_t swipeThreshold = canvas->getSwipeThreshold();
      Canvas * headerCanvas = (*screenCanvasG)[ID_HEADER];
      if (headerCanvas == NULL) {
        return false;
      }
      PageSelectorCanvas * pageSelectorCanvas = (PageSelectorCanvas *) (*headerCanvas)[ID_HEADER_PAGE_SELECTOR];
      if (pageSelectorCanvas == NULL)
        return false;
      int16_t deltaX = eventData.startX - eventData.endX;
      if (deltaX > swipeThreshold*2) {
        deltaX = swipeThreshold*2;
      } else if (deltaX < -swipeThreshold*2) {
        deltaX = -swipeThreshold*2;
      }
      double value = (0.5*deltaX/swipeThreshold) + pageSelectorCanvas->getSelected();
      if (value < -0.5) {
        value += pageSelectorCanvas->getNumPages();
      } else if (value > pageSelectorCanvas->getNumPages() - 0.5) {
        value -= pageSelectorCanvas->getNumPages();
      }
      pageSelectorCanvas->setTempPosition(value);
      pageSelectorCanvas->redraw();
      return true;
    });
  return 0;
}

uint16_t wifiSyncState = 0;
uint16_t prevIconChangeTime = 0;

void setHeaderCanvasState (TFTConfig * tftConfig, Canvas * headerCanvas) {
  Canvas * wifiCanvas = (*headerCanvas)[ID_HEADER_WIFI_ICON];
  Canvas * iconCanvas = (*headerCanvas)[ID_HEADER_ICON];
  Canvas * settingsIconCanvas = (*headerCanvas)[ID_HEADER_SETTINGS_ICON];
  LcdTheme * theme = settings.getLcdSelectedTheme();
  if (wifiCanvas != NULL) {
    ImageCanvas * wifiImageCanvas = (ImageCanvas *) wifiCanvas;
    String wifiIcon = "mdi:wifi-alert";
    if (globalParams.isWifiApMode()) {
      wifiIcon = "mdi:access-point";
    } else if (globalParams.isHomeAssistantConnectionFailed()) {
      wifiIcon = "mdi:wifi-alert";
    } else if (globalParams.isHomeAssistantSyncing()) {
      wifiIcon = "mdi:wifi-arrow-up-down";
    } else if (globalParams.isHomeAssistantConnected()) {
      wifiIcon = "mdi:wifi-lock";
    } else if (globalParams.isWifiUp()) {
      wifiIcon = "mdi:wifi";
    } else {
      wifiIcon = "mdi:wifi-off";
    }
    wifiImageCanvas->setPath(wifiIcon);
    if (globalParams.getScreenPageType() == ScreenPageScreenSaver) {
      wifiImageCanvas->setFgColor(theme->colorScreenSaverHeaderIcon.getRGB656());
    } else {
      wifiImageCanvas->setFgColor(theme->colorHeaderIcon.getRGB656());
    }
  }
  if (iconCanvas != nullptr) {
    #ifndef FIRMWARE_MINIMAL
    iconCanvas->setFgColor(theme->colorHeaderIcon.getRGB656());
    #else 
    iconCanvas->setFgColor(convert2rgb565(0xFF0000));
    #endif
  }
  if (settingsIconCanvas != nullptr) {
    settingsIconCanvas->setFgColor(theme->colorHeaderIcon.getRGB656());
  }
  if (globalParams.getScreenPageType() == ScreenPageScreenSaver) {
    headerCanvas->setBgColor(theme->colorScreenSaverBackground.getRGB656());
  } else {
    headerCanvas->setBgColor(theme->colorHeaderBackground.getRGB656());
  }
  headerCanvas->setFgColor(theme->colorHeaderText.getRGB656());
  headerCanvas->setDrawBackgroundEnable(true);
}

void buildHeaderCanvas (TFTConfig * tftConfig, Canvas * headerCanvas) {
  #ifndef FIRMWARE_MINIMAL
  uint8_t iconNum = 1;
  #endif
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Log::log(LOG_LEVEL_INFO, D_LOG_LCD "No cards exists");
    return;
  }
  if (globalParams.getScreenPageNumber() >= screenConfig->getNumCards()) {
    globalParams.setScreenPageNumber(screenConfig->getNumCards()-1);
  }
  if (screenConfig->getCard(globalParams.getScreenPageNumber()) == NULL) {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Card is not defined");
    return;
  }
  #ifndef FIRMWARE_MINIMAL
  String titleIcon = String(screenConfig->getCard(globalParams.getScreenPageNumber())->getIcon());
  #else 
  String titleIcon = "mdi:alert";
  #endif
  ImageCanvas * titleIconCanvas = NULL;
  if (titleIcon != "") {
    titleIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_ICON);
    titleIconCanvas->setVAlign(ALIGN_MIDDLE);
    titleIconCanvas->setHAlign(ALIGN_CENTER);
    titleIconCanvas->setWidth(HEADER_ICON_WIDTH + HEADER_SETTINGS_LEFT*2);
    titleIconCanvas->setPath(titleIcon);
    #ifdef FIRMWARE_MINIMAL
    titleIconCanvas->setFgColor(convert2rgb565(0xFF0000));
    #endif
    if (settings.isDarkMode()) {
      titleIconCanvas->setFgColor(DARK_THEME_ICON_COLOR);
    } else {
      titleIconCanvas->setFgColor(LIGHT_THEME_ICON_COLOR);
    }
    #ifndef FIRMWARE_MINIMAL
    if (globalParams.getScreenPageType() == ScreenPageSettings) {
      titleIconCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            globalParams.setScreenPageType(ScreenPageMain);
            globalParams.setScreenPageNumber(0);
            return true;
          }
          return false;
        });
    } else if (globalParams.getScreenPageType() == ScreenPageEntity) {
      titleIconCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            globalParams.setScreenPageType(ScreenPageMain);
            globalParams.setScreenPageNumber(0);
            return true;
          }
          return false;
        });
    } else {
      titleIconCanvas->onTouch(std::bind(&handle_touch_header_page, _1, _2, _3));
    }
    #endif
  }
  #ifndef FIRMWARE_MINIMAL
  ImageCanvas * settingsIconCanvas = NULL;
  if ((globalParams.getScreenPageType() != ScreenPageSettings) && (globalParams.getScreenPageType() != ScreenPageScreenSaver)) {
    settingsIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_SETTINGS_ICON);
    settingsIconCanvas->setX(headerCanvas->getX() + headerCanvas->getWidth() - iconNum*HEADER_ICON_SPACING);
    settingsIconCanvas->setWidth(HEADER_ICON_SPACING);
    settingsIconCanvas->setHAlign(ALIGN_CENTER);
    settingsIconCanvas->setVAlign(ALIGN_MIDDLE);
    settingsIconCanvas->setPath("mdi:cog");
    if (settings.isDarkMode()) {
      settingsIconCanvas->setFgColor(DARK_THEME_ICON_COLOR);
    } else {
      settingsIconCanvas->setFgColor(LIGHT_THEME_ICON_COLOR);
    }
    settingsIconCanvas->onTouch(handle_touch_header_setttings);
    iconNum++;
  }
  #endif
  ImageCanvas * wifiIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_WIFI_ICON);
  wifiIconCanvas->setWidth(HEADER_ICON_SPACING);
  #ifndef FIRMWARE_MINIMAL
  if (settingsIconCanvas != NULL) {
    wifiIconCanvas->alignRight(settingsIconCanvas);
  } else {
  #endif
    wifiIconCanvas->alignRight();
  #ifndef FIRMWARE_MINIMAL
  }
  #endif
  wifiIconCanvas->setHAlign(ALIGN_CENTER);
  wifiIconCanvas->setVAlign(ALIGN_MIDDLE);
  if (settings.isDarkMode()) {
    wifiIconCanvas->setFgColor(DARK_THEME_ICON_COLOR);
  } else {
    wifiIconCanvas->setFgColor(LIGHT_THEME_ICON_COLOR);
  }
  PageSelectorCanvas * pageSelectorCanvas = NULL;
  #ifndef FIRMWARE_MINIMAL
  if (screenConfig->getNumCards() > 1) {
    pageSelectorCanvas = new PageSelectorCanvas(headerCanvas, ID_HEADER_PAGE_SELECTOR);
    pageSelectorCanvas->setNumPages(screenConfig->getNumCards());
    pageSelectorCanvas->setSelected(globalParams.getScreenPageNumber());
    pageSelectorCanvas->setWidth(HEADER_PAGE_INDICATOR_SPACING*screenConfig->getNumCards());
    pageSelectorCanvas->alignRight(wifiIconCanvas);
    pageSelectorCanvas->setLimitStep(true);
    pageSelectorCanvas->onValueChange(handle_page_value_change); 
  }
  #endif
  #ifndef FIRMWARE_MINIMAL
  String title = String(screenConfig->getCard(globalParams.getScreenPageNumber())->getTitle());
  #else
  String title = "Upgrade required!";
  #endif
  if (title != "") {
    TextCanvas * titleCanvas = new TextCanvas(headerCanvas, ID_HEADER_TITLE);
    if (titleIconCanvas != NULL) {
      titleCanvas->setX(titleIconCanvas->getRightX());
    }
    if (pageSelectorCanvas != NULL) {
      titleCanvas->setRightX(pageSelectorCanvas->getX());
    } else {
      titleCanvas->setRightX(wifiIconCanvas->getX());
    }
    titleCanvas->setVAlign(ALIGN_MIDDLE);
    titleCanvas->setFont("Roboto-Medium24");
    titleCanvas->setText(title);
    #ifdef FIRMWARE_MINIMAL
    titleCanvas->setFgColor(convert2rgb565(0xFF0000));
    #else
    if (globalParams.getScreenPageType() == ScreenPageSettings) {
      titleCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            globalParams.setScreenPageType(ScreenPageMain);
            globalParams.setScreenPageNumber(0);
            return true;
          }
          return false;
        });
    } else if (globalParams.getScreenPageType() == ScreenPageEntity) {
      titleCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            globalParams.setScreenPageType(ScreenPageMain);
            globalParams.setScreenPageNumber(0);
            return true;
          }
          return false;
        });
    }
  #endif
  }
}

#ifndef FIRMWARE_MINIMAL
bool handle_touch_header_setttings(Canvas *canvas, TouchEvent event, TouchEventData eventData) {
  if (event == TouchActionTapped || event == TouchActionLongPressed) {
    globalParams.setScreenPageType(ScreenPageSettings);
    globalParams.setScreenPageNumber(0);
    return true;
  }
  return false;
}

bool handle_page_value_change (PageSelectorCanvas *canvas, uint16_t selectedPage, uint16_t prevSelectedPage) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (selectedPage >= screenConfig->getNumCards()) {
    selectedPage = screenConfig->getNumCards()-1;
  }
  if (screenConfig->getNumCards() == 0) {
    selectedPage = 0;
  }
  globalParams.setScreenPageNumber(selectedPage);
  return true;
}

bool handle_touch_header_page(Canvas *canvas, TouchEvent event, TouchEventData eventData) {
  if (event == TouchActionTapped || event == TouchActionLongPressed) {
    int8_t next_page = globalParams.getScreenPageNumber() + 1;
    ScreenConfig * screenConfig = getScreenConfig(tftConfig);
    if (next_page == screenConfig->getNumCards()) {
      next_page = 0;
    }
    globalParams.setScreenPageNumber(next_page);
    return true;
  }
  return false;
}

extern char * haplate_dark_mode_entity;
extern char * haplate_bottom_bar_entity;
extern char * haplate_restart;
extern char * haplate_clear_cache_entity;
void set_row_state (String entity_id, String state) {
  homeAssistant.sync_state(entity_id, state);
}

String get_row_state (String entity_id) {
  String state = homeAssistant.getState(entity_id);
  return state;
}

String get_row_attribute (String entity_id, String attribute_name) {
  if (globalParams.getScreenPageType() == ScreenPageSettings) {
    return "";
  } else {
    return homeAssistant.get_attribute(entity_id, attribute_name);
  }
}

void set_row_attribute (String entity_id, String attribute_name, String attribute_value) {
  if (globalParams.getScreenPageType() == ScreenPageSettings) {
  } else {
    homeAssistant.set_attribute(entity_id, attribute_name, attribute_value);
  }
}
#endif

bool handle_touch (TouchEvent event, TouchEventData eventData) {
  globalParams.setLastTouchTimeMillis(millis());
  bool touchedHandled = false;
  if(xSemaphoreTake(screenConfigMutex, portMAX_DELAY) == pdTRUE) {
    touchedHandled = screenCanvasG->handleTouch(event, eventData);
    xSemaphoreGive(screenConfigMutex);
  }
  return touchedHandled;
}

uint16_t convert2rgb565 (uint32_t color) {
  uint8_t blue = color;
  uint8_t green = (color>> 8);
  uint8_t red = (color>> 16);
  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

void tft_set_dark_mode(uint8_t dark_mode) {
  if (settings.isDarkMode() != dark_mode) {
    settings.setDarkMode(dark_mode);
    globalParams.setScreenRedraw(true);
  }
}

void tft_set_bottom_bar (uint8_t state) {
  settings.setBottomBar(state);
}

uint32_t tft_set_page_num (uint8_t page_num) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  uint32_t ret = 1;
  if (page_num >= 0 && page_num < screenConfig->getNumCards()) {
    globalParams.setScreenPageNumber(page_num);
  } else {
    ret = 0;
  }
  return ret;
}

void tft_set_settings_page(bool state) {
  if (state && globalParams.getScreenPageType() != ScreenPageSettings) {
    globalParams.setScreenPageType(ScreenPageSettings);
  } else if (!state && globalParams.getScreenPageType() == ScreenPageSettings) {
    globalParams.setScreenPageType(ScreenPageMain);
  }
}