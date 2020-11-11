#ifndef IMAGE_CANVAS_H_
#define IMAGE_CANVAS_H_

#include "Canvas.hpp"
#include "Arduino.h"

#define LIGHT_THEME_ICON_COLOR 0x632C
#define DARK_THEME_ICON_COLOR  0xE71C

#define MASK_RB       63519 // 0b1111100000011111
#define MASK_G         2016 // 0b0000011111100000
#define MASK_MUL_RB 4065216 // 0b1111100000011111000000
#define MASK_MUL_G   129024 // 0b0000011111100000000000
#define MAX_ALPHA        64 // 6bits+1 with rounding

enum ImageType {
  ImageTypeJpg,
  ImageTypeBin
};

class ImageCanvas : public Canvas {
  public:
    ImageCanvas(Canvas * canvas, uint16_t id);
    void setPath(String path);
    String getPath();
    void setDarkMode(bool darkMode);
    void setMaskColor(uint16_t maskColor);
    uint16_t getMaskColor();
  protected:
    String path;
    ImageType imageType;
    uint16_t maskColor;
    void renderJPEG();
    void renderBin();
    virtual bool draw();
    bool draw(String path);
    bool draw(String path, uint16_t colorMask, uint8_t colorInvert);
    uint16_t alphaBlend(uint16_t fg, uint16_t bg, uint8 alpha);
};

#endif