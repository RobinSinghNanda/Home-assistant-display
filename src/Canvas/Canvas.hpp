#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include "Touch.h"
#include "Arduino.h"
#include <functional>
#include <TFT_eSPI.h> 
#include <JPEGDecoder.h>
#include <iterator>
using namespace std; 
#include <list> 
#include "Icons.hpp"
#include "Color.hpp"
#include "Color16Bit.hpp"

#define CANVAS_MAX_CHILDREN 20

//#define CANVAS_DEBUG 1

#define CANVAS_DEBUG_BORDER_COLOR (this->Color32Bit(0xFFFF00).get16Bit())
#define CANVAS_DEBUG_MARGIN_BORDER_COLOR (this->Color32Bit(0xFF00FF).get16Bit())

#define CANVAS_MAX_LEVELS 10

struct TftConfig;

class Canvas;

typedef std::function<bool(Canvas*, TouchEvent, TouchEventData)> CanvasTouchCallback;

#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
#define invalidateIfNotEqual(a,b) {if(a == b){return;}else{a=b;this->invalidate();}}
#define returnIfEqual(a,b) {if(a == b){return;}else{a=b;}}

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

class Canvas : public iterator<std::input_iterator_tag, Canvas *, Canvas *, const Canvas **, Canvas *> {
  public:
    TFT_eSPI * tft;
    Canvas(TFT_eSPI * tft);
    Canvas(Canvas * other, uint16_t id);
    virtual ~Canvas();

    inline uint16_t getDrawableWidth();
    inline uint16_t getDrawableHeight();
    inline int16_t getDrawX();
    inline int16_t getDrawY();
    inline int16_t getRightX();
    inline int16_t getBottomY();
    inline int16_t getDrawRightX();
    inline int16_t getDrawBottomY();
    
    inline uint16_t getX();
    inline uint16_t getY();
    inline uint16_t getWidth();
    inline uint16_t getHeight();
    
    virtual void print();
    virtual void resetLayout();
    
    bool handleTouch(TouchEvent event, TouchEventData eventData);
    inline void onTouch(CanvasTouchCallback callback);

    //Getter and setter margins
    inline void setHMargin(uint16_t margin);
    inline void setHMargin(uint16_t marginLeft, uint16_t marginRight);
    inline void setVMargin(uint16_t margin);
    inline void setVMargin(uint16_t marginTop, uint16_t marginBottom);
    inline void setMargin(uint16_t margin);
    inline void setMargin(uint16_t margin_horizontal, uint16_t margin_vertical);
    inline void setMargin(uint16_t marginLeft, uint16_t marginRight, uint16_t marginTop, uint16_t marginBottom);
    inline void setMarginLeft(uint16_t marginLeft);
    inline void setMarginRight(uint16_t marginRight);
    inline void setMarginTop(uint16_t marginTop);
    inline void setMarginBottom(uint16_t marginBottom);
    inline uint16_t getMarginLeft();
    inline uint16_t getMarginTop();
    inline uint16_t getMarginRight();
    inline uint16_t getMarginBottom();
    
    //Getter and setter paddings
    inline void setHPadding(uint16_t padding);
    inline void setHPadding(uint16_t paddingLeft, uint16_t paddingRight);
    inline void setVPadding(uint16_t padding);
    inline void setVPadding(uint16_t paddingTop, uint16_t paddingBottom);
    inline void setPadding(uint16_t padding);
    inline void setPadding(uint16_t padding_horizontal, uint16_t padding_vertical);
    inline void setPadding(uint16_t paddingLeft, uint16_t paddingRight, uint16_t paddingTop, uint16_t paddingBottom);
    inline void setPaddingLeft(uint16_t paddingLeft);
    inline void setPaddingRight(uint16_t paddingRight);
    inline void setPaddingTop(uint16_t paddingTop);
    inline void setPaddingBottom(uint16_t paddingBottom);
    inline uint16_t getPaddingLeft();
    inline uint16_t getPaddingTop();
    inline uint16_t getPaddingRight();
    inline uint16_t getPaddingBottom();

