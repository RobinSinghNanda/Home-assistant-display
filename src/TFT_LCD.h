#ifndef TFT_LCD_H
#define TFT_LCD_H

#include <TFT_eSPI.h> 
#include "ScreenConfig.hpp"
#ifndef FIRMWARE_MINIMAL
#include "Canvas/Canvases.hpp"
#else
#include "Canvas/Canvas.hpp"
#endif
#ifndef FIRMWARE_MINIMAL
#include "HomeAssistant/Components/Components.hpp"
#include "HomeAssistant/HomeAssistantManager.hpp"
#endif

#define NUM_TOUCH_REGIONS 10

//IDS
#define ID_HEADER_ICON 0
#define ID_HEADER_TITLE 1
#define ID_HEADER_PAGE_SELECTOR 2
#define ID_HEADER_WIFI_ICON 3
#define ID_HEADER_SETTINGS_ICON 4

#define ID_SWITCH_ENTITY 0

#define ID_LIGHT_SWITCH_ENTITY 0
#define ID_LIGHT_SWITCH_BRIGHTNESS_SLIDER 1

#define ID_FAN_SWITCH_ENTITY 0

#define ID_HEADER 0
#define ID_BODY 1

//page layout params
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_COLOR_PRIMARY  0x03a9f4

//Header parameters
#define HEADER_HEIGHT 40
#define HEADER_MARGIN_LEFT 15
#define HEADER_SETTINGS_LEFT 5
#define HEADER_MARGIN_BOTTOM 10
#define HEADER_TEXT_MARGIN ((HEADER_HEIGHT - 10)/2)
#define HEADER_TEXT_HEIGHT 12
#define HEADER_PAGE_INDICATOR_RAD 3
#define HEADER_SEL_PAGE_INDICATOR_RAD 4
#define HEADER_PAGE_INDICATOR_MARGIN ((HEADER_HEIGHT-HEADER_SEL_PAGE_INDICATOR_RAD*2)/2)
#define HEADER_PAGE_INDICATOR_SPACING 20
#define HEADER_ICON_SPACING 40
#define HEADER_ICON_WIDTH 24
#define HEADER_ICON_HEIGHT 24
#define HEADER_ICON_MARGIN 2
#define HEADER_TOUCH_MARGIN 3
#define HEADER_ICON_INDICATOR_MARGIN 10


//Row parameters
#define ENTITIES_ROW_HEIGHT 40
#define ROW_TEXT_FONT FSS9
#define ROW_TEXT_HEIGHT 9
#define ROW_SWITCH_HEIGHT 40
#define ROW_MARGIN_LEFT 20
#define ROW_SWITCH_STATE_ICON_WIDTH 40
#define ROW_SWITCH_STATE_ICON_HEIGHT 20
#define ROW_FAN_STATE_ICON_HEIGHT 40
#define ROW_FAN_STATE_ICON_WIDTH 161
#define ROW_MARGIN_RIGHT 10
#define ROW_ICON_WIDTH 24
#define ROW_ICON_HEIGHT 24
#define ROW_TEXT_MARGIN_LEFT 5
#define ROW_TEXT_MARGIN_BOTTOM 25

enum ScreenPageType {
  ScreenPageBoot,
  ScreenPageSetup,
  ScreenPageScreenSaver,
  #ifndef FIRMWARE_MINIMAL
  ScreenPageMain,
  ScreenPageEntity,
  ScreenPageSettings,
  #endif
};

enum ScreenOrientation {
  ScreenPortrait = 0,
  ScreenLandscapeInverted = 1,
  ScreenPortraitInverted = 2,
  ScreenLandscape = 3,
  ScreenOrientationInvalid = 4
};

struct TFTConfig {
  ScreenConfig * mainScreenConfig;
  ScreenConfig * settingsScreenConfig;
  ScreenConfig * entityScreenConfig;
  ScreenConfig * setupScreenConfig;
  ScreenConfig * screenSaverScreenConfig;
};

#define DEBUG_TOUCH_REGION 0

void tft_lcd_setup(ScreenConfig * mainScreenConfig, ScreenConfig * settingsScreenConfig);
void buildHeaderCanvas (TFTConfig * tftConfig, Canvas headerCanvas);
void tftSetScreenOrientation (ScreenOrientation rotation);
ScreenConfig * getScreenConfig (TFTConfig * tftConfig);
uint32_t buildBodyCanvas (TFTConfig * tftConfig, Canvas * bodyCanvas);
uint32_t drawScreen();
uint32_t buildScreenCanvas(Canvas * screenCanvasG);
void buildHeaderCanvas (TFTConfig * tftConfig, Canvas * headerCanvas);
uint32_t setScreenCanvasState (Canvas * screenCanvasG);
void setHeaderCanvasState(TFTConfig * tftConfig, Canvas * headerCanvas);
bool handleTouch(TouchEvent event, TouchEventData eventData);
 
uint16_t convert2rgb565 (uint32_t color);
uint32_t tft_set_page_num (uint8_t page_num);

#ifndef FIRMWARE_MINIMAL
bool handle_page_value_change (PageSelectorCanvas *canvas, uint16_t selectedPage, uint16_t prevSelectedPage);
bool handle_touch_header_page(Canvas *canvas, TouchEvent event, TouchEventData eventData);
bool handle_touch_header_setttings(Canvas *canvas, TouchEvent event, TouchEventData eventData);
#endif

#endif
