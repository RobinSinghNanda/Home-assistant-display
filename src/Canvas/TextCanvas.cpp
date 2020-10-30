#include "TextCanvas.hpp"

TextCanvas::TextCanvas(Canvas * canvas, uint16_t id) : Canvas (canvas, id) {
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
  this->tft->fillRect(this->x, this->y, this->width, this->height, this->bgColor);
  if (this->getDrawableWidth() == 0 || this->getDrawableHeight() == 0) {
    return false;
  }
  if (this->text == "") {
    return false;
  }
  if (textHeight > this->getDrawableHeight()) {
    return false;
  }
  if (fontType == FONT_STRING) {
    tft->setTextColor(this->fgColor, this->bgColor);
    tft->loadFont(this->stringFont);
  } else {
    return false;
  }
  uint16_t textWidth = tft->textWidth(text);
  if (textWidth > this->getDrawableWidth()) {
    bool widthReduced = false;
    uint8_t str_length = text.length();
    char * carr = new char [str_length + 1];
    strncpy(carr, text.c_str(), str_length + 1);
    for (uint8_t i=str_length-1;i > 0;i--) {
      carr[i] = 0;
      textWidth = tft->textWidth(carr);
      if (textWidth <= this->getDrawableWidth()) {
        widthReduced = true;
        break;
      }
    }
    if (!widthReduced) {
      Serial.println("Failed to reduce the size");
      this->redrawChildren();
      return false;
    }
    String reducedText = String(carr);
    tft->setCursor(getCursorX(reducedText), getCursorY());
    tft->print(carr);
    return true;
  }
  tft->setCursor(getCursorX(text), getCursorY());
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

uint16_t TextCanvas::getCursorX(String str) {
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