    //Getter and setter borders
    inline void setHBorder(uint16_t border);
    inline void setHBorder(uint16_t borderLeft, uint16_t borderRight);
    inline void setVBorder(uint16_t border);
    inline void setVBorder(uint16_t borderTop, uint16_t borderBottom);
    inline void setBorder(uint16_t border);
    inline void setBorder(uint16_t border_horizontal, uint16_t border_vertical);
    inline void setBorder(uint16_t borderLeft, uint16_t borderRight, uint16_t borderTop, uint16_t borderBottom);
    inline void setBorderLeft(uint16_t borderLeft);
    inline void setBorderRight(uint16_t borderRight);
    inline void setBorderTop(uint16_t borderTop);
    inline void setBorderBottom(uint16_t borderBottom);
    inline uint16_t getBorderLeft();
    inline uint16_t getBorderTop();
    inline uint16_t getBorderRight();
    inline uint16_t getBorderBottom();

    //Getter and setter for align
    inline void setVAlign(CanvasVerticalAlignment vAlign);
    inline void setHAlign(CanvasHorizontalAlignment hAlign);
    inline CanvasVerticalAlignment getVAlign();
    inline CanvasHorizontalAlignment getHAlign();

    void setX(int16_t x);
    void setY(int16_t y);
    void shiftX(int16_t x);
    void shiftY(int16_t y);
    void shift(int16_t x, int16_t y);
    virtual void setWidth(uint16_t width);
    virtual void setHeight(uint16_t height);
    void setRightX(int16_t rightX);
    void setBottomY(int16_t bottomY);

    bool redraw();
    void drawBorder();
    #ifdef CANVAS_DEBUG
    bool drawDebugBorder();
    #endif
    inline void redrawChildren();
    void invalidate();
    inline bool isInvalid();
    void setVisible(bool visible);
    bool isVisible();
    void hAlignLeft(Canvas * canvas);
    void hAlignRight(Canvas * canvas);
    void hAlignLeft();
    void hAlignRight();
    void hAlignBetween(Canvas * leftCanvas, Canvas * rightCanvas);
    void vAlignTop(Canvas * canvas);
    void vAlignBottom(Canvas * canvas);
    void vAlignTop();
    void vAlignBottom();
    void vAlignBetween(Canvas * topCanvas, Canvas * bottomCanvas);
    virtual void setBgColor(Color16Bit bgColor);
    Color16Bit getBgColor(); 
    virtual void setFgColor(Color16Bit fgColor);
    Color16Bit getFgColor(); 
    inline void setBorderColor(Color16Bit borderColor);
    inline Color16Bit getBorderColor();
    uint16_t getID();
    Canvas* operator [] (uint16_t id) const;
    Canvas* get(uint16_t id);
    inline uint16_t getNumChildren();
    void setDrawBackgroundEnable(bool enable);
    bool isDrawBackgroundEnabled();
    void setChanged();
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
    uint16_t borderLeft = 0;
    uint16_t borderRight = 0;
    uint16_t borderTop = 0;
    uint16_t borderBottom = 0;
    Color16Bit bgColor = TFT_BLACK;
    Color16Bit fgColor = TFT_WHITE;
    Color16Bit borderColor = TFT_WHITE;
    CanvasHorizontalAlignment hAlign;
    CanvasVerticalAlignment vAlign;
    bool visible = 1;
    bool invalidated = 1;
    bool changed = 0;
    bool drawBackgroundEnable = false;
    bool add(Canvas * childCanvas);
    virtual bool draw();
    uint32_t counter=0;
    Canvas * parent = NULL;
    void print(String tab);
    int8_t getLevel();
    bool existsIn(uint16_t x, uint16_t y);
    inline void drawBackground();
    inline void drawFullBackground();
    
};

uint16_t Canvas::getNumChildren() {
  return this->children.size();
}

uint16_t Canvas::getDrawableWidth() {
  if (this->width > this->marginLeft + this->marginRight + this->paddingLeft + this->paddingRight + this->borderLeft + this->borderRight) {
    return this->width - this->marginLeft - this->marginRight - this->paddingLeft - this->paddingRight - this->borderLeft - this->borderRight;
  } else {
    return 0;
  }
}

