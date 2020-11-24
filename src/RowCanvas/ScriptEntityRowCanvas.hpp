#ifndef __SCRIPTENTITYROWCANVAS_H__
#define __SCRIPTENTITYROWCANVAS_H__

#include "ButtonEntityRowCanvas.hpp"
#include "HomeAssistant/Components/Script.hpp"

class ScriptEntityRowCanvas : public ButtonEntityRowCanvas {
    public:
        ScriptEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme);
        ~ScriptEntityRowCanvas();
};

#endif // __SCRIPTENTITYROWCANVAS_H__