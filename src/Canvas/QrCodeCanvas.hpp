#ifndef __QRCODECANVAS_H__
#define __QRCODECANVAS_H__

#include "Canvas.hpp"

#define QR_CODE_SIZE 33

class QrCodeCanvas : public Canvas {
    public:
        QrCodeCanvas(Canvas * canvas, uint16_t id);
        void setPath(String path);
        String getPath();
    protected:
        String path;
        virtual bool draw();
};

#endif // __QRCODECANVAS_H__