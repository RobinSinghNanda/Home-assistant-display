#ifndef TEXT_CANVAS_H_
#define TEXT_CANVAS_H_

#include "TFT_eSPI.h"
#include "Arduino.h"
#include "Canvas.hpp"
#include "Free_Fonts.h"

#define TEXT_CANVAS_DEFAULT_FONT FSS9

class TextCanvas : public Canvas {
    public:
        TextCanvas(Canvas * canvas, uint16_t id);
        void setFont(uint8_t font);
        void setFont(const GFXfont *font);
        void setText(String text);
        String getText();
    protected:
      String text = "";
      const GFXfont *font;
      virtual bool draw();
      uint16_t getCursorX(String str);
      uint16_t getCursorY();
};

#endif
