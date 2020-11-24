#ifndef __COLOR_H__
#define __COLOR_H__
#include "Color16Bit.hpp"
#include <stdint.h>

union Color;
class Color16Bit;

typedef Color ColorRGB888;
typedef Color Color32Bit;

union Color {
    Color();
    Color(uint32_t color);
    Color(uint8_t red, uint8_t green, uint8_t blue);
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    uint32_t color;
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
    Color16Bit get16Bit();
    operator uint32_t () const;
    operator Color16Bit () const;
};

#endif // __COLOR_H__