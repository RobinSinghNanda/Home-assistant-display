#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include "Touch.h"
#include "Arduino.h"
#include <functional>
#include <TFT_eSPI.h> 
#include <JPEGDecoder.h>
using namespace std; 
#include <list> 

#define CANVAS_MAX_CHILDREN 10

//#define CANVAS_DEBUG 1

#define CANVAS_DEBUG_BORDER_COLOR (this->convert2rgb565(0xFFFF00))
#define CANVAS_DEBUG_MARGIN_BORDER_COLOR (this->convert2rgb565(0xFF00FF))

#define CANVAS_MAX_LEVELS 10

struct TftConfig;

class Canvas;

typedef std::function<bool(Canvas*, TouchEvent, TouchEventData)> CanvasTouchCallback;

#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
#define invalidateIfNotEqual(a,b) {if(a == b){return;}else{this->invalidate();}}
#define returnIfEqual(a,b) {if(a == b){return;}}

enum CanvasHorizontalAlignment {
  ALIGN_LEFT,
  ALIGN_CENTER,
  ALIGN_RIGHT
};

enum CanvasVerticalAlignment {
  ALIGN_TOP,
  ALIGN_MIDDLE,
  ALIGN_BOTTOM
};


typedef Canvas* CanvasRef;

class Canvas {
  public:
    TFT_eSPI * tft;
    Canvas(TFT_eSPI * tft);
    Canvas(Canvas * other, uint16_t id);
    virtual ~Canvas();

    uint16_t getDrawableWidth();
    uint16_t getDrawableHeight();
    int16_t getDrawX();
    int16_t getDrawY();
    int16_t getRightX();
    int16_t getBottomY();
    int16_t getDrawRightX();
    int16_t getDrawBottomY();
    
    uint16_t getTextWidth(String text);
    uint16_t getTextWidth(char * str);
    virtual void print();
    
    bool handleTouch(TouchEvent event, TouchEventData eventData);
    void onTouch(CanvasTouchCallback callback);
    void setHMargin(uint16_t margin);
    void setHMargin(uint16_t marginLeft, uint16_t marginRight);
    void setVMargin(uint16_t margin);
    void setVMargin(uint16_t marginTop, uint16_t marginBottom);
    void setMargin(uint16_t margin);
    void setMargin(uint16_t margin_horizontal, uint16_t margin_vertical);
    void setMargin(uint16_t marginLeft, uint16_t marginRight, uint16_t marginTop, uint16_t marginBottom);
    void setMarginLeft(uint16_t marginLeft);
    void setMarginRight(uint16_t marginRight);
    void setMarginTop(uint16_t marginTop);
    void setMarginBottom(uint16_t marginBottom);
    uint16_t getMarginLeft();
    uint16_t getMarginTop();
    uint16_t getMarginRight();
    uint16_t getMarginBottom();
    void setHPadding(uint16_t padding);
    void setHPadding(uint16_t paddingLeft, uint16_t paddingRight);
    void setVPadding(uint16_t padding);
    void setVPadding(uint16_t paddingTop, uint16_t paddingBottom);
    void setPadding(uint16_t padding);
    void setPadding(uint16_t padding_horizontal, uint16_t padding_vertical);
    void setPadding(uint16_t paddingLeft, uint16_t paddingRight, uint16_t paddingTop, uint16_t paddingBottom);
    void setPaddingLeft(uint16_t paddingLeft);
    void setPaddingRight(uint16_t paddingRight);
    void setPaddingTop(uint16_t paddingTop);
    void setPaddingBottom(uint16_t paddingBottom);
    uint16_t getPaddingLeft();
    uint16_t getPaddingTop();
    uint16_t getPaddingRight();
    uint16_t getPaddingBottom();
    void setX(int16_t x);
    void setY(int16_t y);
    void shiftX(int16_t x);
    void shiftY(int16_t y);
    void shift(int16_t x, int16_t y);
    virtual void setWidth(uint16_t width);
    virtual void setHeight(uint16_t height);
    void setRightX(int16_t rightX);
    void setBottomY(int16_t bottomY);
    uint16_t getX();
    uint16_t getY();
    uint16_t getWidth();
    uint16_t getHeight();
    void setVAlign(CanvasVerticalAlignment vAlign);
    void setHAlign(CanvasHorizontalAlignment hAlign);
    CanvasVerticalAlignment getVAlign();
    CanvasHorizontalAlignment getHAlign();
    bool redraw();
    #ifdef CANVAS_DEBUG
    bool drawDebugBorder();
    #endif
    bool redrawChildren();
    void invalidate();
    bool isInvalid();
    void setVisible(bool visible);
    bool getVisible();
    virtual void setDarkMode(bool darkMode);
    bool getDarkMode();
    void alignRight(Canvas * canvas);
    void alignRight();
    void horizontalAlign(Canvas * leftCanvas, Canvas * rightCanvas);
    void setBgColor(uint16_t bgColor);
    uint16_t getBgColor(); 
    virtual void setFgColor(uint16_t fgColor);
    uint16_t getFgColor(); 
    uint16_t getID();
    Canvas* operator [] (uint16_t id) const;
    Canvas* get(uint16_t id);
    uint16_t convert2rgb565 (uint32_t color);
    uint16_t convert2rgb565 (uint8_t red, uint8_t green, uint8_t blue);
    uint16_t getRed(uint16_t color);
    uint16_t getGreen(uint16_t color);
    uint16_t getBlue(uint16_t color);
  protected:
    SemaphoreHandle_t tftMutex;
    list<Canvas *> children;
    uint16_t id;
    CanvasTouchCallback onTouchCallback;
    int16_t x;
    int16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t marginLeft = 0;
    uint16_t marginTop = 0;
    uint16_t marginRight = 0;
    uint16_t marginBottom = 0;
    uint16_t paddingLeft = 0;
    uint16_t paddingRight = 0;
    uint16_t paddingTop = 0;
    uint16_t paddingBottom = 0;
    uint16_t bgColor;
    uint16_t fgColor;
    CanvasHorizontalAlignment hAlign;
    CanvasVerticalAlignment vAlign;
    bool visible = 1;
    bool invalidated = 1;
    bool darkMode = 0;
    bool add(Canvas * childCanvas);
    virtual bool draw();
    uint32_t counter=0;
    Canvas * parent = NULL;
    void print(String tab);
    int8_t getLevel();
    bool existsIn(uint16_t x, uint16_t y);
    bool drawBackground();
};

#endif
