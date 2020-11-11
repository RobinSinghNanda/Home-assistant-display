#ifndef SWITCH_ENTITY_CANVAS_H_
#define SWITCH_ENTITY_CANVAS_H_

#include "Canvas.hpp"
#include "TextCanvas.hpp"
#include "ImageCanvas.hpp"
#include "SwitchCanvas.hpp"

#define SWICTH_ENTITY_ICON_WIDTH 24
#define SWICTH_ENTITY_ICON_HEIGHT 24
#define SWICTH_ENTITY_MARGIN_LEFT 10
#define SWICTH_ENTITY_MARGIN_RIGHT 10

#define ICON_PRIMARY_COLOR convert2rgb565(0x44739e)

class BaseEntityRowCanvas : public Canvas {
  public:
    BaseEntityRowCanvas(Canvas * canvas, uint16_t id);
    virtual void setIconPath(String setIconPath);
    String getIconPath();
    void setName(String name);
    String getName();
    void onIconTouch(CanvasTouchCallback callback);
    void onNameTouch(CanvasTouchCallback callback);
    virtual void setFgColor(uint16_t fgColor);
  protected:
    ImageCanvas * iconCanvas;
    TextCanvas * nameCanvas;
    
};

#endif