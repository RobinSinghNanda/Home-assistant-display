#include "InputSelectRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"

InputSelectRowCanvas::InputSelectRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme):
   BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    this->entity = hass->getEntity<InputSelect>(rowConfig->getEntityId());
    if (entity) {
        selectCanvas = new TextSelectCanvas(this, 2);
        selectCanvas->setHAlign(ALIGN_RIGHT);
        selectCanvas->setWidth(140);
        selectCanvas->hAlignRight();
        selectCanvas->setFont(FONT_REGULAR_24);
        selectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                if (event == CHANGE_NEXT) {
                    this->hass->callService(entity, InputSelect::SERVICE_SELECT_NEXT);
                } else {
                    this->hass->callService(entity, InputSelect::SERVICE_SELECT_PREVIOUS);
                }
                return true;
            });

        nameCanvas->setWidth(this->getWidth() - selectCanvas->getWidth());
        this->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool {
                if (isEvent(event, TouchActionLongPressedAndDragged)) {
                    touched = true;
                    if (entity->isAvailable()) {
                        int32_t deltaValue = (eventData.endX - eventData.startX)/SWIPE_THRESHOLD;
                        tmpIndex = entity->getOptionIndex() + (deltaValue);
                        return true;
                    }
                } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
                    touched = false;
                    if (entity->isAvailable()) {
                        if (tmpIndex != entity->getOptionIndex()) {
                            entity->setOption(tmpIndex);
                            this->hass->callService(entity, InputSelect::SERVICE_SELECT_OPTION);
                        }
                        return true;
                    }
                } else {
                    touched = false;
                }
                return false;
            });
    }
}

InputSelectRowCanvas::~InputSelectRowCanvas() {

}

void InputSelectRowCanvas::update() {
    BaseEntityRowCanvas::update();
    if (entity) {
        if (entity->isUnavailable()) {
            iconCanvas->setFgColor(theme->colorEntityIconUnavailable);
            selectCanvas->setVisible(false);
        } else {
            int8_t optionIndex = entity->getOptionIndex();
            iconCanvas->setFgColor(theme->colorEntityIcon);
            if (touched) {
                optionIndex = tmpIndex;
            }
            selectCanvas->setVisible(true);
            this->selectCanvas->setValue(entity->getOption(optionIndex));
        }
    }
}