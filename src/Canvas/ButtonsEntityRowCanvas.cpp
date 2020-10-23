#include "ButtonsEntityRowCanvas.hpp"

ButtonsEntityRowCanvas::ButtonsEntityRowCanvas(Canvas * canvas, uint16_t id, uint16_t numButtons) : BaseEntityRowCanvas(canvas, id){
    this->children.clear();
    this->numButtons = numButtons;
    buttons  = (ImageCanvas **) malloc(sizeof(ImageCanvas *)*numButtons);
    uint16_t buttonWidth = this->getWidth()/numButtons;
    for (uint16_t i=0;i<numButtons;i++) {
        buttons[i] = new ImageCanvas(this, i);
        buttons[i]->setX(buttonWidth*i);
        buttons[i]->setWidth(buttonWidth);
        buttons[i]->setHAlign(ALIGN_CENTER);
        buttons[i]->setVAlign(ALIGN_MIDDLE);
        buttons[i]->setAutoInvert(false);
        buttons[i]->setColorMask(BUTTONS_PRIMARY_COLOR);
    }
}

ButtonsEntityRowCanvas::~ButtonsEntityRowCanvas () {
    free(buttons);
}

void ButtonsEntityRowCanvas::setIconPath(String iconPath, uint16_t index) {
    if (index<numButtons)
        this->buttons[index]->setPath(iconPath);
}

String ButtonsEntityRowCanvas::getIconPath(uint16_t index) {
    if (index<numButtons)
        return this->buttons[index]->getPath();
    return "";
}

void ButtonsEntityRowCanvas::setWidth(uint16_t width) {
    returnIfEqual(this->width, width);
    for (uint8_t i=0;i<numButtons;i++) {
        this->buttons[i]->setX(this->getWidth()*i/numButtons);
        this->buttons[i]->setWidth(this->width/this->numButtons);
    }
}

void ButtonsEntityRowCanvas::onTouch(CanvasTouchCallback callback, uint16_t index) {
    if (index<numButtons)
        this->buttons[index]->onTouch(callback);
}