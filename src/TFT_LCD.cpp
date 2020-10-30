#include "TFT_LCD.h"
TFTConfig * tftConfig ;
Canvas * screenCanvasG;
TFT_eSPI tft = TFT_eSPI(); 
SemaphoreHandle_t screenConfigMutex;
extern HomeAssistant homeAssistant;

using namespace std::placeholders;

void tft_lcd_setup(ScreenConfig * mainScreenConfig, ScreenConfig * settingsScreenConfig) {
  tftConfig = (TFTConfig *) malloc(sizeof(TFTConfig));
  tftConfig->curr_page_num = 0;
  tftConfig->prev_page_num = -1;
  tftConfig->settings_page = 0;
  tftConfig->dark_mode = 1;
  tftConfig->require_update = 1;
  tftConfig->screen_refresh = 1;
  tftConfig->mainScreenConfig = mainScreenConfig;
  tftConfig->settingsScreenConfig = settingsScreenConfig;
  tftConfig->entityScreenConfig = new ScreenConfig();
  tftConfig->firstSetupScreenConfig = new ScreenConfig();
  tftConfig->firstSetupScreenConfig->addCard(new BaseCardConfig(PAGE_TYPE_WIFI_QR, "First setup", "mdi:home-assistant"));
  tftConfig->bottom_header = 0;
  tftConfig->rotation = 3;
  tftConfig->prev_rotation = -1;
  tftConfig->entityPage = 0;
  tftConfig->first_setup = 0;
  tftConfig->connectionState = TFT_WifiDisconnected;
  screenCanvasG = new Canvas(&tft);
  screenConfigMutex = xSemaphoreCreateMutex();
  tft.init();
}

void set_screen_rotation (int8_t rotation) {
  int8_t prev_rotation = tftConfig->rotation;
  if ((rotation == 0) || (rotation == 2)) {
    screenCanvasG->setWidth(LCD_HEIGHT);
    screenCanvasG->setHeight(LCD_WIDTH);
  } else {
    screenCanvasG->setWidth(LCD_WIDTH);
    screenCanvasG->setHeight(LCD_HEIGHT);
  }
  tftConfig->rotation = rotation;
  if (prev_rotation != rotation) {
    tft_screen_refresh();
  }
}

void handle_touch_switch_row_state(Canvas *canvas, TouchEvent event, TouchEventData eventData, String entity_id) {
  if (event == TouchActionTapped || event == TouchActionLongPressed) {
    if ((tftConfig->connectionState == TFT_HaConnected) || (tftConfig->connectionState == TFT_HaSyncing) || tftConfig->settings_page) {
      String state = get_row_state(entity_id);
      if (state == "on") {
        set_row_state(entity_id, "off");
      } else {
        set_row_state(entity_id, "on");
      }
      tftConfig->require_update = 1;
    }
  }
  return;
}

ScreenConfig * getScreenConfig (TFTConfig * tftConfig) {
  if (tftConfig->first_setup) {
    return tftConfig->firstSetupScreenConfig;
  } else if (tftConfig->settings_page) {
    return tftConfig->settingsScreenConfig;
  } else if (tftConfig->entityPage){
    return tftConfig->entityScreenConfig;
  } else {
    return tftConfig->mainScreenConfig;
  }
}

