#include "TFT_LCD.h"
#include "Settings.hpp"
#include "GlobalParams.hpp"
#include "RtcTime.hpp"
#include "Log.hpp"
#include "Fonts.hpp"
#include "RowCanvas/RowCanvases.hpp"
#include "CardCanvas/CardCanvases.hpp"
TFTConfig * tftConfig ;
Canvas * screenCanvasG;
TFT_eSPI tft = TFT_eSPI(); 
SemaphoreHandle_t screenConfigMutex;
using namespace HomeAssistant;
using namespace std::placeholders;
extern HomeAssistantManager hass;

void tft_lcd_setup(ScreenConfig * mainScreenConfig, ScreenConfig * settingsScreenConfig) {
  tftConfig = (TFTConfig *) malloc(sizeof(TFTConfig));
  tftConfig->mainScreenConfig = mainScreenConfig;
  tftConfig->settingsScreenConfig = settingsScreenConfig;
  tftConfig->entityScreenConfig = new ScreenConfig();
  tftConfig->setupScreenConfig = new ScreenConfig();
  tftConfig->setupScreenConfig->addCard(new BaseCardConfig(WifiQrCardConfig::TYPE, "WiFi setup", ICON_HOME_ASSISTANT));
  tftConfig->screenSaverScreenConfig = new ScreenConfig();
  tftConfig->screenSaverScreenConfig->addCard(new BaseCardConfig(DigitalClockCardConfig::TYPE, "", ""));
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

uint32_t buildDigitalClockCanvas(TFTConfig * tftConfig, Canvas * bodyCanvas) {
  DigitalClockCanvas * digitalClockCanvas = new DigitalClockCanvas(bodyCanvas, 0);
  digitalClockCanvas->setHAlign(ALIGN_CENTER);
  digitalClockCanvas->setVAlign(ALIGN_MIDDLE);
  digitalClockCanvas->setMargin(10);
  digitalClockCanvas->onTouch([](Canvas*, TouchEvent event, TouchEventData)->bool{
      if (isEvent(event, TouchEvent::TouchActionTapped)) {
        globalParams.setScreenPageType(ScreenPageMain);
        return true;
      }
      return false;
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
  uint8_t index=(ut.day_of_week-1)*3;
  memcpy(day, (void *)(kDayNamesEngligh+index), 3);
  snprintf(dateString, sizeof(dateString)-1, "%s, %s %d", day, ut.name_of_month, ut.day_of_month);
  digitalClockCanvas->setDate(dateString);
  digitalClockCanvas->setBgColor(theme->colorScreenSaverBackground.get16Bit());
  digitalClockCanvas->setFgColor(theme->colorScreenSaverText.get16Bit());
  return true;
}

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
  bodyCanvas->setFgColor(theme->colorBodyText.get16Bit());
  bodyCanvas->setBgColor(theme->colorBodyBackground.get16Bit());
  bodyCanvas->setDrawBackgroundEnable(true);
  CardCanvas * cardCanvas = (CardCanvas *)(*bodyCanvas)[0];
  if (pageConfig->isType(DigitalClockCardConfig::TYPE)) {
    return setDigitalClockCanvasState(tftConfig, bodyCanvas);
  #ifndef FIRMWARE_MINIMAL
  } else if (pageConfig->isType(EntitiesCardConfig::TYPE) ||
              pageConfig->isType(LightCardConfig::TYPE) ||
              pageConfig->isType(FanCardConfig::TYPE) ||
              pageConfig->isType(MediaControlCardConfig::TYPE) ||
              pageConfig->isType(AlarmPanelCardConfig::TYPE) ||
              pageConfig->isType(WeatherForecastCardConfig::TYPE) ||
              pageConfig->isType(InputTextCardConfig::TYPE) ||
              pageConfig->isType(EntityCardConfig::TYPE) ||
              pageConfig->isType(ThermostatCardConfig::TYPE) ||
              pageConfig->isType(HumidifierCardConfig::TYPE)) {
    cardCanvas->update();
  #endif
  } else if (pageConfig->isType(WifiQrCardConfig::TYPE) && globalParams.getScreenPageType() == ScreenPageSetup) {
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
  LcdTheme * theme = settings.getLcdSelectedTheme();
  if (pageConfig->isType(DigitalClockCardConfig::TYPE)) {
    return buildDigitalClockCanvas(tftConfig, bodyCanvas);
  #ifndef FIRMWARE_MINIMAL
  } else if (pageConfig->isType(EntitiesCardConfig::TYPE)) {
    (new EntitesCardCanvas(bodyCanvas, 0, &hass, (EntitiesCardConfig *)pageConfig, theme))->setCallback([tftConfig](BaseCardConfig * cardConfig)->bool{
        tftConfig->entityScreenConfig->clearCards();
        tftConfig->entityScreenConfig->addCard(cardConfig);
        globalParams.setScreenPageType(ScreenPageEntity);
        return true;
      });
  } else if (pageConfig->isType(SwitchCardConfig::TYPE)) {
    new SwitchCardCanvas(bodyCanvas, 0, &hass, (SwitchCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(LightCardConfig::TYPE)) {
    new LightCardCanvas(bodyCanvas, 0, &hass, (LightCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(FanCardConfig::TYPE)) {
    new FanCardCanvas(bodyCanvas, 0, &hass, (FanCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(MediaControlCardConfig::TYPE)) {
    new MediaControlCardCanvas(bodyCanvas, 0, &hass, (MediaControlCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(AlarmPanelCardConfig::TYPE)) {
    new AlarmPanelCardCanvas(bodyCanvas, 0, &hass, (AlarmPanelCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(WeatherForecastCardConfig::TYPE)) {
    new WeatherForecastCardCanvas(bodyCanvas, 0, &hass, (WeatherForecastCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(InputTextCardConfig::TYPE)) {
    new InputTextCardCanvas(bodyCanvas, 0, &hass, (InputTextCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(ThermostatCardConfig::TYPE)) {
    new ThermostatCardCanvas(bodyCanvas, 0, &hass, (ThermostatCardConfig *)pageConfig, theme);
  } else if (pageConfig->isType(HumidifierCardConfig::TYPE)) {
    new HumidifierCardCanvas(bodyCanvas, 0, &hass, (HumidifierCardConfig *)pageConfig, theme);
  #endif
  } else if (pageConfig->isType(WifiQrCardConfig::TYPE) && globalParams.getScreenPageType() == ScreenPageSetup) {
    return buildWifiQrCanvas(tftConfig, bodyCanvas);
  } else {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Unknown card type %s", pageConfig->getType());
    return 0;
  }
  return 0;
}

uint32_t drawScreen() {
  if (globalParams.getScreenRebuild() || true) {
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
  SwipeCanvas * headerCanvas = new SwipeCanvas(screenCanvas, ID_HEADER);
  headerCanvas->setHeight(HEADER_HEIGHT);
  headerCanvas->setY(screenCanvas->getY() + (settings.isBottomBar())?(screenCanvas->getHeight() - HEADER_HEIGHT):0);
  buildHeaderCanvas(tftConfig, headerCanvas);
  SwipeCanvas * bodyCanvas = new SwipeCanvas(screenCanvas, ID_BODY);
  bodyCanvas->setHeight(screenCanvas->getHeight()-HEADER_HEIGHT);
  bodyCanvas->setY(screenCanvas->getY() + ((settings.isBottomBar())?0:HEADER_HEIGHT));
  buildBodyCanvas(tftConfig, bodyCanvas);
  auto swipeComplete = [](SwipeCanvas* canvas, SwipeEventData eventData)-> bool {
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
  };
  auto swipe = [](SwipeCanvas* canvas, SwipeEventData eventData)-> bool {
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
    return true;
  };
  bodyCanvas->onSwipeComplete(swipeComplete);
  headerCanvas->onSwipeComplete(swipeComplete);
  bodyCanvas->onSwipe(swipe);
  headerCanvas->onSwipe(swipe);
  return 0;
}

uint16_t wifiSyncState = 0;
uint16_t prevIconChangeTime = 0;

void setHeaderCanvasState (TFTConfig * tftConfig, Canvas * headerCanvas) {
  Canvas * wifiCanvas = (*headerCanvas)[ID_HEADER_WIFI_ICON];
  Canvas * iconCanvas = (*headerCanvas)[ID_HEADER_ICON];
  Canvas * settingsIconCanvas = (*headerCanvas)[ID_HEADER_SETTINGS_ICON];
  PageSelectorCanvas * pageSelectorCanvas = (PageSelectorCanvas *)(*headerCanvas)[ID_HEADER_PAGE_SELECTOR];
  LcdTheme * theme = settings.getLcdSelectedTheme();
  if (wifiCanvas != NULL) {
    ImageCanvas * wifiImageCanvas = (ImageCanvas *) wifiCanvas;
    String wifiIcon = ICON_WIFI_ALERT;
    if (globalParams.isWifiApMode()) {
      wifiIcon = ICON_ACCESS_POINT;
    } else if (globalParams.isHomeAssistantConnectionFailed() && globalParams.isWifiUp()) {
      wifiIcon = ICON_WIFI_ALERT;
    } else if (globalParams.isHomeAssistantSyncing() && globalParams.isWifiUp()) {
      wifiIcon = ICON_WIFI_ARROW_UP_DOWN;
    } else if (globalParams.isHomeAssistantConnected() && globalParams.isWifiUp()) {
      wifiIcon = ICON_WIFI_LOCK;
    } else if (globalParams.isWifiUp()) {
      wifiIcon = ICON_WIFI;
    } else {
      wifiIcon = ICON_WIFI_OFF;
    }
    wifiImageCanvas->setPath(wifiIcon);
    if (globalParams.getScreenPageType() == ScreenPageScreenSaver) {
      wifiImageCanvas->setFgColor(theme->colorScreenSaverHeaderIcon.get16Bit());
    } else {
      wifiImageCanvas->setFgColor(theme->colorHeaderIcon.get16Bit());
    }
  }
  if (iconCanvas != nullptr) {
    #ifndef FIRMWARE_MINIMAL
    iconCanvas->setFgColor(theme->colorHeaderIcon.get16Bit());
    #else 
    iconCanvas->setFgColor(Color32Bit(0xFF0000).get16Bit());
    #endif
  }
  if (settingsIconCanvas != nullptr) {
    settingsIconCanvas->setFgColor(theme->colorHeaderIcon.get16Bit());
  }
  if (globalParams.getScreenPageType() == ScreenPageScreenSaver) {
    headerCanvas->setBgColor(theme->colorScreenSaverBackground.get16Bit());
  } else {
    headerCanvas->setBgColor(theme->colorHeaderBackground.get16Bit());
  }
  if (pageSelectorCanvas != nullptr) {
    pageSelectorCanvas->setFgColor(theme->colorPageIndicator.get16Bit());
    pageSelectorCanvas->setSelectedColor(theme->colorPageIndicatorSelected.get16Bit());
  }
  headerCanvas->setFgColor(theme->colorHeaderText.get16Bit());
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
  String titleIcon = ICON_ALERT;
  #endif
  ImageCanvas * titleIconCanvas = NULL;
  if (titleIcon != "") {
    titleIconCanvas = new ImageCanvas(headerCanvas, ID_HEADER_ICON);
    titleIconCanvas->setVAlign(ALIGN_MIDDLE);
    titleIconCanvas->setHAlign(ALIGN_CENTER);
    titleIconCanvas->setWidth(HEADER_ICON_WIDTH + HEADER_SETTINGS_LEFT*2);
    titleIconCanvas->setPath(titleIcon);
    #ifdef FIRMWARE_MINIMAL
    titleIconCanvas->setFgColor(Color32Bit(0xFF0000).get16Bit());
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
    settingsIconCanvas->setPath(ICON_COG);
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
    wifiIconCanvas->hAlignLeft(settingsIconCanvas);
  } else {
  #endif
    wifiIconCanvas->hAlignRight();
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
    uint8_t width = HEADER_PAGE_INDICATOR_SPACING*screenConfig->getNumCards();
    if (width > 80) {
      width = 80;
    }
    pageSelectorCanvas = new PageSelectorCanvas(headerCanvas, ID_HEADER_PAGE_SELECTOR);
    pageSelectorCanvas->setNumPages(screenConfig->getNumCards());
    pageSelectorCanvas->setSelected(globalParams.getScreenPageNumber());
    pageSelectorCanvas->setWidth(width);
    pageSelectorCanvas->hAlignLeft(wifiIconCanvas);
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
    } else {
      titleCanvas->setPaddingLeft(10);
    }
    if (pageSelectorCanvas != NULL) {
      titleCanvas->setRightX(pageSelectorCanvas->getX());
    } else {
      titleCanvas->setRightX(wifiIconCanvas->getX());
    }
    titleCanvas->setVAlign(ALIGN_MIDDLE);
    titleCanvas->setFont(FONT_MEDIUM_24);
    titleCanvas->setText(title);
    #ifdef FIRMWARE_MINIMAL
    titleCanvas->setFgColor(Color32Bit(0xFF0000).get16Bit());
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
#endif

bool handleTouch (TouchEvent event, TouchEventData eventData) {
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
  }
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