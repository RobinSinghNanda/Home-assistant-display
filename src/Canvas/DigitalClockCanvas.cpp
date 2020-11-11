#include "DigitalClockCanvas.hpp"

DigitalClockCanvas::DigitalClockCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {
    this->minute = 0;
    this->second = 0;
    this->hour = 0;
    this->displaySecond = true;
    this->displayDate = true;
    this->blink = true;
    tft->setTextColor(this->fgColor, this->bgColor);
    tft->loadFont("Roboto-Medium");
    this->digitWidth = tft->textWidth("4");
    this->colonWidth = tft->textWidth(":");
    this->digitHeight = tft->fontHeight();
    tft->loadFont("Roboto-MediumPlus");
    this->digitWidthPlus = tft->textWidth("4");
    this->colonWidthPlus = tft->textWidth(":");
    this->digitHeightPlus = tft->fontHeight();
    this->timeFormat = TimeFormat24Hour;
    dateString[0] = '\0';
}

DigitalClockCanvas::~DigitalClockCanvas() {

}

void DigitalClockCanvas::setHour(uint8_t hour) {
    invalidateIfNotEqual(this->hour, hour);
    if (hour > 24) 
        return;
    this->hour = hour;
}

void DigitalClockCanvas::setMinute(uint8_t minute) {
    invalidateIfNotEqual(this->minute, minute);
    if (minute > 60) 
        return;
    this->minute = minute;
}

void DigitalClockCanvas::setSecond(uint8_t second) {
    invalidateIfNotEqual(this->second, second);
    if (second > 60) 
        return;
    this->second = second;
}

void DigitalClockCanvas::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
    setHour(hour);
    setMinute(minute);
    setSecond(second);
}

void DigitalClockCanvas::setDisplaySecond(bool display) {
    invalidateIfNotEqual(this->displaySecond, display);
    this->displaySecond = display;
}

void DigitalClockCanvas::setDisplayDate(bool display) {
    invalidateIfNotEqual(this->displayDate, display);
    this->displayDate = display;
}

void DigitalClockCanvas::setTimeFormat(TimeFormat timeFormat) {
    invalidateIfNotEqual(this->timeFormat, timeFormat);
    this->timeFormat = timeFormat;
}

uint8_t DigitalClockCanvas::getHour() {
    return this->hour;
}

uint8_t DigitalClockCanvas::getMinute() {
    return this->minute;
}

uint8_t DigitalClockCanvas::getSecond() {
    return this->second;
}

bool DigitalClockCanvas::isSecondDisplayed() {
    return this->displaySecond;
}

bool DigitalClockCanvas::isDateDisplayed() {
    return this->displayDate;
}

TimeFormat DigitalClockCanvas::getTimeFormat() {
    return this->timeFormat;
}

void DigitalClockCanvas::setBlink(bool blink) {
    invalidateIfNotEqual(this->blink, blink);
    this->blink = blink;
}

bool DigitalClockCanvas::getBlink() {
    return this->blink;
}

void DigitalClockCanvas::setDate(const char * date) {
    strncpy(this->dateString, date, sizeof(this->dateString)-1);
    this->dateString[13] = '\0';
}

const char * DigitalClockCanvas::getDate() {
    return this->dateString;
}