uint32_t buildLightBodyCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
  LightCardConfig * lightPageConfig = (LightCardConfig *) pageConfig;
  if (String(lightPageConfig->getEntityId()) != "") {
    Serial.println("Adding light entity");
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
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
  LightCardConfig * lightPageConfig = (LightCardConfig *) pageConfig;
  if (String(lightPageConfig->getEntityId()) != "") {
    Serial.println("Setting light entity");
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
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
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
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
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

uint32_t buildWifiQrCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  QrCodeCanvas * qrCodeCanvas = new QrCodeCanvas(bodyCanvas, 0);
  qrCodeCanvas->setHAlign(ALIGN_CENTER);
  qrCodeCanvas->setVAlign(ALIGN_MIDDLE);
  qrCodeCanvas->setMargin(10);
  qrCodeCanvas->setPath("wifi");
  return 1;
}

uint32_t buildFanBodyCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
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

bool setFanBodyCanvasState(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
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
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
  EntitesCardConfig * rowPageConfig = (EntitesCardConfig *) pageConfig;
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
        String state = get_row_state(default_row->getEntityId());\
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
        if (domain == "switch" || domain == "climate" || domain == "media_player") {
          SwitchEntityRowCanvas * switchEntityCanvas = (SwitchEntityRowCanvas *) baseEntityCanvas;
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
        }
      } else if (String(row->getType()) == ENTITES_ROW_TYPE_BUTTONS) {
        ButtonsRowConfig * buttonsRow = (ButtonsRowConfig *)row;
        ButtonsEntityRowCanvas * buttonsEntityRowCanvas = (ButtonsEntityRowCanvas *) baseEntityCanvas;
        for (uint8_t buttonIndex = 0;buttonIndex <buttonsRow->getNumButtons();buttonIndex++) {
          DefaultRowConfig * buttonConfig = (DefaultRowConfig *) buttonsRow->getButton(buttonIndex);
          if (String(buttonConfig->getIcon()) != "") {
            buttonsEntityRowCanvas->setIconPath(buttonConfig->getIcon(), buttonIndex);
          } else {
            String icon = homeAssistant.get_attribute(buttonConfig->getEntityId(), "icon");
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
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
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
              Serial.println("New brightness is "+String(brightness));
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
        if (domain == "light" || domain == "switch" || domain == "climate" || domain == "media_player") {
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
                tftConfig->entityPage = 1;
                tftConfig->screen_refresh = 1;
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
                tftConfig->entityPage = 1;
                tftConfig->screen_refresh = 1;
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
                tftConfig->entityPage = 1;
                tftConfig->screen_refresh = 1;
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

uint32_t setBodyCanvasState (TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Serial.println("Num pages == 0");
    return 0;
  }
  if (screenConfig->getNumCards() <= tftConfig->curr_page_num) {
    Serial.println("Num pages <= curr");
    return 0;
  }
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
  if (pageConfig == NULL) {
    Serial.println("Page is null");
    return 0;
  }
  if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_ENTITES)) {
    return setEntitiesBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_SWITCH)) {
    return setSwitchBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_LIGHT)) {
    return setLightBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_FAN)) {
    return setFanBodyCanvasState(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_WIFI_QR) && tftConfig->first_setup) {
    return false;
  } else {
    Serial.println("Unknown page type");
    return 0;
  }
  return 0;
}

uint32_t buildBodyCanvas (TFTConfig * tftConfig, Canvas * bodyCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Serial.println("Num pages == 0");
    return 0;
  }
  if (screenConfig->getNumCards() <= tftConfig->curr_page_num) {
    Serial.println("Num pages <= curr");
    return 0;
  }
  BaseCardConfig * pageConfig = screenConfig->getCard(tftConfig->curr_page_num);
  if (pageConfig == NULL) {
    Serial.println("Page is null");
    return 0;
  }
  if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_ENTITES)) {
    return buildEntitesBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_SWITCH)) {
    return buildSwitchBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_LIGHT)) {
    return buildLightBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_FAN)) {
    return buildFanBodyCanvas(tftConfig, bodyCanvas);
  } else if ((pageConfig != NULL) && (String(pageConfig->getType()) == PAGE_TYPE_WIFI_QR) && tftConfig->first_setup) {
    return buildWifiQrCanvas(tftConfig, bodyCanvas);
  } else {
    Serial.println("Unknown page type");
    Serial.println(String(pageConfig->getType()));
    return 0;
  }
}

