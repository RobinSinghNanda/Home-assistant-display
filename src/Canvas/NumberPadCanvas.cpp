#include "NumberPadCanvas.hpp"

NumberPadCanvas::NumberPadCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {
    for (uint8_t i=0;i<12;i++) {
        char text[10];
        snprintf(text, sizeof(text), "%d", i+1);
        numberButtonsCanvas[i] = new TextCanvas(this, i);
        numberButtonsCanvas[i]->setText(text);
        numberButtonsCanvas[i]->onTouch([i, this](Canvas* canvas, TouchEvent event, TouchEventData)->bool{
                if (isEventLosely(event, TouchEvent::TouchActionPressed)) {
                    if ((i < 9 && !this->isNumbersDisabled()) || 
                        (i == 9 && !this->isLeftButtonDisabled()) ||
                        (i == 11 && !this->isRightButtonDisabled()) ||
                        (i == 10 && !this->isNumbersDisabled())) {
                        canvas->setBgColor(this->pressedColor);
                    }
                } else if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                    canvas->setBgColor(this->bgColor);
                    if (i<9) {
                        if (this->isNumbersDisabled()) {
                            return false;
                        }
                        return pressCallback(this, i+1);        
                    } else if (i == 9) {
                        if (this->isLeftButtonDisabled()) {
                            return false;
                        }
                        return pressCallback(this, 10);   
                    } else if (i == 10) {
                        if (this->isNumbersDisabled()) {
                            return false;
                        }
                        return pressCallback(this, 0);   
                    } else {
                        if (this->isRightButtonDisabled()) {
                            return false;
                        }
                        return pressCallback(this, 11);   
                    }
                } else {
                    canvas->setBgColor(this->bgColor);
                    return false;
                }
                return false;
            });
    }
    numberButtonsCanvas[9]->setText("");
    numberButtonsCanvas[10]->setText("0");
    numberButtonsCanvas[11]->setText("");
    pressCallback = [](NumberPadCanvas*, uint8_t)->bool{return false;};
}

void NumberPadCanvas::resetLayout() {
    uint16_t size = (this->width/3>this->height/4)?this->height/4:this->width/3;
    uint16_t xOffset = getButtonX();
    uint16_t yOffset = getButtonY();
    for (uint8_t i=0;i<12;i++) {
        numberButtonsCanvas[i]->setWidth(size);
        numberButtonsCanvas[i]->setHeight(size);
        numberButtonsCanvas[i]->setHAlign(ALIGN_CENTER);
        numberButtonsCanvas[i]->setVAlign(ALIGN_MIDDLE);
        uint8_t gridX = ((i)%3)*size;
        uint8_t gridY = ((i)/3)*size;
        numberButtonsCanvas[i]->setX(xOffset + gridX);
        numberButtonsCanvas[i]->setY(yOffset + gridY);
    }
}