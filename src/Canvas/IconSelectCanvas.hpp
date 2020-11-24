#ifndef __ICONSELECTCANVAS_H__
#define __ICONSELECTCANVAS_H__

#include "SelectCanvas.hpp"

class IconSelectCanvas : public SelectCanvas {
    public:
        IconSelectCanvas(Canvas * canvas, uint16_t id);
        ~IconSelectCanvas();
        virtual void setValue(const char * value);
    protected:
        IconCanvas * valueCanvas;
};

#endif // __ICONSELECTCANVAS_H__