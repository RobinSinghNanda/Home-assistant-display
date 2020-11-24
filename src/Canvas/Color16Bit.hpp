#ifndef __COLORRGB565_H__
#define __COLORRGB565_H__
#include "Color.hpp"
#include <stdint.h>

union Color;
class Color16Bit;
typedef Color16Bit ColorRGB565;

class Color16Bit {
    public:
        Color16Bit();
        Color16Bit(uint16_t color);
        Color16Bit(uint8_t red, uint8_t green, uint8_t blue);
        Color get32Bit();
        operator uint16_t () const;
        operator Color () const;
        bool operator == (Color16Bit &color);
        bool operator == (uint16_t color);
        bool operator != (Color16Bit &color);
        bool operator != (uint16_t color);
        inline uint8_t getRed();
        inline uint8_t getGreen();
        inline uint8_t getBlue();
    protected: 
        uint16_t color;
};

uint8_t Color16Bit::getRed() {
    return (color>>8)&0xF8;
}

uint8_t Color16Bit::getGreen() {
    return (color>>3)&0xFC;
}

uint8_t Color16Bit::getBlue() {
    return (color<<3)&0xF8;
}

#endif // __COLORRGB565_H__