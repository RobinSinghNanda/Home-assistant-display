#include "PageSelectorCanvas.hpp"

typedef std::function<void(PageSelectorCanvas*, int16_t, int16_t)> PageSelectorCanvasValueChangeCallback;

const uint8_t cirleImage[] {
    0x00, 0x8e,	0xe8, 0x00,
    0x08, 0xff,	0xff, 0x80,
    0x0e, 0xff,	0xff, 0xe0,
    0x0e, 0xff,	0xff, 0xe0,
    0x08, 0xff,	0xff, 0x80,
    0x00, 0x8e,	0xe8, 0x00,
  };

PageSelectorCanvas::PageSelectorCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id){
    this->numPages = 2;
    this->selectedPage = 0;
    this->dragScale = 0.5;
    this->tempPosition = 0;
    this->prevSelectedPage = this->selectedPage;
    this->limitStep = 0;
    this->setHMargin(2*PAGE_SELECTOR_SELECTED_RAD);
    using namespace std::placeholders;
    onTouch(std::bind(&PageSelectorCanvas::onTouchEventCallback, this, _2, _3));
}

bool PageSelectorCanvas::onTouchEventCallback (TouchEvent event, TouchEventData eventData) {
    if (event & TouchActionTapped) {
        uint16_t nextPage = this->selectedPage + 1;
        if (nextPage == this->numPages) {
            nextPage = 0;
        }
        this->tempPosition = nextPage;
        this->selectedPage = nextPage;
        this->draw();
        if (this->prevSelectedPage != this->selectedPage) {
            onValueChangeCallback(this, this->selectedPage, this->prevSelectedPage);
            this->prevSelectedPage = this->selectedPage;
        }
    } else if (event & TouchActionDraged) {
        uint16_t pageSelectorWidth = this->getDrawableWidth()/this->numPages;
        double deltaX = (eventData.endX - eventData.startX)*this->dragScale;
        if (this->limitStep) {
            if (deltaX > pageSelectorWidth) {
                deltaX = pageSelectorWidth;
            } else if (deltaX < -pageSelectorWidth) {
                deltaX = -pageSelectorWidth;
            }
        }
        this->tempPosition = (double)this->selectedPage + (double)deltaX/(double)pageSelectorWidth;
        if (tempPosition < 0) {
            tempPosition = 0;
        } else if (tempPosition > this->numPages-1) {
            tempPosition = this->numPages-1;
        }
        this->draw();
        return true;
    } else if (event & TouchActionDragReleased) {
        int16_t intValue = tempPosition;
        if (tempPosition - intValue > 0.5) {
            tempPosition = intValue + 1;
        } else {
            tempPosition = intValue;
        }
        this->selectedPage = tempPosition;
        this->draw();
        if (this->prevSelectedPage != this->selectedPage) {
            onValueChangeCallback(this, this->selectedPage, this->prevSelectedPage);
            this->prevSelectedPage = this->selectedPage;
        }
        return true;
    }
    return false;
}

void PageSelectorCanvas::setSelected(uint16_t selectedPage) {
    if (this->selectedPage != selectedPage) {
        this->selectedPage = selectedPage;
        this->prevSelectedPage = this->selectedPage;
        this->tempPosition = this->selectedPage;
        this->invalidate();
    }
}

uint16_t PageSelectorCanvas::getSelected() {
    return this->selectedPage;
}

void PageSelectorCanvas::setNumPages(uint16_t numPages) {
    if (this->numPages != numPages) {
        this->numPages = numPages;
        if (this->selectedPage > numPages-1) {
            this->setSelected(numPages-1);
        }
        this->invalidate();
    }
}

uint16_t PageSelectorCanvas::getNumPages() {
    return this->numPages;
}

void PageSelectorCanvas::setLimitStep(bool limitStep) {
    this->limitStep = limitStep;
}

bool PageSelectorCanvas::getLimitStep() {
    return this->limitStep;
}

void PageSelectorCanvas::setTempPosition(double tempPosition) {
    if (this->tempPosition != tempPosition) {
        if (tempPosition>-0.5 && tempPosition<(this->numPages-0.5)) {
            this->tempPosition = tempPosition;
            this->invalidate();
        }
    }
}

double PageSelectorCanvas::getTempPosition() {
    return this->tempPosition;
}

