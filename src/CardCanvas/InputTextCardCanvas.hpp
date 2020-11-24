#ifndef __INPUTTEXTCARDCANVAS_H__
#define __INPUTTEXTCARDCANVAS_H__

#include "CardCanvas.hpp"
#include "Canvas/KeyboardCanvas.hpp"
#include "HomeAssistant/Components/InputText.hpp"
#include "LovelaceConfig/InputTextCardConfig.hpp"
#include "RowCanvas/InputTextRowCanvas.hpp"

class InputTextCardCanvas : public CardCanvas {
    public:
        InputTextCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, InputTextCardConfig * cardConfig, LcdTheme * theme);
        ~InputTextCardCanvas();
        static constexpr int swipeThreshold = 10;
        virtual void update();
    protected:
        string savedText = "";
        InputTextCardConfig * cardConfig;
        InputTextRowCanvas * entityRowCanvas;
        TextCanvas * textCanvas;
        InputText * entity;
        KeyboardCanvas * keyboardCanvas;
};

#endif // __INPUTTEXTCARDCANVAS_H__