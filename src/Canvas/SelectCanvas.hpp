#ifndef __SELECTCANVAS_H__
#define __SELECTCANVAS_H__

#include "Canvas.hpp"

#include "TextCanvas.hpp"
#include "ImageCanvas.hpp"
#include "Fonts.hpp"

enum SelectCanvasStateChangeEvent {
    CHANGE_NEXT,
    CHANGE_PREV
};

class SelectCanvas;

typedef std::function<bool(SelectCanvas*, SelectCanvasStateChangeEvent, const char * &value)> SelectCanvasStateChangeCallback;

class SelectCanvas : public Canvas {
    public:
        SelectCanvas(Canvas * canvas, uint16_t id);
        ~SelectCanvas();
        virtual void resetLayout();
        inline void onStateChange(SelectCanvasStateChangeCallback callback);
        virtual void setValue(const char * value);
        void resize();
        inline void setLeftDisable(bool disabled);
        inline void setRightDisable(bool disabled);
        inline bool isLeftDisabled();
        inline bool isRightDisabled();
        inline void setDisabledColor(Color16Bit color);
        inline Color16Bit getDisabledColor();
    protected:
        uint16_t swipeThreshold = 20;
        Color16Bit disabledColor = Color32Bit(0x7F7F7F);
        bool leftDisable = false;
        bool rightDisable = false;
        Canvas * valueCanvas = nullptr;
        IconCanvas * valueIconCanvas;
        IconCanvas * leftButtonCanvas;
        IconCanvas * rightButtonCanvas;
        SelectCanvasStateChangeCallback stateChangeCallback;
        bool selectNext();
        bool selectPrev();
        void setValueCanvas(Canvas * valueCanvas);
        virtual bool draw();
};

void SelectCanvas::onStateChange(SelectCanvasStateChangeCallback callback) {
    stateChangeCallback = callback;
}

void SelectCanvas::setLeftDisable(bool disable) {
    returnIfEqual(this->leftDisable, disable);
    this->leftDisable = disable;
    if (disable) {
        this->leftButtonCanvas->setFgColor(disabledColor);
    } else {
        this->leftButtonCanvas->setFgColor(this->fgColor);
    }
}

void SelectCanvas::setRightDisable(bool disable) {
    returnIfEqual(this->rightDisable, disable);
    this->rightDisable = disable;
    if (disable) {
        this->rightButtonCanvas->setFgColor(disabledColor);
    } else {
        this->rightButtonCanvas->setFgColor(this->fgColor);
    }
}

bool SelectCanvas::isLeftDisabled() {
    return this->leftDisable;
}

bool SelectCanvas::isRightDisabled() {
    return this->rightDisable;
}

void SelectCanvas::setDisabledColor(Color16Bit color) {
    returnIfEqual(this->disabledColor, color);
    this->disabledColor = color;
    if (rightDisable) {
        this->rightButtonCanvas->setFgColor(color);
    }
    if (leftDisable) {
        this->leftButtonCanvas->setFgColor(color);
    }
}

Color16Bit SelectCanvas::getDisabledColor() {
    return this->disabledColor;
}

#endif // __SELECTCANVAS_H__