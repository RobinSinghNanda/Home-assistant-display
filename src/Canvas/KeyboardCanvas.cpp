#include "KeyboardCanvas.hpp"
#include "Fonts.hpp"

KeyboardCanvas::KeyboardCanvas(Canvas * canvas, uint16_t id, TextCanvas * textCanvas) : Canvas(canvas, id) {
    this->textCanvas = textCanvas;
    uint16_t buttonWidth = this->getWidth()/MAX_NUM_OF_BUTTONS_PER_ROW;
    this->height = 40*MAX_NUM_OF_ROWS;
    uint16_t buttonHeight = this->getHeight()/MAX_NUM_OF_ROWS;
    for (uint8_t i = 0; i < MAX_NUM_OF_ROWS ; i++) {
        rows[i] = new Canvas(this, i);
        rows[i]->setHeight(buttonHeight);
        rows[i]->setY(this->y + i*40);
    }
    for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
        row1buttons[j] = new TextCanvas(rows[0], j);
        row1buttons[j]->setWidth(buttonWidth);
        row1buttons[j]->setX(this->x+j*buttonWidth);
        row1buttons[j]->setHAlign(ALIGN_CENTER);
        row1buttons[j]->setVAlign(ALIGN_MIDDLE);
        row1buttons[j]->setFont(FONT_REGULAR_24);
    }
    rows[1]->setDrawBackgroundEnable(true);
    for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
        row2buttons[j] = new TextCanvas(rows[1], j);
        row2buttons[j]->setWidth(buttonWidth);
        row2buttons[j]->setX(this->x+buttonWidth/2+j*buttonWidth);
        row2buttons[j]->setHAlign(ALIGN_CENTER);
        row2buttons[j]->setVAlign(ALIGN_MIDDLE);
        row2buttons[j]->setFont(FONT_REGULAR_24);
    }
    shiftButton = new TextCanvas(rows[2], 7);
    shiftButton->setWidth(buttonWidth*1.5);
    shiftButton->setX(x);
    shiftButton->setFont(FONT_REGULAR_24);
    shiftButton->setHAlign(ALIGN_CENTER);
    shiftButton->setVAlign(ALIGN_MIDDLE);
    for (uint8_t j=0;j< 7;j++) {
        row3buttons[j] = new TextCanvas(rows[2], j);
        row3buttons[j]->setWidth(buttonWidth);
        row3buttons[j]->setX(x+buttonWidth*1.5 + j*buttonWidth);
        row3buttons[j]->setHAlign(ALIGN_CENTER);
        row3buttons[j]->setVAlign(ALIGN_MIDDLE);
        row3buttons[j]->setFont(FONT_REGULAR_24);
    }
    backSpaceButton = new IconCanvas(rows[2], 8);
    backSpaceButton->setWidth(buttonWidth*1.5);
    backSpaceButton->setX(x+8.5*buttonWidth);
    backSpaceButton->setHAlign(ALIGN_CENTER);
    backSpaceButton->setVAlign(ALIGN_MIDDLE);
    backSpaceButton->setPath(ICON_BACKSPACE_OUTLINE);
    specialCharsOrAlphabetTextCanvas = new TextCanvas(rows[3], 0);
    commaCharTextCanvas = new TextCanvas(rows[3], 1);
    spaceButtonCanvas = new Canvas(rows[3], 2);
    dotCharTextCanvas  = new TextCanvas(rows[3], 3);
    specialIconCanvas = new IconCanvas(rows[3], 4);
    specialCharsOrAlphabetTextCanvas->setWidth(buttonWidth*2);
    specialCharsOrAlphabetTextCanvas->setFont(FONT_REGULAR_24);
    specialCharsOrAlphabetTextCanvas->setHAlign(ALIGN_CENTER);
    specialCharsOrAlphabetTextCanvas->setVAlign(ALIGN_MIDDLE);
    commaCharTextCanvas->setWidth(buttonWidth);
    commaCharTextCanvas->setX(x+2*buttonWidth);
    commaCharTextCanvas->setFont(FONT_REGULAR_24);
    commaCharTextCanvas->setHAlign(ALIGN_CENTER);
    commaCharTextCanvas->setVAlign(ALIGN_MIDDLE);
    spaceButtonCanvas->setWidth(buttonWidth*4);
    spaceButtonCanvas->setX(x+3*buttonWidth);
    spaceButtonCanvas->setDrawBackgroundEnable(true);
    spaceButtonCanvas->setPadding(5);
    dotCharTextCanvas->setWidth(buttonWidth);
    dotCharTextCanvas->setX(x+7*buttonWidth);
    dotCharTextCanvas->setFont(FONT_REGULAR_24);
    dotCharTextCanvas->setHAlign(ALIGN_CENTER);
    dotCharTextCanvas->setVAlign(ALIGN_MIDDLE);
    specialIconCanvas->setWidth(buttonWidth*2);
    specialIconCanvas->setX(x+8*buttonWidth);
    specialIconCanvas->setHAlign(ALIGN_CENTER);
    specialIconCanvas->setVAlign(ALIGN_MIDDLE);
    specialIconCanvas->setPadding(5);

    spaceButtonCanvas->setBgColor(Color32Bit(0x7F7F7F));
    
    commaCharTextCanvas->setText(",");
    dotCharTextCanvas->setText(".");
    specialIconCanvas->setPath(ICON_KEYBOARD_RETURN);
    specialIconCanvas->setBgColor(Color32Bit(0x03a9f4));
    specialIconCanvas->setDrawBackgroundEnable(true);

    shiftButton->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (this->mode == KEYBOARD_MODE_ALPHABET) {
                if (isEvent(event, TouchEvent::TouchActionDoubleTapped)) {
                    capsLock = !capsLock;
                    return true;
                } else if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                    if (capsLock) {
                        this->capsLock = false;
                        this->shift = false;
                    } else {
                        this->shift = !this->shift;
                    }
                    return true;
                }
            } else if (this->mode == KEYBOARD_MODE_SPECIAL_CHAR1) {
                if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                        this->mode = KEYBOARD_MODE_SPECIAL_CHAR2;
                    return true;
                }
            } else if (this->mode == KEYBOARD_MODE_SPECIAL_CHAR2) {
                if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                        this->mode = KEYBOARD_MODE_SPECIAL_CHAR1;
                    return true;
                }
            }
            return false;
        });
    specialCharsOrAlphabetTextCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                if (this->mode == KEYBOARD_MODE_ALPHABET) {
                    this->mode = KEYBOARD_MODE_SPECIAL_CHAR1;
                } else {
                    this->mode = KEYBOARD_MODE_ALPHABET;
                    this->shift = !this->cursorPosition;
                }
                return true;
            }
            return false;
        });
    backSpaceButton->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                uint32_t time = millis();
                if (time - prevPressTime > repeatTimeout) {
                    if (cursorPosition != 0) {
                        text = text.substr(0, cursorPosition-1)+text.substr(cursorPosition, textLength - cursorPosition);
                        cursorPosition--;
                        prevCursorPosition = cursorPosition;
                        updateDisplayTexts();
                        shift = !cursorPosition;
                    }
                    prevPressTime = time;
                }
                return true;
            } else {
                prevPressTime = 0;
            }
            return false;
        });
    textCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchActionDraged) || isEvent(event, TouchActionLongPressedAndDragged)) {
                touched = true;
                int16_t delta = (eventData.endX -eventData.startX)/30;
                cursorPosition = prevCursorPosition + delta;
                if (cursorPosition < 0) {
                    cursorPosition = 0;
                } else if (cursorPosition > textLength) {
                    cursorPosition = textLength;
                }
                if (cursorPosition != prevCursorPosition) {
                    shift = ~cursorPosition;
                    updateDisplayTexts();
                }
                return true;
            } else if (isEvent(event, TouchActionDragReleased)) {
                touched = false;
                prevCursorPosition = cursorPosition;
                return true;
            } else if (isEventLosely(event, TouchActionPressed)) {
                touched = true;
                return true;
            } else {
                touched = false;
            }
            return false;
        });
    specialIconCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                stateChangeCallback(this, this->text.c_str());
                return true;
            }
            return false;
        });
    spaceButtonCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                add(" ");
                return true;
            }
            return false;
        });
    commaCharTextCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                add(",");
                return true;
            }
            return false;
        });
    dotCharTextCanvas->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                add(".");
                return true;
            }
            return false;
        });
    this->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                    isEvent(event, TouchEvent::TouchActionLongPressed)) {
                uint16_t buttonWidth = this->getWidth()/MAX_NUM_OF_BUTTONS_PER_ROW;
                uint16_t buttonHeight = this->getHeight()/MAX_NUM_OF_ROWS;
                uint16_t rowIndex = (eventData.startY - this->y)/buttonHeight;
                uint16_t buttonIndex;
                string buttonText = "";
                if (rowIndex == 0 || rowIndex == 1) {
                    buttonIndex = (eventData.startX - rows[rowIndex]->get(0)->getX())/buttonWidth;
                    if (buttonIndex < MAX_NUM_OF_BUTTONS_PER_ROW) {
                        buttonText = ((TextCanvas *)(rows[rowIndex]->get(buttonIndex)))->getText().c_str();
                    }
                } else if (rowIndex == 2) {
                    buttonIndex = (eventData.startX - rows[rowIndex]->get(1)->getX())/buttonWidth;
                    if (buttonIndex < 7) {
                        buttonText = ((TextCanvas *)(rows[rowIndex]->get(buttonIndex)))->getText().c_str();
                    }
                }
                if (buttonText != "") {
                    add(buttonText);
                }
            }
            return false;
        });
    resetLayout();
    stateChangeCallback = [](KeyboardCanvas*, const char * text)->bool{return false;};
}

