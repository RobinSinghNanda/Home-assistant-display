#include "SwipeCanvas.hpp"

using namespace std::placeholders;

SwipeCanvas::SwipeCanvas(CanvasRef canvas, uint16_t id) : Canvas(canvas, id) {
    this->swipeThreshold = 70;
    this->swipeValue = SwipeInvalid;
    onTouch(std::bind(&SwipeCanvas::onTouchEventCallback, this, _2, _3));
}

uint16_t SwipeCanvas::getSwipeThreshold() {
    return this->swipeThreshold;
}

void SwipeCanvas::setSwipeThreshold (uint16_t swipeThreshold) {
    this->swipeThreshold = swipeThreshold;
}

void SwipeCanvas::onSwipe(SwipeCanvasSwipeCallback callback) {
    this->onSwipeCallback = callback;
}

void SwipeCanvas::onSwipeComplete(SwipeCanvasSwipeCallback callback) {
    this->onSwipeCompleteCallback = callback;
}

bool SwipeCanvas::onTouchEventCallback(TouchEvent event, TouchEventData eventData) {
    if (isEventLosely(event, TouchActionDraged)) {
        SwipeEventData swipeEventData;
        double deltaX = (eventData.endX - eventData.startX);
        double deltaY = (eventData.endY - eventData.startY);
        SwipeDirection horizontalSwipe = SwipeInvalid;
        SwipeDirection verticalSwipe = SwipeInvalid;
        if (deltaX > this->swipeThreshold) {
            horizontalSwipe = SwipeRight;
        } else if (deltaX < -this->swipeThreshold) {
            horizontalSwipe = SwipeLeft;
        }
        if (deltaY > this->swipeThreshold) {
            verticalSwipe = SwipeDown;
        } else if (deltaY < -this->swipeThreshold) {
            verticalSwipe = SwipeUp;
        }
        this->swipeValue = (SwipeDirection) (horizontalSwipe | verticalSwipe);
        swipeEventData.startX = eventData.startX;
        swipeEventData.endX = eventData.endX;
        swipeEventData.startY = eventData.startY;
        swipeEventData.endY = eventData.endY;
        swipeEventData.swipeDirection = this->swipeValue;
        return onSwipeCallback(this, swipeEventData);
    } else if (isEventLosely(event, TouchActionDragReleased)) {
        SwipeEventData swipeEventData;
        swipeEventData.startX = eventData.startX;
        swipeEventData.endX = eventData.endY;
        swipeEventData.startY = eventData.startY;
        swipeEventData.endY = eventData.startY;
        swipeEventData.swipeDirection = this->swipeValue;
        bool swipeHandled = onSwipeCompleteCallback(this, swipeEventData);
        this->swipeValue = SwipeInvalid;
        return swipeHandled;
        return true;
    }
    return false;
}