bool PageSelectorCanvas::draw() {
    uint16_t pageSelectorWidth = this->getDrawableWidth()/this->numPages;
    tft->fillRect(this->x, this->y, this->width, this->height, this->bgColor);
    for (uint16_t i=0;i<this->numPages;i++) {
        this->fillCircle(
                this->getDrawX() + i*pageSelectorWidth + pageSelectorWidth/2 - PAGE_SELECTOR_RAD,
                this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
                this->getFgColor(),
                (this->getFgColor() == TFT_BLACK)
            );
    }
    // uint16_t tempPositionInt = tempPosition;
    // double remainder = tempPosition - tempPositionInt;
    // if (remainder < 0.5) {
    //     this->fillCircle(
    //             this->getDrawX() +(tempPositionInt+0.5)*(pageSelectorWidth) - PAGE_SELECTOR_SELECTED_RAD,
    //             this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
    //             PAGE_SELECTOR_SELECTED_COLOR,
    //             (this->getFgColor() == TFT_BLACK)
    //         );
    //     this->fillCircle(
    //             this->getDrawX() +(tempPositionInt+0.5+2*remainder)*(pageSelectorWidth) - PAGE_SELECTOR_SELECTED_RAD,
    //             this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
    //             PAGE_SELECTOR_SELECTED_COLOR,
    //             (this->getFgColor() == TFT_BLACK)
    //         );
    //     tft->fillRect(
    //             this->getDrawX() +(tempPositionInt+0.5)*(pageSelectorWidth),
    //             this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
    //             (2*remainder)*(pageSelectorWidth)+1,
    //             PAGE_SELECTOR_RAD*2,
    //             PAGE_SELECTOR_SELECTED_COLOR
    //         );
    // } else {
    //     this->fillCircle(
    //             this->getDrawX() +(tempPositionInt-0.5+remainder*2)*(pageSelectorWidth) - PAGE_SELECTOR_SELECTED_RAD,
    //             this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
    //             PAGE_SELECTOR_SELECTED_COLOR,
    //             (this->getFgColor() == TFT_BLACK)
    //         );
    //     this->fillCircle(
    //             this->getDrawX() +(tempPositionInt+1.5)*(pageSelectorWidth) - PAGE_SELECTOR_SELECTED_RAD,
    //             this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
    //             PAGE_SELECTOR_SELECTED_COLOR,
    //             (this->getFgColor() == TFT_BLACK)
    //         );
    //     tft->fillRect(
    //             this->getDrawX() +(tempPositionInt-0.5+remainder*2)*(pageSelectorWidth),
    //             this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
    //             (2*(1-remainder))*(pageSelectorWidth)+1,
    //             PAGE_SELECTOR_RAD*2,
    //             PAGE_SELECTOR_SELECTED_COLOR
    //         );
    // }
    this->fillCircle(
            this->getDrawX() + (this->tempPosition+0.5)*pageSelectorWidth - PAGE_SELECTOR_RAD,
            this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
            PAGE_SELECTOR_SELECTED_COLOR,
            (this->getFgColor() == TFT_BLACK)
        );
    return true;
}

void PageSelectorCanvas::fillCircle(uint16_t x, uint16_t y, uint16_t colorMask, bool invert) {
    //tft->startWrite();
    //tft->setAddrWindow(x, y, 8, 6);
    for (uint16_t i=0; i<6; i++) {
        for (uint16_t j=0; j<4; j++) {
            uint8_t color8Bit;
            uint16_t color16Bit;
            color8Bit = (*(cirleImage+i*4+j))&0xF0;
            color16Bit = ((color8Bit & 0b11111000)<<8) + ((color8Bit & 0b11111100)<<3)+(color8Bit>>3);
            color16Bit = (invert)?((~color16Bit)|colorMask):(color16Bit&colorMask);
            //tft->pushColor(color16Bit);
            if (((invert)&&(color16Bit != TFT_WHITE))||((!invert)&&(color16Bit != TFT_BLACK)))
                tft->drawPixel(x+j*2, y+i, color16Bit);
            color8Bit = ((*(cirleImage+i*4+j))<<4)&0xF0;
            color16Bit = ((color8Bit & 0b11111000)<<8) + ((color8Bit & 0b11111100)<<3)+(color8Bit>>3);
            color16Bit = (invert)?((~color16Bit)|colorMask):(color16Bit&colorMask);
            //tft->pushColor(color16Bit);
            if (((invert)&&(color16Bit != TFT_WHITE))||((!invert)&&(color16Bit != TFT_BLACK)))
                tft->drawPixel(x+j*2+1, y+i, color16Bit);
        }
    }
    //tft->endWrite();
}

void PageSelectorCanvas::onValueChange(PageSelectorCanvasStateChangeCallback callback) {
    this->onValueChangeCallback = callback;
}