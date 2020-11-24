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

void Canvas::setBgColor(Color16Bit bgColor) { 
  invalidateIfNotEqual(this->bgColor, bgColor);
  for (CanvasRef childCanvas: this->children) {
    childCanvas->setBgColor(bgColor);
  }
  this->invalidate();
}

Color16Bit Canvas::getBgColor() {
  return this->bgColor;
}

void Canvas::setFgColor(Color16Bit fgColor) { 
  invalidateIfNotEqual(this->fgColor, fgColor);
  for (CanvasRef childCanvas: this->children) {
    childCanvas->setFgColor(fgColor);
  }
  this->invalidate();
}

Color16Bit Canvas::getFgColor() {
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

void Canvas::resetLayout() {

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
      }
  } else if (!this->visible && this->invalidated) {
    drawFullBackground();
  }
  this->invalidated = 0;
  if (this->visible && this->changed) {
    redrawChildren();
  }
  return result;
}

bool Canvas::draw() {
  if (drawBackgroundEnable) {
    drawBackground();
  }
  drawBorder();
  #ifdef CANVAS_DEBUG
  this->drawDebugBorder();
  #endif
  return true;
}

void Canvas::drawBorder() {
  if (borderLeft) {
    tft->fillRect(this->x+this->paddingLeft,
          this->y+this->paddingTop,
          borderLeft,
          this->getHeight() - this->paddingTop - this->paddingBottom,
          borderColor);
  }
  if (borderRight) {
    tft->fillRect(this->getRightX()-this->paddingRight - 1,
          this->y+this->paddingTop,
          borderRight,
          this->getHeight() - this->paddingTop - this->paddingBottom,
          borderColor);
  }
  if (borderTop) {
    tft->fillRect(this->x+this->paddingLeft,
          this->y+this->paddingTop,
          this->getWidth() - this->paddingLeft - this->paddingRight,
          borderTop,
          borderColor);
  }
  if (borderBottom) {
    tft->fillRect(this->x+this->paddingLeft,
          this->getBottomY() - this->paddingBottom - 1,
          this->getWidth() - this->paddingLeft - this->paddingRight,
          borderBottom,
          borderColor);
  }
}

#ifdef CANVAS_DEBUG
bool Canvas::drawDebugBorder() {
  tft->drawRect(this->x, this->y, this->width, this->height, CANVAS_DEBUG_BORDER_COLOR);
  tft->drawRect(this->getDrawX(), this->getDrawY(), this->getDrawableWidth(), this->getDrawableHeight(), CANVAS_DEBUG_MARGIN_BORDER_COLOR);
  return true;
}
#endif

bool Canvas::handleTouch(TouchEvent event, TouchEventData eventData) {
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
  this->setChanged();
}

void Canvas::setChanged() {
  this->changed = true;
  if (this->parent != nullptr)
    this->parent->setChanged();
}

void Canvas::setVisible(bool visible) {
  invalidateIfNotEqual(this->visible, visible);
  for (Canvas * childCanvas: this->children) {
    childCanvas->setVisible(visible);
  }
}

bool Canvas::isVisible() {
  return this->visible;
}

void Canvas::hAlignLeft(Canvas * canvas) {
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

void Canvas::hAlignRight(Canvas * canvas) {
  this->setX(canvas->getRightX());
  if (parent != NULL) {
    if (parent->getRightX() < this->getRightX()) {
      this->width = this->width + parent->getRightX() - this->getRightX();
    }
  }
}

void Canvas::hAlignRight() {
  if (parent != NULL) {
    this->setX(this->parent->getDrawRightX() - this->getWidth());
  } else {
    this->setX(tft->width()-this->getWidth());
  }
}

void Canvas::hAlignLeft() {
  if (parent != NULL) {
    this->setX(this->parent->getX());
  } else {
    this->setX(0);
  }
}

void Canvas::hAlignBetween(Canvas * leftCanvas, Canvas * rightCanvas) {
  if (leftCanvas->getRightX() < rightCanvas->getX()) {
    this->setX(leftCanvas->getRightX());
    this->setRightX(rightCanvas->getX());
  }
}

void Canvas::vAlignTop(Canvas * canvas) {
  uint16_t min_y = 0;
  if (parent != NULL) {
    min_y = parent->getDrawY();
  }
  if (canvas->getY() < (this->getHeight() - min_y)) {
    this->setHeight(canvas->getY() - min_y);
    this->setY(min_y);
  } else {
    this->setY(canvas->getY() - this->getHeight());
  }
}

void Canvas::vAlignBottom(Canvas * canvas) {
  this->setY(canvas->getBottomY());
  if (parent != NULL) {
    if (parent->getBottomY() < this->getBottomY()) {
      this->height = this->height + parent->getBottomY() - this->getBottomY();
    }
  }
}

void Canvas::vAlignBottom() {
  if (parent != NULL) {
    this->setY(this->parent->getDrawBottomY() - this->getHeight());
  } else {
    this->setY(tft->height()-this->getHeight());
  }
}

void Canvas::vAlignTop() {
  if (parent != NULL) {
    this->setY(this->parent->getY());
  } else {
    this->setY(0);
  }
}

void Canvas::vAlignBetween(Canvas * topCanvas, Canvas * bottomCanvas) {
  if (topCanvas->getBottomY() < bottomCanvas->getY()) {
    this->setY(topCanvas->getBottomY());
    this->setBottomY(bottomCanvas->getY());
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
  return nullptr;
}

void Canvas::setDrawBackgroundEnable(bool enable) {
  this->drawBackgroundEnable = enable;
}

bool Canvas::isDrawBackgroundEnabled() {
  return this->drawBackgroundEnable;
}