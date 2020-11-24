#ifndef SWITCH_ENTITY_CANVAS_H_
#define SWITCH_ENTITY_CANVAS_H_

#include "BaseRowCanvas.hpp"
#include "LovelaceConfig/BaseCardConfig.hpp"
#include "LovelaceConfig/DefaultRowConfig.hpp"

#include "Canvas/TextCanvas.hpp"
#include "Canvas/ImageCanvas.hpp"
#include "Canvas/SwitchCanvas.hpp"
#include "HomeAssistant/Components/Entity.hpp"
using namespace HomeAssistant;

#define SWICTH_ENTITY_ICON_WIDTH 24
#define SWICTH_ENTITY_ICON_HEIGHT 24
#define SWICTH_ENTITY_MARGIN_LEFT 10
#define SWICTH_ENTITY_MARGIN_RIGHT 10

#define ICON_PRIMARY_COLOR Color32Bit(0x44739e).get16Bit()

typedef std::function<bool(BaseCardConfig *)> EntitesRowCanvasCallback;

class BaseEntityRowCanvas : public BaseRowCanvas {
  public:
    BaseEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
    virtual void update();
    void setCallback(EntitesRowCanvasCallback callback);
  protected:
    DefaultRowConfig * rowConfig;
    ImageCanvas * iconCanvas;
    TextCanvas * nameCanvas;
    Entity * entity = nullptr;
    EntitesRowCanvasCallback entityCallback;
    bool defaultStateColor = 0;
};

#endif