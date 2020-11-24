#ifndef __MEDIAPLAYERENTITYROWCANVAS_H__
#define __MEDIAPLAYERENTITYROWCANVAS_H__

#include "BaseEntityRowCanvas.hpp"
#include "Canvas/ImageCanvas.hpp"
#include "HomeAssistant/Components/MediaPlayer.hpp"

class MediaPlayerEntityRowCanvas : public BaseEntityRowCanvas {
    public:
        MediaPlayerEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        virtual void update();
    protected:
        IconCanvas * stateCanvas;
        MediaPlayer * entity;
};

#endif // __MEDIAPLAYERENTITYROWCANVAS_H__