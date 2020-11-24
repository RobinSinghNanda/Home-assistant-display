#ifndef __TEXTSELECTCANVAS_H__
#define __TEXTSELECTCANVAS_H__

#include "SelectCanvas.hpp"

class TextSelectCanvas : public SelectCanvas {
    public:
        TextSelectCanvas(Canvas * canvas, uint16_t id);
        ~TextSelectCanvas();
        virtual void setValue(const char * value);
        inline void setFont(const char * font);
        inline const char * getFont();
        virtual void resetLayout();
    protected:
        TextCanvas * valueCanvas;
};

void TextSelectCanvas::setFont(const char * font) {
    this->valueCanvas->setFont(font);
}

const char * TextSelectCanvas::getFont() {
    return this->valueCanvas->getFont();
}

#endif // __TEXTSELECTCANVAS_H__