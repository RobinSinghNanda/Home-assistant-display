#ifndef __NUMBERPADCANVAS_H__
#define __NUMBERPADCANVAS_H__

#include "Canvas.hpp"
#include "TextCanvas.hpp"

class NumberPadCanvas ;

typedef std::function<bool(NumberPadCanvas*, uint8_t number)> NumberPadCanvasPressCallback;

class NumberPadCanvas : public Canvas {
    public:
        NumberPadCanvas(Canvas * canvas, uint16_t id);
        virtual void resetLayout();
        inline void setFont(const char * font);
        inline const char * getFont();
        inline uint16_t getButtonSize();
        inline void setHBorder(uint16_t border);
        inline void setHBorder(uint16_t borderLeft, uint16_t borderRight);
        inline void setVBorder(uint16_t border);
        inline void setVBorder(uint16_t borderTop, uint16_t borderBottom);
        inline void setBorder(uint16_t border);
        inline void setBorder(uint16_t border_horizontal, uint16_t border_vertical);
        inline void setBorder(uint16_t borderLeft, uint16_t borderRight, uint16_t borderTop, uint16_t borderBottom);
        inline void setBorderLeft(uint16_t borderLeft);
        inline void setBorderRight(uint16_t borderRight);
        inline void setBorderTop(uint16_t borderTop);
        inline void setBorderBottom(uint16_t borderBottom);
        inline void setBorderColor(Color16Bit borderColor);
        inline void setLeftButtonText(const char * text);
        inline void setRightButtonText(const char * text);
        inline void onNumberPress(NumberPadCanvasPressCallback callback);
        inline void setDisabled(bool disable);
        inline bool isDisabled();
        inline void setDisabledColor(Color16Bit color);
        inline Color16Bit getDisabledColor();
        inline void setEnabledColor(Color16Bit color);
        inline Color16Bit getEnabledColor();
        inline void setPressedColor(Color16Bit color);
        inline Color16Bit getPressedColor();
        inline void setNumbersDisable(bool disable);
        inline void setLeftButtonDisable(bool disable);
        inline void setRightButtonDisable(bool disable);
        inline bool isNumbersDisabled();
        inline bool isLeftButtonDisabled();
        inline bool isRightButtonDisabled();
    protected:
        Color16Bit pressedColor = Color32Bit(0x7F7F7F);
        string font;
        TextCanvas * numberButtonsCanvas[12];
        uint16_t buttonHeight = 50;
        uint16_t buttonWidth = 50;
        Color16Bit disabledColor = Color32Bit(0x7F7F7F);
        Color16Bit enabledColor = TFT_WHITE;
        bool numbersDisabled = false;
        bool leftButtonDisabled = false;
        bool rightButtonDisabled = false;
        NumberPadCanvasPressCallback pressCallback;
        inline uint16_t getButtonX();
        inline uint16_t getButtonY();
};

void NumberPadCanvas::setFont(const char * font) {
    invalidateIfNotEqual(this->font, font);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setFont(font);
    }
}

const char * NumberPadCanvas::getFont() {
    return this->font.c_str();
}

uint16_t NumberPadCanvas::getButtonSize() {
    return (this->width/3>this->height/4)?this->height/4:this->width/3;
}

uint16_t NumberPadCanvas::getButtonX() {
    uint16_t size = getButtonSize();
    if (hAlign == ALIGN_LEFT){
        return getDrawX();
    } else if (hAlign == ALIGN_CENTER) {
        return getDrawX()+(getDrawableWidth()-size*3)/2;
    } else {
        return getDrawX()+(width-size*3);
    }
}

uint16_t NumberPadCanvas::getButtonY() {
    uint16_t size = getButtonSize();
    if (vAlign == ALIGN_TOP){
        return getDrawY();
    } else if (vAlign == ALIGN_MIDDLE) {
        return getDrawY() + (getDrawableHeight()-size*4)/2;
    } else {
        return getDrawY() + (getDrawableHeight()-size*4);
    }
}

void NumberPadCanvas::setHBorder(uint16_t border) {
    Canvas::setHBorder(border);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setHBorder(border);
    }
}

void NumberPadCanvas::setHBorder(uint16_t borderLeft, uint16_t borderRight){
    Canvas::setHBorder(borderLeft, borderRight);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setHBorder(borderLeft, borderRight);
    }
}

void NumberPadCanvas::setVBorder(uint16_t border){
    Canvas::setVBorder(border);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setVBorder(border);
    }
}

void NumberPadCanvas::setVBorder(uint16_t borderTop, uint16_t borderBottom){
    Canvas::setVBorder(borderTop, borderBottom);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setVBorder(borderTop, borderBottom);
    }
}

void NumberPadCanvas::setBorder(uint16_t border){
    Canvas::setBorder(border);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorder(border);
    }
}

void NumberPadCanvas::setBorder(uint16_t border_horizontal, uint16_t border_vertical){
    Canvas::setBorder(border_horizontal, border_vertical);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorder(border_horizontal, border_vertical);
    }
}

