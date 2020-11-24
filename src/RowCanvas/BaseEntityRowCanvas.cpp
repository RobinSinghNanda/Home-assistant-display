#include "BaseEntityRowCanvas.hpp"
#include "Fonts.hpp"

using namespace std::placeholders;
BaseEntityRowCanvas::BaseEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme) :
    BaseRowCanvas(canvas, id, hass, rowConfig, theme) {
    this->entity = hass->getEntity(rowConfig->getEntityId());
    this->rowConfig = rowConfig;
    if (this->entity) {
        iconCanvas = new ImageCanvas(this, 0);
        nameCanvas = new TextCanvas(this, 1);

        iconCanvas->setHAlign(ALIGN_CENTER);
        iconCanvas->setVAlign(ALIGN_MIDDLE);
        iconCanvas->setWidth(SWICTH_ENTITY_MARGIN_LEFT*2 + SWICTH_ENTITY_ICON_WIDTH);
        iconCanvas->setBgColor(theme->colorBodyBackground.get16Bit());
        iconCanvas->setFgColor(theme->colorEntityIcon.get16Bit());

        nameCanvas->setVAlign(ALIGN_MIDDLE);
        nameCanvas->setX(iconCanvas->getRightX());
        nameCanvas->setFont(FONT_REGULAR_24);
        nameCanvas->setBgColor(theme->colorBodyBackground.get16Bit());
        nameCanvas->setFgColor(theme->colorBodyText.get16Bit());
    }
}

void BaseEntityRowCanvas::update() {
    if (entity) {
        if (rowConfig->getIcon()[0] == '\0') {
            this->iconCanvas->setPath(entity->getIcon());
        } else {
            this->iconCanvas->setPath(rowConfig->getIcon());
        }
        if (rowConfig->getName()[0] == '\0') {
            this->nameCanvas->setText(entity->getFriendlyName());
        } else {
            this->nameCanvas->setText(rowConfig->getName());
        }
        if (entity->isUnavailable()) {
            iconCanvas->setFgColor(theme->colorEntityIconUnavailable);
        } else {
            if (!entity->isOff() && (rowConfig->getStateColor() == 1 || (rowConfig->getStateColor() == 2 && defaultStateColor))) {
                iconCanvas->setFgColor(theme->colorEntityIconEnabled);
            } else {
                iconCanvas->setFgColor(theme->colorEntityIcon);
            }
        }
    }
}

void BaseEntityRowCanvas::setCallback(EntitesRowCanvasCallback callback) {
  this->entityCallback = callback;
}