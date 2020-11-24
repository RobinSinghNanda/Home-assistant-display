#include "InputTextRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"
#include "LovelaceConfig/InputTextCardConfig.hpp"

InputTextRowCanvas::InputTextRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme):
   BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    this->entity = hass->getEntity<InputText>(rowConfig->getEntityId());
    if (entity) {
        textCanvas = new TextCanvas(this, 2);
        textCanvas->setWidth(this->getRightX() - nameCanvas->getRightX());
        textCanvas->setHeight(this->getHeight());
        textCanvas->setX(this->getRightX());
        textCanvas->setBorderBottom(1);
        textCanvas->setBorderColor(theme->colorBodyText);
        textCanvas->setFont(FONT_REGULAR_24);
        textCanvas->setHAlign(ALIGN_RIGHT);
        textCanvas->setPaddingRight(10);
        textCanvas->setVAlign(ALIGN_MIDDLE);
        this->onTouch([this](Canvas*, TouchEvent event, TouchEventData)->bool{
                if (isEvent(event, TouchActionTapped)) {
                    return entityCallback(new InputTextCardConfig(entity->getEntityId(), this->rowConfig->getName(),
                    ICON_CHEVRON_LEFT, this->rowConfig->getIcon(), this->rowConfig->getStateColor()));
                }
                return false;
            });
    }
}

InputTextRowCanvas::~InputTextRowCanvas() {

}

void InputTextRowCanvas::update() {
    BaseEntityRowCanvas::update();
    uint16_t nameWidth = tft->textWidth(nameCanvas->getText()+10);
    nameCanvas->setWidth(nameWidth);
    if (entity) {
        if (!entity->isValue(value.c_str())) {
            if (entity->isMode(InputText::MODE_PASSWORD)) {
                uint16_t length = strlen(entity->getValue());
                char password[length+1];
                for (uint16_t i=0;i<length;i++) {
                    password[i] = '*';
                }
                password[length] = '\0';
                textCanvas->setText(password);
                value = entity->getValue();
            } else {
                textCanvas->setText(entity->getValue());
                value = entity->getValue();
            }
            textCanvas->setWidth(tft->textWidth(value.c_str())+10);
            textCanvas->hAlignRight();
        }
        if (entity->isUnavailable()) {
            textCanvas->setVisible(false);
        } else {
            textCanvas->setVisible(true);
        }
    }
}