void NumberPadCanvas::setBorder(uint16_t borderLeft, uint16_t borderRight, uint16_t borderTop, uint16_t borderBottom){
    Canvas::setBorder(borderLeft, borderRight, borderTop, borderBottom);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorder(borderLeft, borderRight, borderTop, borderBottom);
    }
}

void NumberPadCanvas::setBorderLeft(uint16_t borderLeft){
    Canvas::setBorderLeft(borderLeft);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorderLeft(borderLeft);
    }
}

void NumberPadCanvas::setBorderRight(uint16_t borderRight){
    Canvas::setBorderRight(borderRight);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorderRight(borderRight);
    }
}

void NumberPadCanvas::setBorderTop(uint16_t borderTop){
    Canvas::setBorderTop(borderTop);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorderTop(borderTop);
    }
}

void NumberPadCanvas::setBorderBottom(uint16_t borderBottom){
    Canvas::setBorderBottom(borderBottom);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorderBottom(borderBottom);
    }
}

void NumberPadCanvas::setBorderColor(Color16Bit borderColor) {
    Canvas::setBorderColor(borderColor);
    for (TextCanvas * button : numberButtonsCanvas) {
        button->setBorderColor(borderColor);
    }
}

void NumberPadCanvas::setLeftButtonText(const char * text) {
    numberButtonsCanvas[9]->setText(text);
}

void NumberPadCanvas::setRightButtonText(const char * text) {
    numberButtonsCanvas[11]->setText(text);
}

void NumberPadCanvas::onNumberPress(NumberPadCanvasPressCallback callback) {
    this->pressCallback = callback;
}

void NumberPadCanvas::setDisabled(bool disable) {
    setNumbersDisable(disable);
    setLeftButtonDisable(disable);
    setRightButtonDisable(disable);
}

void NumberPadCanvas::setNumbersDisable(bool disable) {
    invalidateIfNotEqual(this->numbersDisabled, disable);
    for (uint8_t i=0;i<9;i++) {
        numberButtonsCanvas[i]->setFgColor(disable?disabledColor:enabledColor);
    }
    numberButtonsCanvas[10]->setFgColor(disable?disabledColor:enabledColor);
}

void NumberPadCanvas::setLeftButtonDisable(bool disable) {
    invalidateIfNotEqual(this->leftButtonDisabled, disable);
    numberButtonsCanvas[9]->setFgColor(disable?disabledColor:enabledColor);
}

void NumberPadCanvas::setRightButtonDisable(bool disable) {
    invalidateIfNotEqual(this->rightButtonDisabled, disable);
    numberButtonsCanvas[11]->setFgColor(disable?disabledColor:enabledColor);
}

bool NumberPadCanvas::isDisabled() {
    return this->numbersDisabled || this->leftButtonDisabled || this->rightButtonDisabled;
}

void NumberPadCanvas::setDisabledColor(Color16Bit color) {
    invalidateIfNotEqual(this->disabledColor, color);
    if (numbersDisabled) {
        for (uint8_t i=0;i<9;i++) {
            numberButtonsCanvas[i]->setFgColor(disabledColor);
        }
        numberButtonsCanvas[10]->setFgColor(disabledColor);
    }
    if (leftButtonDisabled) {
        numberButtonsCanvas[9]->setFgColor(disabledColor);
    }
    if (rightButtonDisabled) {
        numberButtonsCanvas[11]->setFgColor(disabledColor);
    }
}

bool NumberPadCanvas::isNumbersDisabled() {
    return this->numbersDisabled;
}

bool NumberPadCanvas::isLeftButtonDisabled() {
    return this->leftButtonDisabled;
}

bool NumberPadCanvas::isRightButtonDisabled() {
    return this->rightButtonDisabled;
}

Color16Bit NumberPadCanvas::getDisabledColor() {
    return this->disabledColor;
}

void NumberPadCanvas::setEnabledColor(Color16Bit color) {
    invalidateIfNotEqual(this->enabledColor, color);
    if (!numbersDisabled) {
        for (uint8_t i=0;i<9;i++) {
            numberButtonsCanvas[i]->setFgColor(enabledColor);
        }
        numberButtonsCanvas[10]->setFgColor(enabledColor);
    }
    if (!leftButtonDisabled) {
        numberButtonsCanvas[9]->setFgColor(enabledColor);
    }
    if (!rightButtonDisabled) {
        numberButtonsCanvas[11]->setFgColor(enabledColor);
    }
}

Color16Bit NumberPadCanvas::getEnabledColor() {
    return this->enabledColor;
}

void NumberPadCanvas::setPressedColor(Color16Bit color) {
    invalidateIfNotEqual(this->pressedColor, color);
}

Color16Bit NumberPadCanvas::getPressedColor() {
    return this->pressedColor;
}

#endif // __NUMBERPADCANVAS_H__