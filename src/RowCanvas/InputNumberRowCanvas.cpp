#include "InputNumberRowCanvas.hpp"
#include "Fonts.hpp"
#include "Support.hpp"

InputNumberRowCanvas::InputNumberRowCanvas(Canvas * canvas, uint16_t id, HomeAssistantManager * hass, DefaultRowConfig * rowConfig, LcdTheme * theme):
   BaseEntityRowCanvas(canvas, id, hass, rowConfig, theme) {
    this->entity = hass->getEntity<InputNumber>(rowConfig->getEntityId());
    if (entity) {
        selectCanvas = new TextSelectCanvas(this, 2);
        selectCanvas->setHAlign(ALIGN_RIGHT);
        selectCanvas->setWidth(140);
        selectCanvas->hAlignRight();
        selectCanvas->setFont(FONT_REGULAR_24);
        selectCanvas->setDisabledColor(theme->colorEntityIconUnavailable);
        selectCanvas->onStateChange([this](SelectCanvas*, SelectCanvasStateChangeEvent event, const char * &value)->bool {       
                if (event == CHANGE_NEXT) {
                    this->hass->callService(entity, InputNumber::SERVICE_INCREMENT);
                } else {
                    this->hass->callService(entity, InputNumber::SERVICE_DECREMENT);
                }
                return true;
            });

        nameCanvas->setWidth(this->getWidth() - selectCanvas->getWidth());
        this->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool {
                if (isEvent(event, TouchActionLongPressedAndDragged)) {
                    touched = true;
                    if (entity->isAvailable()) {
                        int32_t deltaValue = (eventData.endX - eventData.startX)/SWIPE_THRESHOLD;
                        tmpValue = entity->getValue() + (deltaValue*entity->getStep());
                        if (tmpValue < entity->getMin()) {
                            tmpValue = entity->getMin();
                        } else if (tmpValue > entity->getMax()) {
                            tmpValue = entity->getMax();
                        }
                        return true;
                    }
                } else if (isEvent(event, TouchActionLongPressedAndDraggedReleased)) {
                    touched = false;
                    if (entity->isAvailable()) {
                        if (tmpValue != entity->getValue()) {
                            entity->setValue(tmpValue);
                            this->hass->callService(entity, InputNumber::SERVICE_SET_VALUE);
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

InputNumberRowCanvas::~InputNumberRowCanvas() {

}

void InputNumberRowCanvas::update() {
    BaseEntityRowCanvas::update();
    if (entity) {
        if (entity->isUnavailable()) {
            iconCanvas->setFgColor(theme->colorEntityIconUnavailable);
            selectCanvas->setVisible(false);
        } else {
            if (touched) {
                selectCanvas->setVisible(true);
            } else {
                selectCanvas->setVisible(true);
            }
            float value = entity->getValue();
            if (value == entity->getMin()) {
                selectCanvas->setLeftDisable(true);
                selectCanvas->setRightDisable(false);
            } else if (value == entity->getMax()){
                selectCanvas->setRightDisable(true);
                selectCanvas->setLeftDisable(false);
            } else {
                selectCanvas->setRightDisable(false);
                selectCanvas->setLeftDisable(false);
            }
            iconCanvas->setFgColor(theme->colorEntityIcon);
            if (touched) {
                value = tmpValue;
            }
            
            char str[12];
            Support::snprintfloat(str, sizeof(str), value, 3);
            this->selectCanvas->setValue(str);
        }
    }
}