uint32_t drawScreen() {
  uint8_t screen_refresh = tftConfig->screen_refresh;
  uint8_t require_update = tftConfig->require_update;
  if (screen_refresh || require_update) {
    Canvas * _screenCanvas = screenCanvasG;
    if(xSemaphoreTake(screenConfigMutex, portMAX_DELAY) == pdTRUE) {
      if (screen_refresh) {
        _screenCanvas = new Canvas(&tft);
        if ((tftConfig->rotation == 0) || (tftConfig->rotation == 2)) {
          _screenCanvas->setWidth(LCD_HEIGHT);
          _screenCanvas->setHeight(LCD_WIDTH);
        } else {
          _screenCanvas->setWidth(LCD_WIDTH);
          _screenCanvas->setHeight(LCD_HEIGHT);
        }
        buildScreenCanvas(_screenCanvas);
        _screenCanvas->setDarkMode(tftConfig->dark_mode);
        _screenCanvas->setBgColor(tftConfig->dark_mode?TFT_BLACK:TFT_WHITE);
        _screenCanvas->setFgColor(tftConfig->dark_mode?TFT_WHITE:TFT_BLACK);
      }
      setScreenCanvasState(_screenCanvas);
      _screenCanvas->redraw();
      if (screen_refresh) {
        
          Canvas * prevScreenCanvas = screenCanvasG;
          screenCanvasG = _screenCanvas;
          delete prevScreenCanvas;
      }
      xSemaphoreGive(screenConfigMutex);
    } else {
      Serial.println("Could not obtain the mutex");
    }
  }
  tftConfig->screen_refresh = 0;
  tftConfig->require_update = 0;
  return 1;
}

uint32_t setScreenCanvasState(Canvas * screenCanvasG) {
  Canvas * headerCanvas = (*screenCanvasG)[ID_HEADER];
  if (headerCanvas != NULL) {
    setHeaderCanvasState(tftConfig, headerCanvas);
  }
  Canvas * bodyCanvas =  (*screenCanvasG)[ID_BODY];
  if (bodyCanvas != NULL) {
    setBodyCanvasState(tftConfig, bodyCanvas);
  }
  return 0;
}

