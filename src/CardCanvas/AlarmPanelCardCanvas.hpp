#ifndef __ALARMPANELCARDCANVAS_H__
#define __ALARMPANELCARDCANVAS_H__

#include "CardCanvas.hpp"
#include <Canvas/ImageCanvas.hpp>
#include <Canvas/TextCanvas.hpp>
#include <Canvas/NumberPadCanvas.hpp>
#include "LovelaceConfig/AlarmPanelCardConfig.hpp"
#include "HomeAssistant/Components/AlarmControlPanel.hpp"
#include <list>


class AlarmPanelCardCanvas : public CardCanvas {
    public:
        AlarmPanelCardCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, AlarmPanelCardConfig * cardConfig, LcdTheme * theme);
        virtual ~AlarmPanelCardCanvas();
        static constexpr int MAX_STATE_BUTTONS = 3;
        static constexpr int MAX_CODE_SIZE = 10;
        virtual const char * getCardTitle();
        virtual const char * getCardIcon();
        virtual void update();
    protected:
        uint32_t prevTime = 0;
        bool iconState = false;
        uint8_t code[MAX_CODE_SIZE];
        int8_t codeIndex = 0;
        NumberPadCanvas * numPadCanvas;
        TextCanvas * codeCanvas;
        TextCanvas * stateChangeButtonCanvas[MAX_STATE_BUTTONS];
        ImageCanvas * stateCanvas;
        ImageCanvas * passwordVisibilityToggleButton;
        bool passwordVisible = false;
        uint8_t numStateButtons = 0;
        AlarmPanelCardConfig * cardConfig;
        AlarmControlPanel * entity;
        std::list<string> states;
        void setCodeString();
};

#endif // __ALARMPANELCARDCANVAS_H__