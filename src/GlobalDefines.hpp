#pragma once
#include "stdint.h"

#ifndef DARK_WEB_THEME
#define DARK_WEB_THEME 1
#endif

#if DARK_WEB_THEME
#ifndef COLOR_TEXT
#define COLOR_TEXT                  0xeaeaea    // [WebColor1] Global text color - Very light gray
#endif
#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND            0x252525    // [WebColor2] Global background color - Very dark gray (mostly black)
#endif
#ifndef COLOR_FORM
#define COLOR_FORM                  0x4f4f4f    // [WebColor3] Form background color - Very dark gray
#endif
#ifndef COLOR_INPUT_TEXT
#define COLOR_INPUT_TEXT            0x000000    // [WebColor4] Input text color - Black
#endif
#ifndef COLOR_INPUT
#define COLOR_INPUT                 0xd0d0d0    // [WebColor5] Input background color - Very light gray
#endif
#ifndef COLOR_CONSOLE_TEXT
#define COLOR_CONSOLE_TEXT          0x65c115    // [WebColor6] Console text color - Strong Green
#endif
#ifndef COLOR_CONSOLE
#define COLOR_CONSOLE               0x1f1f1f    // [WebColor7] Console background color - Very dark gray (mostly black)
#endif
#ifndef COLOR_TEXT_WARNING
#define COLOR_TEXT_WARNING          0xff5661    // [WebColor8] Warning text color - Brick Red
#endif
#ifndef COLOR_TEXT_SUCCESS
#define COLOR_TEXT_SUCCESS          0x008000    // [WebColor9] Success text color - Dark lime green
#endif
#ifndef COLOR_BUTTON_TEXT
#define COLOR_BUTTON_TEXT           0xfaffff    // [WebColor10] Button text color - Very pale (mostly white) cyan
#endif
#ifndef COLOR_BUTTON
#define COLOR_BUTTON                0x1fa3ec    // [WebColor11] Button color - Vivid blue
#endif
#ifndef COLOR_BUTTON_HOVER
#define COLOR_BUTTON_HOVER          0x0e70a4    // [WebColor12] Button color when hovered over - Dark blue
#endif
#ifndef COLOR_BUTTON_RESET
#define COLOR_BUTTON_RESET          0xd43535    // [WebColor13] Restart/Reset/Delete button color - Strong red
#endif
#ifndef COLOR_BUTTON_RESET_HOVER
#define COLOR_BUTTON_RESET_HOVER    0x931f1f    // [WebColor14] Restart/Reset/Delete button color when hovered over - Dark red
#endif
#ifndef COLOR_BUTTON_SAVE
#define COLOR_BUTTON_SAVE           0x47c266    // [WebColor15] Save button color - Moderate lime green
#endif
#ifndef COLOR_BUTTON_SAVE_HOVER
#define COLOR_BUTTON_SAVE_HOVER     0x5aaf6f    // [WebColor16] Save button color when hovered over - Dark moderate lime green
#endif
#ifndef COLOR_TIMER_TAB_TEXT
#define COLOR_TIMER_TAB_TEXT        0xfaffff    // [WebColor17] Config timer tab text color - Very pale (mostly white) cyan.
#endif
#ifndef COLOR_TIMER_TAB_BACKGROUND
#define COLOR_TIMER_TAB_BACKGROUND  0x909090    // [WebColor18] Config timer tab background color - Dark gray
#endif
#ifndef COLOR_TITLE_TEXT
#define COLOR_TITLE_TEXT            0xeaeaea    // [WebColor19] Title text color - Very light gray
#endif
#else 
#ifndef COLOR_TEXT
#define COLOR_TEXT                  0x000000     // Global text color - Black
#endif
#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND            0xFFFFFF     // Global background color - White
#endif
#ifndef COLOR_FORM
#define COLOR_FORM                  0xf2f2f2  // Form background color - Greyish
#endif
#ifndef COLOR_INPUT_TEXT
#define COLOR_INPUT_TEXT            0x000000     // Input text color - Black
#endif
#ifndef COLOR_INPUT
#define COLOR_INPUT                 0xFFFFFF     // Input background color - White
#endif
#ifndef COLOR_CONSOLE_TEXT
#define COLOR_CONSOLE_TEXT          0x000000     // Console text color - Black
#endif
#ifndef COLOR_CONSOLE
#define COLOR_CONSOLE               0xFFFFFF     // Console background color - White
#endif
#ifndef COLOR_TEXT_WARNING
#define COLOR_TEXT_WARNING          0xff0000     // Warning text color - Red
#endif
#ifndef COLOR_TEXT_SUCCESS
#define COLOR_TEXT_SUCCESS          0x008000  // Success text color - Green
#endif
#ifndef COLOR_BUTTON_TEXT
#define COLOR_BUTTON_TEXT           0xFFFFFF     // Button text color - White
#endif
#ifndef COLOR_BUTTON
#define COLOR_BUTTON                0x1fa3ec  // Button color - Blueish
#endif
#ifndef COLOR_BUTTON_HOVER
#define COLOR_BUTTON_HOVER          0x0e70a4  // Button color when hovered over - Darker blueish
#endif
#ifndef COLOR_BUTTON_RESET
#define COLOR_BUTTON_RESET          0xd43535  // Restart/Reset/Delete button color - Redish
#endif
#ifndef COLOR_BUTTON_RESET_HOVER
#define COLOR_BUTTON_RESET_HOVER    0x931f1f  // Restart/Reset/Delete button color when hovered over - Darker redish
#endif
#ifndef COLOR_BUTTON_SAVE
#define COLOR_BUTTON_SAVE           0x47c266  // Save button color - Greenish
#endif
#ifndef COLOR_BUTTON_SAVE_HOVER
#define COLOR_BUTTON_SAVE_HOVER     0x5aaf6f  // Save button color when hovered over - Darker greenish
#endif
#ifndef COLOR_TIMER_TAB_TEXT
#define COLOR_TIMER_TAB_TEXT        0xFFFFFF     // Config timer tab text color - White
#endif
#ifndef COLOR_TIMER_TAB_BACKGROUND
#define COLOR_TIMER_TAB_BACKGROUND  0x909090     // Config timer tab background color - Light grey
#endif
#ifndef COLOR_TITLE_TEXT
#define COLOR_TITLE_TEXT			      COLOR_TEXT // Title text color defaults to global text color either dark or light
#endif
#endif