bool DigitalClockCanvas::draw() {
    if (!valid) {
        prevHour = 0xFF;
        prevMinute = 0xFF;
        prevSecond = 0xFF;
        tft->drawRect(this->getDrawX(), this->getDrawY(), this->getDrawableWidth(), this->getDrawableHeight(), bgColor);
        once = true;
        return true;
    }
    if (once) {
        tft->fillRect(this->getX(), this->getY(), this->getWidth(),this->getHeight(), bgColor);
        once = false;
    }
    tft->setTextColor(this->fgColor, this->bgColor);
    uint16_t digitHeight = this->digitHeight;
    uint16_t colonWidth = this->colonWidth;
    uint16_t textSpacing = colonWidth>>1;
    uint16_t colonOffset = 0;
    uint16_t digitWidth = this->digitWidth;
    if (!displaySecond) {
        digitHeight = digitHeightPlus;
        colonWidth = colonWidthPlus;
        digitWidth = digitWidthPlus;
        textSpacing = colonWidth>>1;
        colonOffset=2;
        tft->loadFont("Roboto-MediumPlus");
    } else {
        tft->loadFont("Roboto-Medium");
    }
    uint16_t textWidth = 4*digitWidth+colonWidth+2*textSpacing;
    uint16_t textHeight = digitHeight;
    if (displaySecond) {
        textWidth += 2*digitWidth+2*colonWidth+2*textSpacing;
    }
    if (displayDate) {
        textHeight += 24+10;
    }
    int16_t prevWidth = 0;
    if (prevHour != hour) {
        uint8_t foramatedHour = hour;
        if (this->timeFormat==TimeFormat12Hour && hour >= 12) {
            foramatedHour -= 12;
            if (foramatedHour == 12) {
                foramatedHour = 0;
            }
        }
        char hourString [3];
        snprintf(hourString, 3, "%02u", foramatedHour);
        tft->fillRect(this->getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), 2*digitWidth+textSpacing, digitHeight, bgColor);
        // tft->drawRect(this->getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), 2*digitWidth+textSpacing, digitHeight, 0xFF00);
        tft->setCursor(this->getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight));
        tft->print(hourString);
    }
    prevWidth += 2*digitWidth+textSpacing;
    if ((prevSecond != second && blink) || prevSecond == 0xFF) {
        tft->fillRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), colonWidth+textSpacing, digitHeight, bgColor);
        // tft->drawRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), colonWidth+textSpacing, digitHeight, 0x00FF);
        tft->setCursor(getCursorX(textWidth)+prevWidth+colonOffset, this->getCursorY(textHeight));
        if (blink) {
            if (second&0x1) {
                tft->print(':');
            } else {
                tft->print(' ');
            }
        } else {
            tft->print(':');
        }
    }
    prevWidth += colonWidth+textSpacing;
    if (prevMinute != minute) {
        char minuteString [3];
        snprintf(minuteString, 3, "%02u", minute);
        tft->fillRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), 2*digitWidth+textSpacing, digitHeight, bgColor);
        // tft->drawRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), 2*digitWidth+textSpacing, digitHeight, 0xFF00);
        tft->setCursor(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight));
        tft->print(minuteString);
    }
    prevWidth += 2*digitWidth+textSpacing;
    if (displaySecond) {
        if (prevSecond != second) {
            tft->fillRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), colonWidth+textSpacing, digitHeight, this->bgColor);
            // tft->drawRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), colonWidth+textSpacing, digitHeight, 0x00FF);
            tft->setCursor(getCursorX(textWidth)+prevWidth+colonOffset, this->getCursorY(textHeight));
            if (blink) {
                if (second&0x1) {
                    tft->print(':');
                } else {
                    tft->print(' ');
                }
            } else {
                tft->print(':');
            }
            prevWidth += colonWidth+textSpacing;
            tft->fillRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), 2*digitWidth+textSpacing, digitHeight, bgColor);
            // tft->drawRect(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight), 2*digitWidth+textSpacing, digitHeight, 0xFF00);
            char secondString [3];
            snprintf(secondString, 3, "%02u", second);
            tft->setCursor(getCursorX(textWidth)+prevWidth, this->getCursorY(textHeight));
            tft->print(secondString);
        }
    }
    if (displayDate) {
        tft->loadFont("Roboto-Regular24");
        uint16_t dateWidth = tft->textWidth(this->dateString);
        tft->setCursor(getCursorX(dateWidth), getCursorY(textHeight)+10+digitHeight);
        tft->print(dateString);
    }
    uint8_t foramatedHour = hour;
    if (this->timeFormat==TimeFormat12Hour && hour >= 12) {
        foramatedHour -= 12;
        if (foramatedHour == 12) {
            foramatedHour = 0;
        }
    }
    prevSecond = second;
    prevHour = hour;
    prevMinute = minute;
    return true;
}

uint16_t DigitalClockCanvas::getCursorX(uint16_t textWidth) {
  uint16_t cursorX = 0;
  if (hAlign == ALIGN_LEFT) {
    cursorX = this->getDrawX();
  } else if (hAlign == ALIGN_CENTER) {
    cursorX = this->getDrawX() + (this->getDrawableWidth()-textWidth)/2 ;
  } else if (hAlign == ALIGN_RIGHT) {
    cursorX = this->getDrawX() + (this->getDrawableWidth()-textWidth);
  }
  return cursorX;
}

uint16_t DigitalClockCanvas::getCursorY(uint16_t textHeight) {
  uint16_t cursorY = 0;
  if (vAlign == ALIGN_TOP) {
    cursorY = this->getDrawY();
  } else if (vAlign == ALIGN_MIDDLE) {
    cursorY = this->getDrawY() + 3 +(this->getDrawableHeight()-textHeight)/2 ;
  } else if (vAlign == ALIGN_BOTTOM) {
    cursorY = this->getDrawY() + (this->getDrawableHeight()-textHeight);
  }
  return cursorY;
}

void DigitalClockCanvas::setValid(bool valid) {
    invalidateIfNotEqual(this->valid, valid);
    this->valid = valid;
}

bool DigitalClockCanvas::getValid() {
    return this->valid;
}