uint32_t buildScreenCanvas(Canvas * screenCanvas) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    return 0;
  }
  if (tftConfig->curr_page_num >= screenConfig->getNumCards()) {
    tftConfig->curr_page_num = 0;
  }
  if (tftConfig->prev_rotation != tftConfig->rotation) {
    tft.setRotation(tftConfig->rotation);
    tftConfig->prev_rotation = tftConfig->rotation;
  }
  Canvas * headerCanvas = new Canvas(screenCanvas, ID_HEADER);
  headerCanvas->setHeight(HEADER_HEIGHT);
  headerCanvas->setY(screenCanvas->getY() + (tftConfig->bottom_header)?(screenCanvas->getHeight() - HEADER_HEIGHT):0);
  buildHeaderCanvas(tftConfig, headerCanvas);
  SwipeCanvas * bodyCanvas = new SwipeCanvas(screenCanvas, ID_BODY);
  bodyCanvas->setHeight(screenCanvas->getHeight()-HEADER_HEIGHT);
  bodyCanvas->setY(screenCanvas->getY() + ((tftConfig->bottom_header)?0:HEADER_HEIGHT));
  buildBodyCanvas(tftConfig, bodyCanvas);
  bodyCanvas->onSwipeComplete([](SwipeCanvas* canvas, SwipeEventData eventData)-> bool {
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
      int16_t temp_page_num = tftConfig->curr_page_num + delta;
      if (temp_page_num >= screenConfig->getNumCards()) {
        temp_page_num = 0;
      } else if (temp_page_num < 0) {
        temp_page_num = screenConfig->getNumCards()-1;
      }
      if (tftConfig->prev_page_num != temp_page_num) {
        tftConfig->curr_page_num = temp_page_num;
        tft_screen_refresh();
      }
      return true;
    });
  bodyCanvas->onSwipe([](SwipeCanvas* canvas, SwipeEventData eventData)-> bool{
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
  tftConfig->prev_page_num = tftConfig->curr_page_num;
  return 0;
}

uint16_t wifiSyncState = 0;
uint16_t prevIconChangeTime = 0;

void setHeaderCanvasState (TFTConfig * tftConfig, Canvas * headerCanvas) {
  Canvas * wifiCanvas = (*headerCanvas)[ID_HEADER_WIFI_ICON];
  if (wifiCanvas != NULL) {
    ImageCanvas * wifiImageCanvas = (ImageCanvas *) wifiCanvas;
    String wifiIcon = "mdi:wifi-alert";
    if (tftConfig->connectionState == TFT_FirstSetup) {
      wifiIcon = "mdi:access-point";
    } else if (tftConfig->connectionState == TFT_HaSyncing) {
      wifiIcon = "mdi:wifi-arrow-up-down";
    } else if (tftConfig->connectionState == TFT_HaConnected) {
      wifiIcon = "mdi:wifi-lock";
    } else if (tftConfig->connectionState == TFT_WifiConnected) {
      wifiIcon = "mdi:wifi";
    } else if (tftConfig->connectionState == TFT_WifiDisconnected) {
      wifiIcon = "mdi:wifi-off";
    } else if (tftConfig->connectionState == TFT_WifiAlert) {
      wifiIcon = "mdi:wifi-alert";
    }
    wifiImageCanvas->setPath(wifiIcon);
    //wifiImageCanvas->stop();
  }
}

void buildHeaderCanvas (TFTConfig * tftConfig, Canvas * headerCanvas) {
  uint8_t iconNum = 1;
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (screenConfig->getNumCards() == 0) {
    Serial.println("num == 0");
    return;
  }
  if (tftConfig->curr_page_num >= screenConfig->getNumCards()) {
    Serial.println("pages > num");
    return;
  }
  if (screenConfig->getCard(tftConfig->curr_page_num) == NULL) {
    Serial.println("page is null");
    return;
  }
  String titleIcon = String(screenConfig->getCard(tftConfig->curr_page_num)->getIcon());
  ImageCanvas * titleIconCanvas = NULL;
  if (titleIcon != "") {
    titleIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_ICON);
    titleIconCanvas->setVAlign(ALIGN_MIDDLE);
    titleIconCanvas->setHAlign(ALIGN_CENTER);
    titleIconCanvas->setWidth(HEADER_ICON_WIDTH + HEADER_SETTINGS_LEFT*2);
    titleIconCanvas->setPath(titleIcon);
    if (tftConfig->dark_mode) {
      titleIconCanvas->setFgColor(DARK_THEME_ICON_COLOR);
    } else {
      titleIconCanvas->setFgColor(LIGHT_THEME_ICON_COLOR);
    }
    if (tftConfig->settings_page) {
      titleIconCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            tftConfig->settings_page = 0;
            tftConfig->curr_page_num = 0;
            tftConfig->screen_refresh = 1;
            return true;
          }
          return false;
        });
    } else if (tftConfig->entityPage) {
      titleIconCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            tftConfig->entityPage = 0;
            tftConfig->curr_page_num = 0;
            tftConfig->screen_refresh = 1;
            return true;
          }
          return false;
        });
    } else {
      titleIconCanvas->onTouch(std::bind(&handle_touch_header_page, _1, _2, _3));
    }
  }
  
  ImageCanvas * settingsIconCanvas = NULL;
  if (!tftConfig->settings_page) {
    settingsIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_SETTINGS_ICON);
    settingsIconCanvas->setX(headerCanvas->getX() + headerCanvas->getWidth() - iconNum*HEADER_ICON_SPACING);
    settingsIconCanvas->setWidth(HEADER_ICON_SPACING);
    settingsIconCanvas->setHAlign(ALIGN_CENTER);
    settingsIconCanvas->setVAlign(ALIGN_MIDDLE);
    settingsIconCanvas->setPath("mdi:cog");
    if (tftConfig->dark_mode) {
      settingsIconCanvas->setFgColor(DARK_THEME_ICON_COLOR);
    } else {
      settingsIconCanvas->setFgColor(LIGHT_THEME_ICON_COLOR);
    }
    settingsIconCanvas->onTouch(handle_touch_header_setttings);
    iconNum++;
  }
  
  ImageCanvas * wifiIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_WIFI_ICON);
  wifiIconCanvas->setWidth(HEADER_ICON_SPACING);
  if (settingsIconCanvas != NULL) {
    wifiIconCanvas->alignRight(settingsIconCanvas);
  } else {
    wifiIconCanvas->alignRight();
  }
  wifiIconCanvas->setHAlign(ALIGN_CENTER);
  wifiIconCanvas->setVAlign(ALIGN_MIDDLE);
  if (tftConfig->dark_mode) {
    wifiIconCanvas->setFgColor(DARK_THEME_ICON_COLOR);
  } else {
    wifiIconCanvas->setFgColor(LIGHT_THEME_ICON_COLOR);
  }
  PageSelectorCanvas * pageSelectorCanvas = NULL;
  if (screenConfig->getNumCards() > 1) {
    pageSelectorCanvas = new PageSelectorCanvas(headerCanvas, ID_HEADER_PAGE_SELECTOR);
    pageSelectorCanvas->setNumPages(screenConfig->getNumCards());
    pageSelectorCanvas->setSelected(tftConfig->curr_page_num);
    pageSelectorCanvas->setWidth(HEADER_PAGE_INDICATOR_SPACING*screenConfig->getNumCards());
    pageSelectorCanvas->alignRight(wifiIconCanvas);
    pageSelectorCanvas->setLimitStep(true);
    pageSelectorCanvas->onValueChange(handle_page_value_change); 
  }
  String title = String(screenConfig->getCard(tftConfig->curr_page_num)->getTitle());
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
    titleCanvas->setText(title);
    titleCanvas->setFont("Roboto-Medium24");
    if (tftConfig->settings_page && screenConfig->getNumCards() <= 1) {
      titleCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            tftConfig->settings_page = 0;
            tftConfig->curr_page_num = 0;
            tftConfig->screen_refresh = 1;
            return true;
          }
          return false;
        });
    } else if (tftConfig->entityPage) {
      titleCanvas->onTouch([tftConfig](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
          if (event == TouchActionTapped || event == TouchActionLongPressed) {
            tftConfig->entityPage = 0;
            tftConfig->curr_page_num = 0;
            tftConfig->screen_refresh = 1;
            return true;
          }
          return false;
        });
    } else {
      titleCanvas->onTouch(handle_touch_header_page); 
    }
  }
}

