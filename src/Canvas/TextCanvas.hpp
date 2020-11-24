#ifndef TEXT_CANVAS_H_
#define TEXT_CANVAS_H_

#include "TFT_eSPI.h"
#include "Arduino.h"
#include "Canvas.hpp"

#define TEXT_CANVAS_DEFAULT_FONT FSS9

enum TextFontType {
    FONT_INVALID,
    FONT_INT,
    FONT_STRING
};

class TextCanvas : public Canvas {
    public:
        TextCanvas(Canvas * canvas, uint16_t id);
        void setFont(uint8_t font);
        void setFont(String font);
        void setText(String text);
        inline const char * getFont();
        String getText();
        uint8_t getTextHeight();
        void resize();
        inline void setMaxWidth(uint16_t width);
        inline uint16_t getMaxWidth();
    protected:
      uint16_t maxWidth = 0;
      uint16_t textHeight = 9;
      String text = "";
      TextFontType fontType = FONT_INVALID;
      String stringFont = "";
      virtual bool draw();
      uint16_t getCursorX(const char * str);
      uint16_t getCursorY();
};

void TextCanvas::setMaxWidth(uint16_t width) {
    invalidateIfNotEqual(this->maxWidth, width);
}

uint16_t TextCanvas::getMaxWidth() {
    return this->maxWidth;
}

const char * TextCanvas::getFont() {
    return this->stringFont.c_str();
}

#endif