#ifndef COLOR_DARK_LCD_BODY_BACKGROUND
#define COLOR_DARK_LCD_BODY_BACKGROUND    0x000000
#endif
#ifndef COLOR_DARK_LCD_BODY_TEXT
#define COLOR_DARK_LCD_BODY_TEXT    0xFFFFFF
#endif
#ifndef COLOR_DARK_LCD_HEADER_BACKGROUND
#define COLOR_DARK_LCD_HEADER_BACKGROUND    0x1f1f1f
#endif
#ifndef COLOR_DARK_LCD_HEADER_TEXT
#define COLOR_DARK_LCD_HEADER_TEXT    0xFFFFFF
#endif
#ifndef COLOR_DARK_LCD_HEADER_ICON
#define COLOR_DARK_LCD_HEADER_ICON    COLOR_DARK_LCD_HEADER_TEXT
#endif
#ifndef COLOR_DARK_LCD_PAGE_INDICATOR
#define COLOR_DARK_LCD_PAGE_INDICATOR    0xFFFFFF
#endif
#ifndef COLOR_DARK_LCD_PAGE_INDICATOR_SELECTED
#define COLOR_DARK_LCD_PAGE_INDICATOR_SELECTED    0x03a9f4
#endif
#ifndef COLOR_DARK_LCD_SS_BACKGROUND
#define COLOR_DARK_LCD_SS_BACKGROUND    0x000000
#endif
#ifndef COLOR_DARK_LCD_SS_TEXT
#define COLOR_DARK_LCD_SS_TEXT    0xFFFFFF
#endif
#ifndef COLOR_DARK_LCD_SS_HEADER_ICON
#define COLOR_DARK_LCD_SS_HEADER_ICON    0x7F7F7F
#endif
#ifndef COLOR_DARK_LCD_ENTITY_ICON
#define COLOR_DARK_LCD_ENTITY_ICON    0x44739e
#endif
#ifndef COLOR_DARK_LCD_ENTITY_ICON_UNAVAILABLE
#define COLOR_DARK_LCD_ENTITY_ICON_UNAVAILABLE    0x6f6f6f
#endif
#ifndef COLOR_DARK_LCD_LCD_ENTITY_ICON_ENABLED
#define COLOR_DARK_LCD_LCD_ENTITY_ICON_ENABLED    0xfdd835
#endif
#ifndef COLOR_DARK_SWITCH_SECONDARY
#define COLOR_DARK_SWITCH_SECONDARY         0x03a9f4
#endif
#ifndef COLOR_DARK_SWITCH_SURFACE
#define COLOR_DARK_SWITCH_SURFACE           0x999999
#endif
#ifndef COLOR_DARK_SWITCH_ON_SURFACE
#define COLOR_DARK_SWITCH_ON_SURFACE        0xFFFFFF
#endif