uint16_t Canvas::getDrawableHeight() {
  if (this->height > this->marginTop + this->marginBottom + this->paddingTop + this->paddingBottom + this->borderTop + this->borderBottom) {
    return this->height - this->marginTop - this->marginBottom - this->paddingTop - this->paddingBottom - this->borderTop - this->borderBottom;
  } else {
    return 0;
  }
}

int16_t Canvas::getDrawX() {
  return this->x + this->marginLeft + this->paddingLeft + this->borderLeft;
}

int16_t Canvas::getDrawY() {
  return this->y + this->marginTop + this->paddingTop + this->borderTop;
}

int16_t Canvas::getDrawRightX() {
  return this->getDrawX() + this->getDrawableWidth();
}

int16_t Canvas::getDrawBottomY() {
  return this->getDrawY() + this->getDrawableHeight();
}

int16_t Canvas::getRightX() {
  return this->x + this->width;
}

int16_t Canvas::getBottomY() {
  return this->y + this->height;
}

uint16_t Canvas::getX() {
  return this->x;
}

uint16_t Canvas::getY() {
  return this->y;
}

uint16_t Canvas::getWidth() {
  return this->width;
}

uint16_t Canvas::getHeight() {
  return this->height;
}

void Canvas::setHMargin(uint16_t margin) {
  this->setHMargin(margin, margin);
}

void Canvas::setHMargin(uint16_t marginLeft, uint16_t marginRight) {
  this->marginLeft = marginLeft;
  this->marginRight = marginRight;
}

void Canvas::setVMargin(uint16_t margin) {
  this->setVMargin(margin, margin);
}

void Canvas::setVMargin(uint16_t margin_top, uint16_t margin_bottom) {
  this->marginTop = margin_top;
  this->marginBottom = margin_bottom;
}

void Canvas::setMargin(uint16_t margin) {
  this->setMargin(margin, margin, margin, margin);
}

void Canvas::setMargin(uint16_t margin_horizontal, uint16_t margin_vertical) {
  this->setHMargin(margin_horizontal);
  this->setVMargin(margin_vertical);
}

void Canvas::setMargin(uint16_t margin_left, uint16_t margin_right, uint16_t margin_top, uint16_t margin_bottom) {
  this->marginTop = margin_top;
  this->marginLeft = margin_left;
  this->marginRight = margin_right;
  this->marginBottom = margin_bottom;
}

void Canvas::setMarginLeft(uint16_t marginLeft) {
  this->marginLeft = marginLeft;
}

void Canvas::setMarginRight(uint16_t marginRight) {
  this->marginRight = marginRight;
}

void Canvas::setMarginTop(uint16_t marginTop) {
  this->marginTop = marginTop;
}

void Canvas::setMarginBottom(uint16_t marginBottom) {
  this->marginBottom = marginBottom;
}

uint16_t Canvas::getMarginLeft() {
  return this->marginLeft;
}

uint16_t Canvas::getMarginTop()  {
  return this->marginTop;
}

uint16_t Canvas::getMarginRight() {
  return this->marginRight;
}

uint16_t Canvas::getMarginBottom() {
  return this->marginBottom;
}

void Canvas::setHPadding(uint16_t padding) {
  this->setPadding(padding, padding);
}

void Canvas::setHPadding(uint16_t padding_left, uint16_t padding_right) {
  this->paddingLeft = padding_left;
  this->paddingRight = padding_right;
}

void Canvas::setVPadding(uint16_t padding) {
  this->setVPadding(padding, padding);
}

void Canvas::setVPadding(uint16_t padding_top, uint16_t padding_bottom) {
  this->paddingTop = padding_top;
  this->paddingBottom = padding_bottom;
}

void Canvas::setPadding(uint16_t padding) {
  this->setPadding(padding, padding, padding, padding);
}

void Canvas::setPadding(uint16_t padding_horizontal, uint16_t padding_vertical) {
  this->setHPadding(padding_horizontal);
  this->setVPadding(padding_vertical);
}

