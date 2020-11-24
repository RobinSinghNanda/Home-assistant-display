#ifndef __ENTITESCARDCANVAS_H__
#define __ENTITESCARDCANVAS_H__

#include "CardCanvas.hpp"
#include "LovelaceConfig/EntitiesCardConfig.hpp"

class EntitesCardCanvas;

typedef std::function<bool(BaseCardConfig *)> EntitesCardCanvasCallback;

class EntitesCardCanvas : public CardCanvas {
    public:
        EntitesCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, EntitiesCardConfig * cardConfig, LcdTheme * theme);
        virtual ~EntitesCardCanvas();
        static constexpr uint16_t ROW_HEIGHT = 40;
        void setCallback(EntitesCardCanvasCallback callback);
        virtual void update();
    protected:
        EntitiesCardConfig * cardConfig;
        EntitesCardCanvasCallback entityCallback;
};

#endif // __ENTITESCARDCANVAS_H__