#ifndef COLOR_LIGHT_LCD_BODY_BACKGROUND
#define COLOR_LIGHT_LCD_BODY_BACKGROUND    0xFFFFFF
#endif
#ifndef COLOR_LIGHT_LCD_BODY_TEXT
#define COLOR_LIGHT_LCD_BODY_TEXT    0x000000
#endif
#ifndef COLOR_LIGHT_LCD_HEADER_BACKGROUND
#define COLOR_LIGHT_LCD_HEADER_BACKGROUND    0x03a9f4
#endif
#ifndef COLOR_LIGHT_LCD_HEADER_TEXT
#define COLOR_LIGHT_LCD_HEADER_TEXT    0xFFFFFF
#endif
#ifndef COLOR_LIGHT_LCD_HEADER_ICON
#define COLOR_LIGHT_LCD_HEADER_ICON    COLOR_LIGHT_LCD_HEADER_TEXT
#endif
#ifndef COLOR_LIGHT_LCD_PAGE_INDICATOR
#define COLOR_LIGHT_LCD_PAGE_INDICATOR    0xFFFFFF
#endif
#ifndef COLOR_LIGHT_LCD_PAGE_INDICATOR_SELECTED
#define COLOR_LIGHT_LCD_PAGE_INDICATOR_SELECTED    0x000000
#endif
#ifndef COLOR_LIGHT_LCD_SS_BACKGROUND
#define COLOR_LIGHT_LCD_SS_BACKGROUND    COLOR_LIGHT_LCD_HEADER_BACKGROUND
#endif
#ifndef COLOR_LIGHT_LCD_SS_TEXT
#define COLOR_LIGHT_LCD_SS_TEXT    0x000000
#endif
#ifndef COLOR_LIGHT_LCD_SS_HEADER_ICON
#define COLOR_LIGHT_LCD_SS_HEADER_ICON    0x7F7F7F
#endif
#ifndef COLOR_LIGHT_LCD_ENTITY_ICON
#define COLOR_LIGHT_LCD_ENTITY_ICON    0x44739e
#endif
#ifndef COLOR_LIGHT_LCD_ENTITY_ICON_UNAVAILABLE
#define COLOR_LIGHT_LCD_ENTITY_ICON_UNAVAILABLE    0xbdbdbd
#endif
#ifndef COLOR_LIGHT_LCD_LCD_ENTITY_ICON_ENABLED
#define COLOR_LIGHT_LCD_LCD_ENTITY_ICON_ENABLED    0xfdd835
#endif
#ifndef COLOR_LIGHT_SWITCH_SECONDARY
#define COLOR_LIGHT_SWITCH_SECONDARY        COLOR_LIGHT_LCD_HEADER_BACKGROUND
#endif
#ifndef COLOR_LIGHT_SWITCH_SURFACE
#define COLOR_LIGHT_SWITCH_SURFACE          0xFFFFFF
#endif
#ifndef COLOR_LIGHT_SWITCH_ON_SURFACE
#define COLOR_LIGHT_SWITCH_ON_SURFACE       0x000000
#endif

const uint16_t WEB_LOG_SIZE = 4000;
#ifndef SCREEN_SAVER_TIME
#define SCREEN_SAVER_TIME 120000
#endif

#ifndef SCREEN_SAVER_ENABLE
#define SCREEN_SAVER_ENABLE 1
#endif