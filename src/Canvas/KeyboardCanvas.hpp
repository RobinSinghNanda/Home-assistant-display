#ifndef __KEYBOARDCANVAS_H__
#define __KEYBOARDCANVAS_H__

#include "Canvas.hpp"
#include "TextCanvas.hpp"
#include "ImageCanvas.hpp"

class KeyboardCanvas;

typedef std::function<bool(KeyboardCanvas*, const char * text)> KeyboardCanvasStateChangeCallback;

class KeyboardCanvas : public Canvas {
    public:
        enum KeyboardMode {
            KEYBOARD_MODE_ALPHABET,
            KEYBOARD_MODE_SPECIAL_CHAR1,
            KEYBOARD_MODE_SPECIAL_CHAR2
        };
    public:
        KeyboardCanvas(Canvas * canvas, uint16_t id, TextCanvas * textCanvas);
        static constexpr int MAX_NUM_OF_BUTTONS_PER_ROW = 10;
        static constexpr int MAX_NUM_OF_ROWS = 4;
        void resetLayout();
        inline void setPasswordMode(bool passwordMode);
        inline bool isPasswordMode();
        inline void setPasswordVisible(bool show);
        inline bool isPasswordVisible();
        inline void setText(const char * text);
        inline const char * getText();
        inline void onStateChange(KeyboardCanvasStateChangeCallback callback);
        inline void loop();
    protected:
        KeyboardCanvasStateChangeCallback stateChangeCallback;
        bool passwordMode = false;
        uint32_t prevPassTime = 0;
        uint32_t prevCursorTime = 0;
        uint32_t prevPressTime = 0;
        uint32_t repeatTimeout = 1000;
        bool touched = false;
        TextCanvas * textCanvas;
        string displayText = "";
        string displayTextWithCursor = "";
        string displayTextWithPasswordVisible = "";
        uint32_t textLength = 0;
        bool showCursor = true;
        bool showPasswordChar = true;
        bool passwordVisible = false;
        string text = "";
        int32_t cursorPosition = 0;
        int32_t prevCursorPosition = 0;
        KeyboardMode mode = KEYBOARD_MODE_ALPHABET;
        bool capsLock = false;
        bool shift = true;
        Canvas * rows[MAX_NUM_OF_ROWS];
        TextCanvas * row1buttons[MAX_NUM_OF_BUTTONS_PER_ROW];
        TextCanvas * row2buttons[MAX_NUM_OF_BUTTONS_PER_ROW];
        TextCanvas * shiftButton;
        TextCanvas * row3buttons[7];
        IconCanvas * backSpaceButton;
        TextCanvas * specialCharsOrAlphabetTextCanvas;
        TextCanvas * commaCharTextCanvas;
        Canvas * spaceButtonCanvas;
        TextCanvas *  dotCharTextCanvas;
        IconCanvas * specialIconCanvas;
        void updateDisplayTexts();
        void add(string str);
};

const char * KeyboardCanvas::getText() {
    return text.c_str();
}

void KeyboardCanvas::setText(const char * text) {
    this->text = text;
    textLength = this->text.size();
    cursorPosition = textLength;
    shift = !cursorPosition;
    updateDisplayTexts();
}

void KeyboardCanvas::onStateChange(KeyboardCanvasStateChangeCallback callback) {
    this->stateChangeCallback = callback;
}

void KeyboardCanvas::loop() {
    uint32_t currTime = millis();
    if (currTime - prevCursorTime > 1000) {
        showCursor = !showCursor;
        textCanvas->setText((showCursor && !touched)?displayTextWithCursor.c_str():displayText.c_str());
        prevCursorTime = currTime; 
    }
    if (currTime - prevPassTime > 1500 && showPasswordChar) {
        showPasswordChar = false;
        updateDisplayTexts();
        prevPassTime = currTime;
    }
}

void KeyboardCanvas::setPasswordMode(bool passwordMode) {
    if (this->passwordMode != passwordMode) {
        this->passwordMode = passwordMode;
        updateDisplayTexts();
    }
}

bool KeyboardCanvas::isPasswordMode() {
    return this->passwordMode;
}

void KeyboardCanvas::setPasswordVisible(bool show) {
    if (this->passwordVisible != show) {
        this->passwordVisible = show;
        updateDisplayTexts();
    }
}

bool KeyboardCanvas::isPasswordVisible() {
    return this->passwordVisible;
}

#endif // __KEYBOARDCANVAS_H__