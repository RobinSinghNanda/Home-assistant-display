#include "Color16Bit.hpp"
#include <Arduino.h>

Color16Bit::Color16Bit() {
    this->color = 0;
}

Color16Bit::Color16Bit(uint16_t color) {
    this->color = color;
}

Color16Bit::Color16Bit(uint8_t red, uint8_t green, uint8_t blue) {
    this->color = ((red&0xF8)<<8)|((green&0xFC)<<3)|((blue&0xF8)>>3);
}

Color16Bit::operator uint16_t () const {
    return color;
}

Color16Bit::operator Color () const {
    return Color((color>>8)&0xF8, (color>>3)&0xFC, (color<<3)&0xF8);
}

bool Color16Bit::operator == (Color16Bit &color) {
    return this->color == color.color;
}

bool Color16Bit::operator == (uint16_t color) {
    return this->color == color;
}

bool Color16Bit::operator != (Color16Bit &color) {
    return this->color != color.color;
}

bool Color16Bit::operator != (uint16_t color) {
    return this->color != color;
}