#include "Color.hpp"

Color::Color() {
    this->color = 0;
}

Color::Color(uint32_t color) {
    this->color = color;
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) : Color(red, green, blue, 0) {
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}

Color::operator uint32_t () const {
    return color;
}

Color::operator Color16Bit () const {
    return Color16Bit(red, green, blue);
}

Color16Bit Color::get16Bit() {
    return Color16Bit(red, green, blue);
}