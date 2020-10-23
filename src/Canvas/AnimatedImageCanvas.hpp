#ifndef __ANIMATEDIMAGECANVAS_H__
#define __ANIMATEDIMAGECANVAS_H__

#include "ImageCanvas.hpp"

class AnimatedImageCanvas : public ImageCanvas {
    public:
        AnimatedImageCanvas(Canvas * canvas, uint16_t id);
        virtual ~AnimatedImageCanvas();
        void setTimeDelay(uint32_t delayMS);
        uint32_t getTimeDelay();
        void addImage(String image);
        void clearImages();
        void start();
        void stop();
        String getNextImage();
    protected:
        uint32_t imageIndex = 0;
        uint32_t timeDelay;
        std::vector<String> images;
        TimerHandle_t animatationTimer;
        void animationTask(TimerHandle_t xTimer);
};

#endif // __ANIMATEDIMAGECANVAS_H__