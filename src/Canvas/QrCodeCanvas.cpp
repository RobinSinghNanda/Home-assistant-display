#include "QrCodeCanvas.hpp"

QrCodeCanvas::QrCodeCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {

}

void QrCodeCanvas::setPath(String path) {
    invalidateIfNotEqual(this->path, path);
}

String QrCodeCanvas::getPath() {
    return this->path;
}

bool QrCodeCanvas::draw() {
    String qrPath = "/qr/"+this->path+".bin";
    fs::File file = SPIFFS.open( qrPath, "r");
    tft->fillRect(this->x, this->y, this->width, this->height, TFT_WHITE);
    if ( !file ) {
        Serial.print("ERROR: File \""); Serial.print(qrPath); Serial.println ("\" not found!");
        return false;
    }
    uint16_t xPos=0, yPos=0;
    uint16_t qrStepWidth = (this->getDrawableWidth()/QR_CODE_SIZE);
    uint16_t qrStepHeight = (this->getDrawableHeight()/QR_CODE_SIZE);
    uint16_t qrStep = minimum(qrStepWidth, qrStepHeight);
    uint16_t qrSize = qrStep * QR_CODE_SIZE;
    if (this->hAlign == ALIGN_LEFT) {
        xPos = this->getDrawX();
    } else if (this->hAlign == ALIGN_CENTER) {
        xPos = this->getDrawX() + (this->getDrawableWidth() - qrSize)/2;
    } else if (this->hAlign == ALIGN_RIGHT) {
        xPos = this->getDrawX() + this->getDrawableWidth() - qrSize;
    }
    if (this->vAlign == ALIGN_TOP) {
        yPos = this->getDrawY();
    } else if (this->vAlign == ALIGN_MIDDLE) {
        yPos = this->getDrawY() + (this->getDrawableHeight() - qrSize)/2;
    } else if (this->vAlign == ALIGN_BOTTOM) {
        yPos = this->getDrawY() + this->getDrawableHeight() - qrSize;
    }
    tft->startWrite();
    tft->setAddrWindow(xPos, yPos, qrSize, qrSize);

    uint32_t num_pixels = 5*33;
    uint32_t i=0,j=0;
    uint32_t stepCounter = 0;
    uint8_t * pixelsData = new uint8_t[5];
    uint32_t num_pixels_pushed = 0;
    while (file.available() && num_pixels > 0) {
        if (stepCounter == 0) {
            pixelsData[i] = file.read();
            num_pixels--;
        }
        for (int8_t k=7;k>=0;k--) {
            bool pixel = (((pixelsData[i])&(1<<k)) != 0);
            if ((i==4) && (k<=6)) {
                break;
            }
            for (uint8_t l=0;l<qrStep;l++) {
                tft->pushColor(pixel?TFT_WHITE:TFT_BLACK);
                num_pixels_pushed++;
            }
        }
        i++;
        if (i == 5) {
            j++;
            i = 0;
            stepCounter++;
            if (stepCounter == qrStep) {
                stepCounter = 0;
            }
        }
    }
    tft->endWrite();
    return true;
}