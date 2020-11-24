#ifndef __SCENEENTITYROWCANVAS_H__
#define __SCENEENTITYROWCANVAS_H__

#include "ButtonEntityRowCanvas.hpp"
#include "HomeAssistant/Components/Script.hpp"

class SceneEntityRowCanvas : public ButtonEntityRowCanvas {
    public:
        SceneEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        ~SceneEntityRowCanvas();
};

#endif // __SCENEENTITYROWCANVAS_H__