void Canvas::setPadding(uint16_t padding_left, uint16_t padding_right, uint16_t padding_top, uint16_t padding_bottom) {
  this->paddingTop = padding_top;
  this->paddingLeft = padding_left;
  this->paddingRight = padding_right;
  this->paddingBottom = padding_bottom;
}

void Canvas::setPaddingLeft(uint16_t paddingLeft) {
  this->paddingLeft = paddingLeft;
}

void Canvas::setPaddingRight(uint16_t paddingRight) {
  this->paddingRight = paddingRight;
}

void Canvas::setPaddingTop(uint16_t paddingTop) {
  this->paddingTop = paddingTop;
}

void Canvas::setPaddingBottom(uint16_t paddingBottom) {
  this->paddingBottom = paddingBottom;
}

uint16_t Canvas::getPaddingLeft() {
  return this->paddingLeft;
}

uint16_t Canvas::getPaddingTop()  {
  return this->paddingTop;
}

uint16_t Canvas::getPaddingRight() {
  return this->paddingRight;
}

uint16_t Canvas::getPaddingBottom() {
  return this->paddingBottom;
}

void Canvas::setHBorder(uint16_t border) {
  this->setBorder(border, border);
}

void Canvas::setHBorder(uint16_t border_left, uint16_t border_right) {
  this->borderLeft = border_left;
  this->borderRight = border_right;
}

void Canvas::setVBorder(uint16_t border) {
  this->setVBorder(border, border);
}

void Canvas::setVBorder(uint16_t border_top, uint16_t border_bottom) {
  this->borderTop = border_top;
  this->borderBottom = border_bottom;
}

void Canvas::setBorder(uint16_t border) {
  this->setBorder(border, border, border, border);
}

void Canvas::setBorder(uint16_t border_horizontal, uint16_t border_vertical) {
  this->setHBorder(border_horizontal);
  this->setVBorder(border_vertical);
}

void Canvas::setBorder(uint16_t border_left, uint16_t border_right, uint16_t border_top, uint16_t border_bottom) {
  this->borderTop = border_top;
  this->borderLeft = border_left;
  this->borderRight = border_right;
  this->borderBottom = border_bottom;
}

void Canvas::setBorderLeft(uint16_t borderLeft) {
  this->borderLeft = borderLeft;
}

void Canvas::setBorderRight(uint16_t borderRight) {
  this->borderRight = borderRight;
}

void Canvas::setBorderTop(uint16_t borderTop) {
  this->borderTop = borderTop;
}

void Canvas::setBorderBottom(uint16_t borderBottom) {
  this->borderBottom = borderBottom;
}

uint16_t Canvas::getBorderLeft() {
  return this->borderLeft;
}

uint16_t Canvas::getBorderTop()  {
  return this->borderTop;
}

uint16_t Canvas::getBorderRight() {
  return this->borderRight;
}

uint16_t Canvas::getBorderBottom() {
  return this->borderBottom;
}

void Canvas::setVAlign(CanvasVerticalAlignment vAlign) {
  this->vAlign = vAlign;
}

void Canvas::setHAlign(CanvasHorizontalAlignment hAlign) {
  this->hAlign = hAlign;
}

CanvasVerticalAlignment Canvas::getVAlign() {
  return this->vAlign;
}

CanvasHorizontalAlignment Canvas::getHAlign() {
  return this->hAlign;
}

void Canvas::setBorderColor(Color16Bit borderColor) {
  invalidateIfNotEqual(this->borderColor, borderColor);
}

Color16Bit Canvas::getBorderColor() {
  return this->borderColor;
}

void Canvas::drawBackground() {
  tft->fillRect(getDrawX(), getDrawY(), getDrawableWidth(), getDrawableHeight(), this->bgColor);
}

void Canvas::drawFullBackground() {
  tft->fillRect(this->x, this->y, this->width, this->height, this->bgColor);
}

void Canvas::onTouch(CanvasTouchCallback callback) {
  this->onTouchCallback = callback;
}

void Canvas::redrawChildren() {
  for (Canvas * childCanvas: this->children) {
    childCanvas->redraw();
  }
}

bool Canvas::isInvalid() {
  return this->invalidated;
}

#endif