KeyboardCanvas::KeyboardMode prevMode = KeyboardCanvas::KeyboardMode::KEYBOARD_MODE_SPECIAL_CHAR1;

void KeyboardCanvas::resetLayout() {
    uint16_t buttonWidth = this->getWidth()/MAX_NUM_OF_BUTTONS_PER_ROW;
    uint16_t buttonHeight = this->getHeight()/MAX_NUM_OF_ROWS;
    for (uint8_t i = 0; i < MAX_NUM_OF_ROWS ; i++) {
        rows[i]->setY(this->y + i*buttonHeight);
    }
    for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
        row1buttons[j]->setY(rows[0]->getY());
        row1buttons[j]->setX(x+j*this->getWidth()/MAX_NUM_OF_BUTTONS_PER_ROW);
    }
    for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
        row2buttons[j]->setY(rows[1]->getY());
    }
    for (uint8_t j=0;j< 7;j++) {
        row3buttons[j]->setY(rows[2]->getY());
    }
    shiftButton->setY(rows[2]->getY());
    backSpaceButton->setY(rows[2]->getY());
    specialIconCanvas->setY(rows[3]->getY());
    dotCharTextCanvas->setY(rows[3]->getY());
    spaceButtonCanvas->setY(rows[3]->getY());
    commaCharTextCanvas->setY(rows[3]->getY());
    specialCharsOrAlphabetTextCanvas->setY(rows[3]->getY());
    if (mode == KEYBOARD_MODE_ALPHABET && (shift || capsLock)) {
        for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
            row2buttons[j]->setX(x+buttonWidth/2+j*buttonWidth);
        }
        row1buttons[0]->setText("Q");
        row1buttons[1]->setText("W");
        row1buttons[2]->setText("E");
        row1buttons[3]->setText("R");
        row1buttons[4]->setText("T");
        row1buttons[5]->setText("Y");
        row1buttons[6]->setText("U");
        row1buttons[7]->setText("I");
        row1buttons[8]->setText("O");
        row1buttons[9]->setText("P");

        row2buttons[0]->setText("A");
        row2buttons[1]->setText("S");
        row2buttons[2]->setText("D");
        row2buttons[3]->setText("F");
        row2buttons[4]->setText("G");
        row2buttons[5]->setText("H");
        row2buttons[6]->setText("J");
        row2buttons[7]->setText("K");
        row2buttons[8]->setText("L");
        row2buttons[9]->setText("");

        row3buttons[0]->setText("Z");
        row3buttons[1]->setText("X");
        row3buttons[2]->setText("C");
        row3buttons[3]->setText("V");
        row3buttons[4]->setText("B");
        row3buttons[5]->setText("N");
        row3buttons[6]->setText("M");
        if (prevMode != mode) 
            rows[1]->invalidate();
        if (capsLock)
            shiftButton->setText("\u00BF");
        else
            shiftButton->setText("\u00C1");
        specialCharsOrAlphabetTextCanvas->setText("?123");
    } else if (mode == KEYBOARD_MODE_ALPHABET) {
        for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
            row2buttons[j]->setX(x+buttonWidth/2+j*buttonWidth);
        }
        row1buttons[0]->setText("q");
        row1buttons[1]->setText("w");
        row1buttons[2]->setText("e");
        row1buttons[3]->setText("r");
        row1buttons[4]->setText("t");
        row1buttons[5]->setText("y");
        row1buttons[6]->setText("u");
        row1buttons[7]->setText("i");
        row1buttons[8]->setText("o");
        row1buttons[9]->setText("p");

        row2buttons[0]->setText("a");
        row2buttons[1]->setText("s");
        row2buttons[2]->setText("d");
        row2buttons[3]->setText("f");
        row2buttons[4]->setText("g");
        row2buttons[5]->setText("h");
        row2buttons[6]->setText("j");
        row2buttons[7]->setText("k");
        row2buttons[8]->setText("l");
        row2buttons[9]->setText("");

        row3buttons[0]->setText("z");
        row3buttons[1]->setText("x");
        row3buttons[2]->setText("c");
        row3buttons[3]->setText("v");
        row3buttons[4]->setText("b");
        row3buttons[5]->setText("n");
        row3buttons[6]->setText("m");
        if (prevMode != mode) 
            rows[1]->invalidate();
        shiftButton->setText("\u00C0");
        specialCharsOrAlphabetTextCanvas->setText("?123");
    } else if (mode == KEYBOARD_MODE_SPECIAL_CHAR1) {
        for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
            row2buttons[j]->setX(x+j*buttonWidth);
        }
        row1buttons[0]->setText("1");
        row1buttons[1]->setText("2");
        row1buttons[2]->setText("3");
        row1buttons[3]->setText("4");
        row1buttons[4]->setText("5");
        row1buttons[5]->setText("6");
        row1buttons[6]->setText("7");
        row1buttons[7]->setText("8");
        row1buttons[8]->setText("9");
        row1buttons[9]->setText("0");

        row2buttons[0]->setText("@");
        row2buttons[1]->setText("#");
        row2buttons[2]->setText("$");
        row2buttons[3]->setText("_");
        row2buttons[4]->setText("&");
        row2buttons[5]->setText("-");
        row2buttons[6]->setText("+");
        row2buttons[7]->setText("(");
        row2buttons[8]->setText(")");
        row2buttons[9]->setText("/");

        row3buttons[0]->setText("*");
        row3buttons[1]->setText("\"");
        row3buttons[2]->setText("'");
        row3buttons[3]->setText(":");
        row3buttons[4]->setText(";");
        row3buttons[5]->setText("!");
        row3buttons[6]->setText("?");
        if (prevMode != mode) 
            rows[1]->invalidate();
        shiftButton->setText("=\\<");
        specialCharsOrAlphabetTextCanvas->setText("ABC");
    } else if (mode == KEYBOARD_MODE_SPECIAL_CHAR2) {
        for (uint8_t j=0;j< MAX_NUM_OF_BUTTONS_PER_ROW;j++) {
            row2buttons[j]->setX(x+j*buttonWidth);
        }
        row1buttons[0]->setText("~");
        row1buttons[1]->setText("`");
        row1buttons[2]->setText("|");
        row1buttons[3]->setText("•");
        row1buttons[4]->setText("√");
        row1buttons[5]->setText("π");
        row1buttons[6]->setText("÷");
        row1buttons[7]->setText("×");
        row1buttons[8]->setText("¶");
        row1buttons[9]->setText("∆");

        row2buttons[0]->setText("€");
        row2buttons[1]->setText("¥");
        row2buttons[2]->setText("$");
        row2buttons[3]->setText("¢");
        row2buttons[4]->setText("^");
        row2buttons[5]->setText("°");
        row2buttons[6]->setText("=");
        row2buttons[7]->setText("{");
        row2buttons[8]->setText("}");
        row2buttons[9]->setText("\\");

        row3buttons[0]->setText("%");
        row3buttons[1]->setText("©");
        row3buttons[2]->setText("®");
        row3buttons[3]->setText("™");
        row3buttons[4]->setText("Σ");
        row3buttons[5]->setText("[");
        row3buttons[6]->setText("]");
        if (prevMode != mode) 
            rows[1]->invalidate();
        shiftButton->setText("?123");
        specialCharsOrAlphabetTextCanvas->setText("ABC");
    }
    prevMode = mode;
}