bool handle_touch_header_setttings(Canvas *canvas, TouchEvent event, TouchEventData eventData) {
  if (event == TouchActionTapped || event == TouchActionLongPressed) {
    tftConfig->settings_page = 1;
    tftConfig->curr_page_num = 0;
    tftConfig->screen_refresh = 1;
    return true;
  }
  return false;
}

bool handle_page_value_change (PageSelectorCanvas *canvas, uint16_t selectedPage, uint16_t prevSelectedPage) {
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  if (selectedPage >= screenConfig->getNumCards()) {
    Serial.println("Page Error it can't exceed max");
    return true;
  }
  tftConfig->curr_page_num = selectedPage;
  tftConfig->screen_refresh = 1;
  return true;
}

bool handle_touch_header_page(Canvas *canvas, TouchEvent event, TouchEventData eventData) {
  if (event == TouchActionTapped || event == TouchActionLongPressed) {
    int8_t next_page = tftConfig->curr_page_num + 1;
    ScreenConfig * screenConfig = getScreenConfig(tftConfig);
    if (next_page == screenConfig->getNumCards()) {
      next_page = 0;
    }
    tftConfig->curr_page_num = next_page;
    tftConfig->screen_refresh = 1;
    return true;
  }
  return false;
}

extern char * haplate_dark_mode_entity;
extern char * haplate_bottom_bar_entity;
extern char * haplate_restart;
extern char * haplate_clear_cache_entity;


void set_row_state (String entity_id, String state) {
  if (tftConfig->settings_page) {
    if (entity_id == String(haplate_dark_mode_entity)) {
      if (state == "on") {
        tftConfig->dark_mode = 1;
      } else {
        tftConfig->dark_mode = 0;
      }
      tftConfig->screen_refresh = 1;
      homeAssistant.sync_state(haplate_dark_mode_entity, state);
    } else if (entity_id == String(haplate_bottom_bar_entity)) {
      if (state == "on") {
        tftConfig->bottom_header = 1;
      } else {
        tftConfig->bottom_header = 0;
      }
      tftConfig->screen_refresh = 1;
      homeAssistant.sync_state(haplate_bottom_bar_entity, state);
    } else if (entity_id == String(haplate_restart)) {
      homeAssistant.sync_state(haplate_restart, state);
      ESP.restart();
    } else if (entity_id == String(haplate_clear_cache_entity)) {
      homeAssistant.clear_cache();
    }
  } else {
    homeAssistant.sync_state(entity_id, state);
  }
}

