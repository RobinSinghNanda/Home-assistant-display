#include "AnimatedImageCanvas.hpp"

using namespace std::placeholders;

void timeCallback (TimerHandle_t animatationTimer);

AnimatedImageCanvas::AnimatedImageCanvas(Canvas * canvas, uint16_t id) : ImageCanvas(canvas, id){
    this->imageIndex = 0;
    this->timeDelay = 250;
    animatationTimer = xTimerCreate( "Animation timer",
                     this->timeDelay/portTICK_PERIOD_MS,
                     pdTRUE,
                     this,
                     timeCallback
                   );
}

AnimatedImageCanvas::~AnimatedImageCanvas(){
    if(xSemaphoreTake(tftMutex, portMAX_DELAY) == pdTRUE) {       
        if (xTimerIsTimerActive( animatationTimer )) {
            Serial.println("Timer is active");
            xTimerDelete(animatationTimer, 1000);
        } else {
            xTimerDelete(animatationTimer, 1000);
        }
        xSemaphoreGive(tftMutex);
    } else {
        Serial.println("Could not obtain the tft Mutex");
    }
}

void AnimatedImageCanvas::setTimeDelay(uint32_t delayMS) {
    if (xTimerIsTimerActive( animatationTimer ) != pdFALSE) {
        if (xTimerStop(animatationTimer, 1000) == pdPASS ) {
            if( xTimerChangePeriod( animatationTimer, delayMS / portTICK_PERIOD_MS, 1000 ) == pdPASS){
                this->timeDelay = delayMS;
                this->invalidate();
            }
        }
    } else {
        if( xTimerChangePeriod( animatationTimer, delayMS / portTICK_PERIOD_MS, 1000 ) == pdPASS){
            this->timeDelay = delayMS;
            this->invalidate();
        }
    }
}

uint32_t AnimatedImageCanvas::getTimeDelay() {
    return this->timeDelay;
}

void AnimatedImageCanvas::addImage(String image) {
    auto it = images.begin();
    this->images.insert(it, image);
}

void AnimatedImageCanvas::clearImages() {
    this->images.clear();
}

void AnimatedImageCanvas::start() {
    if (xTimerIsTimerActive( animatationTimer ) == pdFALSE) {
        if(xSemaphoreTake(tftMutex, portMAX_DELAY) == pdTRUE) {
            Serial.println("Animationt timer started");
            xTimerStart(animatationTimer, 1000);
            xSemaphoreGive(tftMutex);
        } else {
            Serial.println("Could not obtain the tft Mutex");
        }
    }
}

void AnimatedImageCanvas::stop() {
    if (xTimerIsTimerActive( animatationTimer ) != pdFALSE) {
        if(xSemaphoreTake(tftMutex, portMAX_DELAY) == pdTRUE) {
            Serial.println("Animation timer stopped");
            xTimerStop(animatationTimer, 1000);
            xSemaphoreGive(tftMutex);
        } else {
            Serial.println("Could not obtain the tft Mutex");
        }
    }
}

String AnimatedImageCanvas::getNextImage() {
    this->imageIndex++;
    if (imageIndex >= images.size()) {
        imageIndex = 0;
    }
    if (images.size() != 0) {
        return this->images[imageIndex];
    }
    return "";
}

void timeCallback (TimerHandle_t animatationTimer) {  
    AnimatedImageCanvas * canvas = static_cast<AnimatedImageCanvas *>(pvTimerGetTimerID(animatationTimer));
    String image = canvas->getNextImage();
    if (image != "") {
        canvas->setPath(image);
        canvas->invalidate();
        canvas->redraw();
    }
}