void KeyboardCanvas::updateDisplayTexts() {
    textLength = text.size();
    if (passwordMode && !passwordVisible) {
        displayText = "";
        displayTextWithCursor = "";
        for (uint8_t i = 0;i<cursorPosition;i++) {
            if (showPasswordChar && i == cursorPosition - 1) {
                displayText = displayText + text.at(cursorPosition-1);
                displayTextWithCursor = displayTextWithCursor + text.at(cursorPosition-1);
            } else {
                displayText = displayText + "*";
                displayTextWithCursor = displayTextWithCursor + "*";
            }
        }
        displayText = displayText + "|";
        displayTextWithCursor = displayTextWithCursor + "\u00C2";
        for (uint8_t i = cursorPosition;i<textLength;i++) {
            displayText = displayText + "*";
            displayTextWithCursor = displayTextWithCursor + "*";
        }
    } else {
        displayText = text.substr(0, cursorPosition)+"|"+text.substr(cursorPosition, textLength - cursorPosition);
        displayTextWithCursor = text.substr(0, cursorPosition)+"\u00C2"+text.substr(cursorPosition, textLength - cursorPosition);
    }
    textCanvas->setText((showCursor && !touched)?displayTextWithCursor.c_str():displayText.c_str());
}

void KeyboardCanvas::add(string str) {
    text = text.substr(0, cursorPosition)+str+text.substr(cursorPosition, textLength - cursorPosition);
    cursorPosition++;
    prevCursorPosition = cursorPosition;
    shift = !cursorPosition;
    if (passwordMode && !passwordVisible) {
        showPasswordChar = true;
        prevPassTime = millis();
    }
    updateDisplayTexts();
}