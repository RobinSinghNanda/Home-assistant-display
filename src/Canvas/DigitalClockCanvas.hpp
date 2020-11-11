#ifndef __DIGITALCLOCKCANVAS_H__
#define __DIGITALCLOCKCANVAS_H__

#include "TextCanvas.hpp"

enum TimeFormat{
    TimeFormat12Hour,
    TimeFormat24Hour
};

class DigitalClockCanvas : public Canvas {
    public:
        DigitalClockCanvas(Canvas * canvas, uint16_t id);
        ~DigitalClockCanvas();
        void setHour(uint8_t hour);
        void setMinute(uint8_t minute);
        void setSecond(uint8_t second);
        void setTime(uint8_t hour, uint8_t minute, uint8_t second);
        void setDisplaySecond(bool display);
        void setDisplayDate(bool display);
        void setTimeFormat(TimeFormat timeFormat);
        uint8_t getHour();
        uint8_t getMinute();
        uint8_t getSecond();
        bool isSecondDisplayed();
        bool isDateDisplayed();
        TimeFormat getTimeFormat();
        void setBlink(bool blink);
        bool getBlink();
        void setValid(bool valid);
        bool getValid();
        void setDate(const char * date);
        const char * getDate();
    protected:
        uint8_t minute;
        uint8_t hour;
        uint8_t second;
        uint8_t prevMinute = 0xFF;
        uint8_t prevHour = 0xFF;
        uint8_t prevSecond = 0xFF;
        uint8_t displaySecond=false;
        bool displayDate=true;
        TimeFormat timeFormat = TimeFormat24Hour;
        bool blink = true;
        bool valid = false;
        uint16_t textWidth = 0;
        uint8_t digitWidth = 0;
        uint8_t digitHeight = 0;
        uint8_t colonWidth = 0;
        uint8_t digitWidthPlus = 0;
        uint8_t digitHeightPlus = 0;
        uint8_t colonWidthPlus = 0;
        bool once = true;
        String stringFont = "7segment";
        char dateString[13];
        void setText();
        virtual bool draw();
        uint16_t getCursorY(uint16_t textHeight);
        uint16_t getCursorX(uint16_t textWidth);
};

#endif // __DIGITALCLOCKCANVAS_H__