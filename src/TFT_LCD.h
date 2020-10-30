#ifndef TFT_LCD_H
#define TFT_LCD_H

#include <TFT_eSPI.h> 

#include "ScreenConfig.hpp"
#include "Canvas/Canvases.hpp"
#include "HomeAssistant.hpp"

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

struct ScreenInfo {
  int8_t curr_page_num;
  int8_t prev_page_num;
};

enum TFTConnectionState {
  TFT_FirstSetup,
  TFT_WifiDisconnected,
  TFT_WifiConnected,
  TFT_WifiAlert,
  TFT_HaConnected,
  TFT_HaSyncing
};

struct TFTConfig {
  ScreenConfig * mainScreenConfig;
  ScreenConfig * settingsScreenConfig;
  ScreenConfig * entityScreenConfig;
  ScreenConfig * firstSetupScreenConfig;
  uint32_t require_update = 1;
  uint32_t screen_refresh = 1;
  int8_t curr_page_num;
  int8_t prev_page_num=-1;
  int8_t settings_page=0;
  bool first_setup = 0;
  TFTConnectionState connectionState;
  int8_t dark_mode = 1;
  int8_t bottom_header = 0;
  int8_t rotation = 3;
  int8_t prev_rotation = 3;
  int8_t entityPage = 0;
};


#define DEBUG_TOUCH_REGION 0

struct Image {
  String path = "";
  uint16_t color = 0xFFFF;
  uint16_t invert = 0;
  uint16_t width = 0;
  uint16_t height = 0;
};

struct Text {
  String text = "";
  uint16_t color = 0x0000;
};

struct TouchBox {
  uint16_t touch_x;
  uint16_t touch_y;
  uint16_t touch_abs_x;
  uint16_t touch_abs_y;
  uint16_t box_x;
  uint16_t box_y;
  uint16_t box_width;
  uint16_t box_height;
};

void tft_lcd_setup(ScreenConfig * mainScreenConfig, ScreenConfig * settingsScreenConfig);
void buildHeaderCanvas (TFTConfig * tftConfig, Canvas headerCanvas);
void tft_lcd_setup(ScreenConfig * mainScreenConfig, ScreenConfig * settingsScreenConfig);
void set_screen_rotation (int8_t rotation);
uint32_t drawRow (TFTConfig * tftConfig, BaseRowConfig * row, Canvas * rowCanvas, Text &name, Image &icon, Image &state, Text &stateText);
ScreenConfig * getScreenConfig (TFTConfig * tftConfig);
uint32_t buildBodyCanvas (TFTConfig * tftConfig, Canvas * bodyCanvas);
uint32_t drawScreen();
uint32_t buildScreenCanvas(Canvas * screenCanvasG);
void buildHeaderCanvas (TFTConfig * tftConfig, Canvas * headerCanvas);
uint32_t setScreenCanvasState (Canvas * screenCanvasG);
void setHeaderCanvasState(TFTConfig * tftConfig, Canvas * headerCanvas);
bool handle_touch_header_setttings(Canvas *canvas, TouchEvent event, TouchEventData eventData);
bool handle_page_value_change (PageSelectorCanvas *canvas, uint16_t selectedPage, uint16_t prevSelectedPage);
bool handle_touch_header_page(Canvas *canvas, TouchEvent event, TouchEventData eventData);
bool handle_touch(TouchEvent event, TouchEventData eventData);
void set_row_state (String entity_id, String state);
String get_row_state (String entity_id);
String get_row_attribute (String entity_id, String attribute_name);
void set_row_attribute (String entity_id, String attribute_name, String attribute_value);
 
uint16_t convert2rgb565 (uint32_t color);
void tft_require_update ();
void tft_screen_refresh ();
void tft_set_dark_mode(uint8_t dark_mode);
void tft_set_connection_state (TFTConnectionState state);
void tft_set_bottom_bar (uint8_t state);
uint32_t tft_set_page_num (uint8_t page_num);
void tft_set_settings_page (uint8_t state);
void tft_set_first_setup_page (uint8_t state);
#endif
