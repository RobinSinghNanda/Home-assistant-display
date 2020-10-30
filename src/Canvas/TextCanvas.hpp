#ifndef TEXT_CANVAS_H_
#define TEXT_CANVAS_H_

#include "TFT_eSPI.h"
#include "Arduino.h"
#include "Canvas.hpp"
#include "Free_Fonts.h"

#define TEXT_CANVAS_DEFAULT_FONT FSS9

enum TextFontType {
    FONT_INVALID,
    FONT_INT,
    FONT_GFX,
    FONT_STRING
};

class TextCanvas : public Canvas {
    public:
        TextCanvas(Canvas * canvas, uint16_t id);
        void setFont(uint8_t font);
        void setFont(const GFXfont *font);
        void setFont(String font);
        void setText(String text);
        String getText();
        uint8_t getTextHeight();
    protected:
      uint16_t textHeight = 9;
      String text = "";
      TextFontType fontType = FONT_INVALID;
      const GFXfont *font;
      String stringFont;
      virtual bool draw();
      uint16_t getCursorX(String str);
      uint16_t getCursorY();
};

#endif
