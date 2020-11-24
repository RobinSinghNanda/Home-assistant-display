#include "TextCanvas.hpp"

TextCanvas::TextCanvas(Canvas * canvas, uint16_t id) : Canvas (canvas, id) {
  this->drawBackgroundEnable = true;
  this->stringFont = "";
}

void TextCanvas::setFont(String font) {
  if ((font == this->stringFont) && (fontType == FONT_STRING)) {
    return;
  }
  fontType = FONT_STRING;
  this->stringFont = font;
  this->invalidate();
}

bool TextCanvas::draw() {
  Canvas::draw();
  if (this->getDrawableWidth() == 0 || this->getDrawableHeight() == 0) {
    return false;
  }
  if (this->text == "") {
    
    return false;
  }
  if (textHeight > this->getDrawableHeight()) {
    return false;
  }
  tft->setTextColor(this->fgColor, this->bgColor);
  if (String(tft->fontFile.name()) != "/"+this->stringFont+".vlw") {
    tft->loadFont(this->stringFont);
  }
  uint16_t textWidth = tft->textWidth(text);
  if (textWidth > this->getDrawableWidth()) {
    bool widthReduced = false;
    char str[40];
    char originalStr[40];
    uint16_t length = strlen(text.c_str());
    snprintf(originalStr, sizeof(originalStr), "%s", text.c_str());
    while(true) {
      snprintf(str, sizeof(str), "%s...", originalStr);
      if (tft->textWidth(str) <= getMaxWidth()) {
        widthReduced = true;
        break;
      } else if (length == 0) {
        break;
      }
      originalStr[length-1] = '\0';
      length--;
    }
    if (!widthReduced) {
      this->redrawChildren();
      return false;
    }
    tft->setCursor(getCursorX(str), getCursorY());
    tft->print(str);
    return true;
  }
  tft->setCursor(getCursorX(text.c_str()), getCursorY());
  tft->print(text);
  return true;
}

void TextCanvas::setText(String text) {
    String prev_text =  this->text;
    this->text = text;
    if (prev_text != text) {
        this->invalidate();
    }
}

String TextCanvas::getText() {
    return this->text;
}

uint16_t TextCanvas::getCursorX(const char * str) {
  uint16_t cursorX = 0;
  uint16_t textWidth = tft->textWidth(str);
  if (hAlign == ALIGN_LEFT) {
    cursorX = this->getDrawX();
  } else if (hAlign == ALIGN_CENTER) {
    cursorX = this->getDrawX() + (this->getDrawableWidth()-textWidth)/2 ;
  } else if (hAlign == ALIGN_RIGHT) {
    cursorX = this->getDrawX() + (this->getDrawableWidth()-textWidth);
  }
  return cursorX;
}

uint16_t TextCanvas::getCursorY() {
  uint16_t cursorY = 0;
  if (vAlign == ALIGN_TOP) {
    cursorY = this->getDrawY();
  } else if (vAlign == ALIGN_MIDDLE) {
    cursorY = this->getDrawY() + 3 +(this->getDrawableHeight()-tft->fontHeight())/2 ;
  } else if (vAlign == ALIGN_BOTTOM) {
    cursorY = this->getDrawY() + (this->getDrawableHeight()-tft->fontHeight());
  }
  return cursorY;
}

uint8_t TextCanvas::getTextHeight() {
  return tft->fontHeight();
}

void TextCanvas::resize() {
  uint16_t textWidth = tft->textWidth(this->text);
  this->width = textWidth + marginLeft + marginRight + paddingRight + paddingLeft + borderLeft + borderRight;
  if (maxWidth && width > maxWidth) {
    width = maxWidth;
  }
}