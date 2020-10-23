#ifndef IMAGE_CANVAS_H_
#define IMAGE_CANVAS_H_

#include "Canvas.hpp"
#include "Arduino.h"

#define LIGHT_THEME_ICON_COLOR 0x632C
#define DARK_THEME_ICON_COLOR  0xE71C

enum ImageType {
  ImageTypeJpg,
  ImageTypeBin
};

class ImageCanvas : public Canvas {
  public:
    ImageCanvas(Canvas * canvas, uint16_t id);
    void setPath(String path);
    String getPath();
    void setColorMask(uint16_t colorMask);
    uint16_t getColorMask();
    void setColorInvert(bool colorInvert);
    bool getColorInvert();
    void setAutoInvert(bool autoInvert);
    bool getAutoInvert();
    void setDarkMode(bool darkMode);
  protected:
    String path;
    uint16_t colorMask;
    bool colorInvert;
    bool autoInvert = false;
    ImageType imageType;
    void renderJPEG();
    void renderBin();
    virtual bool draw();
    bool draw(String path);
    bool draw(String path, uint16_t colorMask, uint8_t colorInvert);
};

#endif