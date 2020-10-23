#ifndef __CUSTOMSLIDERCANVAS_H__
#define __CUSTOMSLIDERCANVAS_H__

#include "SliderCanvas.hpp"

class CustomSliderCanvas;

typedef std::function<bool(CustomSliderCanvas*, TFT_eSPI*)> SliderDrawCallback;

class CustomSliderCanvas : public SliderCanvas {
    public:
        CustomSliderCanvas(Canvas * canvas, uint16_t id);
        void onDraw(SliderDrawCallback callback);
    protected:
        bool draw();
        SliderDrawCallback drawCallback;
        
};

#endif // __CUSTOMSLIDERCANVAS_H__