String get_row_state (String entity_id) {
  if (tftConfig->settings_page) {
    if (entity_id == String(haplate_dark_mode_entity)) {
      String state = homeAssistant.get_state(haplate_dark_mode_entity);
      if (state == "") {
        return (tftConfig->dark_mode)?"on":"off";
      }
      if (state == "on") {
        tft_set_dark_mode(1);
      } else {
        tft_set_dark_mode(0);
      }
      return state;
    } else if (entity_id == String(haplate_bottom_bar_entity)) {
      String state = homeAssistant.get_state(haplate_bottom_bar_entity);
      if (state == "") {
        return (tftConfig->bottom_header)?"on":"off";
      }
      if (state == "on") {
        tft_set_bottom_bar(1);
      } else {
        tft_set_bottom_bar(1);
      }
      homeAssistant.sync_state(haplate_bottom_bar_entity, state);
      return state;
    } else if (entity_id == String(haplate_restart)) {
      return "off";
    } else if (entity_id == String(haplate_clear_cache_entity)) {
      return "off";
    } else {
      return "";
    }
  } else {
    return homeAssistant.get_state(entity_id);
  }
}

String get_row_attribute (String entity_id, String attribute_name) {
  if (tftConfig->settings_page) {
    return "";
  } else {
    return homeAssistant.get_attribute(entity_id, attribute_name);
  }
}

void set_row_attribute (String entity_id, String attribute_name, String attribute_value) {
  if (tftConfig->settings_page) {
  } else {
    homeAssistant.set_attribute(entity_id, attribute_name, attribute_value);
  }
}

bool handle_touch (TouchEvent event, TouchEventData eventData) {
  bool touchedHandled = false;
  if(xSemaphoreTake(screenConfigMutex, portMAX_DELAY) == pdTRUE) {
    touchedHandled = screenCanvasG->handleTouch(event, eventData);
    xSemaphoreGive(screenConfigMutex);
  } else {
    Serial.println("Could not obtain the mutex");
  }
  return touchedHandled;
}

uint16_t convert2rgb565 (uint32_t color) {
  uint8_t blue = color;
  uint8_t green = (color>> 8);
  uint8_t red = (color>> 16);
  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

void tft_require_update () {
  tftConfig->require_update = 1;
}

void tft_screen_refresh () {
  tftConfig->screen_refresh = 1;
}

void tft_set_dark_mode(uint8_t dark_mode) {
  uint8_t prev_dark_mode = tftConfig->dark_mode;
  tftConfig->dark_mode = dark_mode;
  if (prev_dark_mode != dark_mode) {
    tft_require_update();
  }
}

void tft_set_connection_state (TFTConnectionState state) {
  TFTConnectionState prev_state = tftConfig->connectionState;
  tftConfig->connectionState = state;
  if (state == TFT_FirstSetup) {
    tft_set_first_setup_page(true);
  } else {
    tft_set_first_setup_page(false);
  }
  if (prev_state != state)
    tft_require_update();
}

void tft_set_bottom_bar (uint8_t state) {
  uint8_t prev_state = tftConfig->bottom_header;
  tftConfig->bottom_header = state;
  if (prev_state != state)
    tft_screen_refresh();
}

uint32_t tft_set_page_num (uint8_t page_num) {
  uint8_t prev_page_num = tftConfig->curr_page_num;
  ScreenConfig * screenConfig = getScreenConfig(tftConfig);
  uint32_t ret = 1;
  if (page_num >= 0 && page_num < screenConfig->getNumCards()) {
    tftConfig->curr_page_num = page_num;
  } else {
    ret = 0;
  }
  if (prev_page_num != tftConfig->curr_page_num) {
    tft_screen_refresh();
  }
  return ret;
}

void tft_set_settings_page (uint8_t state) {
  uint8_t prev_state = tftConfig->settings_page;
  tftConfig->settings_page = state;
  if (prev_state != state)
    tft_screen_refresh();
}


void tft_set_first_setup_page (uint8_t state) {
  uint8_t prev_state = tftConfig->first_setup;
  tftConfig->first_setup = state;
  if (prev_state != state)
    tft_screen_refresh();
}