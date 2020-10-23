#include "Canvas.hpp"

Canvas::~Canvas () {
  for (Canvas * childCanvas: this->children) {
    delete childCanvas;
  }
}

Canvas::Canvas (TFT_eSPI * tft) {
  this->tft = tft;
  this->x = 0;
  this->y = 0;
  this->width = 0;
  this->height = 0;
  this->marginLeft = 0;
  this->marginTop = 0;
  this->marginRight = 0;
  this->marginBottom = 0;
  this->hAlign = ALIGN_LEFT;
  this->vAlign = ALIGN_TOP;
  this->darkMode = 0;
  this->onTouchCallback = [](Canvas*, TouchEvent, TouchEventData)-> bool{return false;};
  this->parent = NULL;
  this->id = 0;
  this->bgColor = TFT_BLACK;
  this->fgColor = TFT_WHITE;
  tftMutex = xSemaphoreCreateMutex();
}

Canvas::Canvas (Canvas * other, uint16_t id) {
  this->tft = other->tft;
  this->x = other->x;
  this->y = other->y;
  this->width = other->width;
  this->height = other->height;
  this->marginLeft = 0;
  this->marginTop = 0;
  this->marginRight = 0;
  this->marginBottom = 0;
  this->hAlign = other->hAlign;
  this->vAlign = other->vAlign;
  this->darkMode = other->darkMode;
  this->onTouchCallback = [](Canvas*, TouchEvent, TouchEventData)->bool{return false;};
  this->parent = other;
  this->id = id;
  this->bgColor = other->bgColor;
  this->fgColor = other->fgColor;
  this->tftMutex = other->tftMutex;
  other->add(this);
}

void Canvas::shiftX(int16_t x) {
  if (x != 0) {
    this->x += x;
    for (CanvasRef childCanvas:this->children) {
      childCanvas->x += x;
    }
    this->invalidate();
  }
}

void Canvas::shiftY(int16_t y) {
  if (y != 0) {
    this->y += y;
    for (CanvasRef childCanvas:this->children) {
      childCanvas->y += y;
    }
    this->invalidate();
  }
}

void Canvas::shift (int16_t x, int16_t y) {
  shiftX(x);
  shiftY(y);
}

void Canvas::setX(int16_t x) {
  if (this->x != x) {
    this->x = x;
    if (parent != NULL) {
      if (this->x < parent->getDrawX()) {
        this->x = parent->getDrawX();
      } else if (this->x > parent->getDrawRightX()) {
        this->x = parent->getDrawRightX();
        this->width = 0;
        Serial.println("X can't be more than parent x, reducing width to 0.");
      } else if (this->getRightX() > parent->getDrawRightX()) {
        this->width = parent->getDrawRightX() - this->getX();
      }
    }
    for (Canvas * childCanvas:this->children) {
      if (childCanvas->getX() < x) {
        childCanvas->setX(x);
      } else {
        childCanvas->setX(childCanvas->getX() + x - this->x);
      }
    }
    this->invalidate();
  }
}

void Canvas::setY(int16_t y) {
  if (this->y != y) {
    this->y = y;
    if (parent != NULL) {
      if (this->y < parent->getDrawY()) {
        this->y = parent->getDrawY();
      } else if (this->y > parent->getDrawBottomY()) {
        this->y = parent->getDrawBottomY();
        this->setHeight(0);
        Serial.println("Y can't be more than parent y, reducing width to 0.");
      } else if (this->getBottomY() > parent->getDrawBottomY()) {
        this->setHeight(parent->getDrawBottomY() - this->getY());
      }
    }
    for (Canvas * childCanvas:this->children) {
      if (childCanvas->getY() < y) {
        childCanvas->setY(y);
      } else {
        childCanvas->setY(childCanvas->getY() + y - this->y);
      }
    }
    this->invalidate();
  }
}

void Canvas::setWidth(uint16_t width) {
  if (this->width != width) {
    this->width = width;
    for (Canvas * childCanvas: this->children) {
      if (childCanvas->getRightX() > this->getDrawBottomY()) {
        childCanvas->setRightX(this->getDrawBottomY());
      }
    }
    this->invalidate();
  }
}

void Canvas::setHeight(uint16_t height) {
  if (this->height != height) {
    this->height = height;
    for (Canvas * childCanvas: this->children) {
      if (childCanvas->getBottomY() > this->getDrawBottomY()) {
        childCanvas->setBottomY(this->getDrawBottomY());
      }
    }
    this->invalidate();
  }
}

