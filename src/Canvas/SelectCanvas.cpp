#include "SelectCanvas.hpp"

SelectCanvas::SelectCanvas(Canvas * canvas, uint16_t id) : Canvas (canvas, id) {
    stateChangeCallback = [](SelectCanvas *, SelectCanvasStateChangeEvent, const char * &value)->bool{return false;};
    leftButtonCanvas = new IconCanvas(this, 0);
    rightButtonCanvas = new IconCanvas(this, 1);

    leftButtonCanvas->setWidth(24);
    leftButtonCanvas->setVAlign(ALIGN_MIDDLE);
    leftButtonCanvas->setPath(ICON_CHEVRON_LEFT);
    leftButtonCanvas->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                isEvent(event, TouchEvent::TouchActionLongPressed)) {
                return selectPrev();
            } else if (isEvent(event, TouchEvent::TouchActionDragReleased) || 
                    isEvent(event, TouchEvent::TouchActionLongPressedAndDraggedReleased)) {
                int16_t delta = (eventData.endX -eventData.startX)/swipeThreshold;
                if (delta > 1) {
                    return selectPrev();
                } else if (delta < 1) {
                    return selectNext();
                }
            }
            return false;
        });

    rightButtonCanvas->setWidth(24);
    rightButtonCanvas->setVAlign(ALIGN_MIDDLE);
    rightButtonCanvas->setPath(ICON_CHEVRON_RIGHT);
    rightButtonCanvas->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                isEvent(event, TouchEvent::TouchActionLongPressed)) {
                return selectNext();
            } else if (isEvent(event, TouchEvent::TouchActionDragReleased) || 
                    isEvent(event, TouchEvent::TouchActionLongPressedAndDraggedReleased)) {
                int16_t delta = (eventData.endX -eventData.startX)/swipeThreshold;
                if (delta > 1) {
                    return selectPrev();
                } else if (delta < 1) {
                    return selectNext();
                }
            }
            return false;
        });
    this->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
            if (isEvent(event, TouchEvent::TouchActionTapped) ||
                isEvent(event, TouchEvent::TouchActionLongPressed)) {
                return selectPrev();
            } else if (isEvent(event, TouchEvent::TouchActionDragReleased) || 
                    isEvent(event, TouchEvent::TouchActionLongPressedAndDraggedReleased)) {
                int16_t delta = (eventData.endX -eventData.startX)/swipeThreshold;
                if (delta > 1) {
                    return selectPrev();
                } else if (delta < 1) {
                    return selectNext();
                }
            }
            return false;
        });
}

SelectCanvas::~SelectCanvas() {
    
}

void SelectCanvas::setValueCanvas(Canvas * valueCanvas) {
    this->valueCanvas = valueCanvas;
    valueCanvas->onTouch([this](Canvas * canvas, TouchEvent event, TouchEventData eventData)->bool{
        if (isEvent(event, TouchEvent::TouchActionTapped) ||
                isEvent(event, TouchEvent::TouchActionLongPressed)) {
            if (eventData.startX < canvas->getX() + canvas->getWidth()/2) {
                return selectPrev();
            } else {
                return selectNext();
            }
            return true;
        }
        return false;
    });
}

void SelectCanvas::resetLayout() {
    Canvas::resetLayout();
    if (hAlign == ALIGN_RIGHT) {
        rightButtonCanvas->hAlignRight();
        valueCanvas->hAlignLeft(rightButtonCanvas);
        leftButtonCanvas->hAlignLeft(valueCanvas);
    } else {
        rightButtonCanvas->hAlignLeft();
        valueCanvas->hAlignRight(rightButtonCanvas);
        leftButtonCanvas->hAlignRight(valueCanvas);
    }
    if (leftButtonCanvas->isInvalid())
        this->invalidate();
}

bool SelectCanvas::selectNext() {
    if (rightDisable)
        return false;
    const char * value = nullptr;
    bool changed = stateChangeCallback(this, CHANGE_NEXT, value);
    if (value != nullptr) {
        setValue(value);
    }
    return changed;
}

bool SelectCanvas::selectPrev() {
    if (leftDisable)
        return false;
    const char * value = nullptr;
    bool changed = stateChangeCallback(this, CHANGE_PREV, value);
    if (value != nullptr) {
        setValue(value);
    }
    return changed;
}

void SelectCanvas::resize() {
    if (hAlign == ALIGN_RIGHT) {
        this->x = this->x + this->width - (leftButtonCanvas->getWidth() + rightButtonCanvas->getWidth() + valueCanvas->getWidth());
    }
    this->width = leftButtonCanvas->getWidth() + rightButtonCanvas->getWidth() + valueCanvas->getWidth();
}

void SelectCanvas::setValue(const char * value) {

}

bool SelectCanvas::draw() {
    if (hAlign == ALIGN_RIGHT) {
        tft->fillRect(getDrawX(), getDrawY(), leftButtonCanvas->getDrawX() - x, getDrawableHeight(), bgColor);
    } else {
        tft->fillRect(rightButtonCanvas->getDrawRightX(), getDrawY(), getDrawRightX() - rightButtonCanvas->getDrawX(), getDrawableHeight(), bgColor);
    }
    return true;
}