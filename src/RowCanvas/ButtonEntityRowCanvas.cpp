#include "ButtonEntityRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"

ButtonEntityRowCanvas::ButtonEntityRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme):
   BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    stateCanvas = new TextCanvas(this, 0);

    stateCanvas->setWidth(100);
    stateCanvas->setMarginRight(10);
    stateCanvas->hAlignRight();
    stateCanvas->setHAlign(ALIGN_RIGHT);
    stateCanvas->setVAlign(ALIGN_MIDDLE);
    stateCanvas->setFont(FONT_REGULAR_24);
    stateCanvas->setFgColor(theme->colorSwitchSecondary);
    stateCanvas->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
            if (isEventLosely(event, TouchEvent::TouchActionTapped)) {
                this->hass->callService(entity, Entity::SERVICE_TURN_ON);
                return true;
            }
            return false;
        });
}

ButtonEntityRowCanvas::~ButtonEntityRowCanvas() {

}

void ButtonEntityRowCanvas::update() {
    BaseEntityRowCanvas::update();
    if (entity) {
        if (entity->isUnavailable()) {
            stateCanvas->setFgColor(theme->colorEntityIconUnavailable);
        } else {
            stateCanvas->setFgColor(theme->colorSwitchSecondary);
        }
    }
}