void Canvas::setRightX(int16_t rightX) {
  if (rightX > this->x) {
    this->setWidth(rightX - this->getX());
  } else {
    this->setWidth(0);
  }
}

void Canvas::setBottomY(int16_t bottomY) {
  if (bottomY > this->y) {
    this->setHeight(bottomY - this->getY());
  } else {
    this->setHeight(0);
  }
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

uint16_t Canvas::getDrawableWidth() {
  if (this->width > this->marginLeft + this->marginRight) {
    return this->width - this->marginLeft - this->marginRight;
  } else {
    return 0;
  }
}

uint16_t Canvas::getDrawableHeight() {
  if (this->height > this->marginTop + this->marginBottom) {
    return this->height - this->marginTop - this->marginBottom;
  } else {
    return 0;
  }
}

int16_t Canvas::getDrawX() {
  return this->x + this->marginLeft;
}

int16_t Canvas::getDrawY() {
  return this->y + this->marginTop;
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

void Canvas::setBgColor(uint16_t bgColor) { 
  invalidateIfNotEqual(this->bgColor, bgColor);
  this->bgColor = bgColor;
  for (CanvasRef childCanvas: this->children) {
    childCanvas->setBgColor(bgColor);
  }
  this->invalidate();
}

uint16_t Canvas::getBgColor() {
  return this->bgColor;
}

void Canvas::setFgColor(uint16_t fgColor) { 
  invalidateIfNotEqual(this->bgColor, bgColor);
  this->fgColor = fgColor;
  for (CanvasRef childCanvas: this->children) {
    childCanvas->setFgColor(fgColor);
  }
  this->invalidate();
}

uint16_t Canvas::getFgColor() {
  return this->fgColor;
}

bool Canvas::add (Canvas * childCanvas) {
  if (this->children.size() != CANVAS_MAX_CHILDREN) {
    this->children.push_back(childCanvas);
    return 1;
  } else {
    return 0;
  }
}

uint16_t Canvas::getTextWidth(String text) {
  return tft->textWidth(text);
}

uint16_t Canvas::getTextWidth(char * str) {
  return tft->textWidth(str);
}

void Canvas::print() {
  this->print("");
}

void Canvas::print(String tab) {
  if (tab == "") {
    Serial.println("root:");
  } else {
    Serial.print(tab);
    Serial.print(id);
    Serial.println(":");
  }
  Serial.print(tab+"  x: ");
  Serial.println(this->x);
  Serial.print(tab+"  y: ");
  Serial.println(this->y);
  Serial.print(tab+"  width: ");
  Serial.println(this->width);
  Serial.print(tab+"  height: ");
  Serial.println(this->height);
  Serial.print(tab+"  margin_left: ");
  Serial.println(this->marginLeft);
  Serial.print(tab+"  margin_right: ");
  Serial.println(this->marginRight);
  Serial.print(tab+"  margin_top: ");
  Serial.println(this->marginTop);
  Serial.print(tab+"  margin_bottom: ");
  Serial.println(this->marginBottom);
  if (this->children.size() != 0) {
    Serial.println(tab+"  children:");
  } else {
    Serial.println(tab+"  children: ~");
  }
  for (Canvas * childCanvas : this->children) {
    childCanvas->print(tab+"    ");
  }
}

bool Canvas::redraw () {
  bool result = false;
  if (this->visible &&
      this->invalidated &&
      this->getRightX() >= 0 &&
      this->getX() <= tft->width() ) {
      if(xSemaphoreTake(tftMutex, portMAX_DELAY) == pdTRUE) {
        result = draw();
        xSemaphoreGive(tftMutex);
      } else {
        Serial.println("Could not obtain the tft Mutex");
      }
  }
  if (!this->visible && this->invalidated) {
    tft->fillRect(this->x, this->y, this->width, this->height, this->bgColor);
  }
  this->invalidated = 0;
  if (this->visible) {
    redrawChildren();
  }
  return result;
}

bool Canvas::draw() {
  drawBackground();
  #ifdef CANVAS_DEBUG
  this->drawDebugBorder();
  #endif
  return true;
}

bool Canvas::drawBackground() {
  tft->fillRect(this->x, this->y, this->width, this->height, this->bgColor);
  return true;
}

bool Canvas::redrawChildren() {
  bool result = true;
  for (Canvas * childCanvas: this->children) {
    result = result && ~(childCanvas->redraw());
  }
  return result;
}

uint16_t Canvas::convert2rgb565 (uint32_t color) {
  uint8_t blue = color;
  uint8_t green = (color>> 8);
  uint8_t red = (color>> 16);
  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

uint16_t Canvas::convert2rgb565 (uint8_t red, uint8_t green, uint8_t blue) {
  return (((red & 0b11111000)<<8) + ((green & 0b11111100)<<3)+(blue>>3));
}

#ifdef CANVAS_DEBUG
bool Canvas::drawDebugBorder() {
  tft->drawRect(this->x, this->y, this->width, this->height, CANVAS_DEBUG_BORDER_COLOR);
  tft->drawRect(this->getDrawX(), this->getDrawY(), this->getDrawableWidth(), this->getDrawableHeight(), CANVAS_DEBUG_MARGIN_BORDER_COLOR);
  return true;
}
#endif

bool Canvas::handleTouch(TouchEvent event, TouchEventData eventData) {
  //Serial.println("Handling touch for Canvas "+String(getLevel()));
  //Serial.println("Touch at x = "+String(eventData.x)+" y = "+String(eventData.y));
  //tft->drawPixel(eventData.x, eventData.y, CANVAS_DEBUG_MARGIN_BORDER_COLOR);
  //Serial.println("Event type is "+String(event));
  #ifdef CANVAS_DEBUG
    this->drawDebugBorder();
  #endif
  bool touchHandled = false;
  for (Canvas * childCanvas : this->children) {
    uint16_t x = eventData.startX;
    uint16_t y = eventData.startY;
    if (childCanvas->existsIn(x, y) && !touchHandled && childCanvas->visible) {
      touchHandled = touchHandled || childCanvas->handleTouch(event, eventData);
    }
  }
  if (!touchHandled) {
    touchHandled = this->onTouchCallback(this, event, eventData);
  }
  return touchHandled;
}

void Canvas::onTouch(CanvasTouchCallback callback) {
  this->onTouchCallback = callback;
}

bool Canvas::existsIn(uint16_t x, uint16_t y) {
  return ((x > this->x) &&
    (y > this->y) &&
    (x < (this->x + this->width)) &&
    (y < (this->y + this->height)));
}

void Canvas::invalidate() {
  this->invalidated = 1;
  for (CanvasRef childCanvas: this->children) {
    childCanvas->invalidate();
  }
}

bool Canvas::isInvalid() {
  return this->invalidated;
}

void Canvas::setVisible(bool visible) {
  invalidateIfNotEqual(this->visible, visible);
  this->visible = visible;
  for (Canvas * childCanvas: this->children) {
    childCanvas->setVisible(visible);
  }
}

bool Canvas::getVisible() {
  return this->visible;
}

void Canvas::setDarkMode(bool darkMode) {
  invalidateIfNotEqual(this->darkMode, darkMode);
  this->darkMode = darkMode;
  for (Canvas * childCanvas: this->children) {
      childCanvas->setDarkMode(darkMode);
  }
  this->invalidate();
}

bool Canvas::getDarkMode() {
  return this->darkMode;
}

void Canvas::alignRight(Canvas * canvas) {
  uint16_t min_x = 0;
  if (parent != NULL) {
    min_x = parent->getDrawX();
  }
  if (canvas->getX() < (this->getWidth() - min_x)) {
    this->setWidth(canvas->getX() - min_x);
    this->setX(min_x);
  } else {
    this->setX(canvas->getX() - this->getWidth());
  }
}

void Canvas::alignRight() {
  if (parent != NULL) {
    this->setX(this->parent->getDrawRightX() - this->getWidth());
  } else {
    this->setX(tft->width()-this->getWidth());
  }
}

void Canvas::horizontalAlign(Canvas * leftCanvas, Canvas * rightCanvas) {
  if (leftCanvas->getRightX() < rightCanvas->getX()) {
    this->setX(leftCanvas->getRightX());
    this->setRightX(rightCanvas->getX());
  }
}

int8_t Canvas::getLevel() {
  uint32_t level = 0;
  Canvas * tmpCanvas = this;
  while (true) {
    if (level != CANVAS_MAX_LEVELS) {
      if (tmpCanvas->parent != NULL) {
        level++;
        tmpCanvas = tmpCanvas->parent;
      } else {
        return level;
      }
    } else {
      return -1;
    }
  }
}

uint16_t Canvas::getID() {
  return id;
}

Canvas* Canvas::operator [] (uint16_t id) const {
  for (Canvas * childCanvas : this->children) {
    if (childCanvas->id == id) {
      return childCanvas;
    }
  }
  return NULL;
}

Canvas* Canvas::get(uint16_t id) {
  for (Canvas * childCanvas : this->children) {
    if (childCanvas->id == id) {
      return childCanvas;
    }
  }
  return NULL;
}