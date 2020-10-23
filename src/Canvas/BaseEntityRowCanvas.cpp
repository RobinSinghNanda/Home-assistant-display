#include "BaseEntityRowCanvas.hpp"

using namespace std::placeholders;
BaseEntityRowCanvas::BaseEntityRowCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {
    iconCanvas = new ImageCanvas(this, 0);
    nameCanvas = new TextCanvas(this, 1);

    iconCanvas->setHAlign(ALIGN_CENTER);
    iconCanvas->setVAlign(ALIGN_MIDDLE);
    iconCanvas->setWidth(SWICTH_ENTITY_MARGIN_LEFT*2 + SWICTH_ENTITY_ICON_WIDTH);
    iconCanvas->setAutoInvert(true);
    iconCanvas->setColorMask(ICON_PRIMARY_COLOR);

    nameCanvas->setVAlign(ALIGN_MIDDLE);
    nameCanvas->setX(iconCanvas->getRightX());
}

void BaseEntityRowCanvas::setIconPath(String iconPath) {
    String prevIconPath = this->getIconPath();
    this->iconCanvas->setPath(iconPath);
    if (prevIconPath != iconPath) {
        if (iconPath != "") {
          this->nameCanvas->setX(this->x + this->iconCanvas->getWidth());
        } else {
          this->nameCanvas->setX(this->x);
        }
    }
}

String BaseEntityRowCanvas::getIconPath() {
    return this->iconCanvas->getPath();
}

void BaseEntityRowCanvas::setName(String name) {
    this->nameCanvas->setText(name);
    
}

String BaseEntityRowCanvas::getName() {
    return this->nameCanvas->getText();
}

void BaseEntityRowCanvas::setDarkMode(bool darkMode) {
    invalidateIfNotEqual(this->darkMode, darkMode);
    Canvas::setDarkMode(darkMode);
    this->iconCanvas->setColorInvert(!darkMode);
}

void BaseEntityRowCanvas::onIconTouch(CanvasTouchCallback callback) {
    this->iconCanvas->onTouch(callback);
}

void BaseEntityRowCanvas::onNameTouch(CanvasTouchCallback callback) {
    this->nameCanvas->onTouch(callback);
}