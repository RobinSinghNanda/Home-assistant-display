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
    this->selectedColor = PAGE_SELECTOR_SELECTED_COLOR;
    this->setHMargin(2*PAGE_SELECTOR_SELECTED_RAD);
    using namespace std::placeholders;
    this->onTouch([this](Canvas *, TouchEvent event, TouchEventData eventData)->bool {
            if (isEvent(event, TouchActionTapped)) {
                uint16_t nextPage = this->selectedPage + 1;
                if (nextPage == this->numPages) {
                    nextPage = 0;
                }
                this->selectedPage = nextPage;
                onValueChangeCallback(this, this->selectedPage, this->prevSelectedPage);
                this->prevSelectedPage = this->selectedPage;
                this->invalidate();
                return true;
            }
            return false;
        });
}

void PageSelectorCanvas::setSelectedColor(uint16_t color) {
    this->selectedColor = color;
}

uint16_t PageSelectorCanvas::getSelectedColor() {
    return this->selectedColor;
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
                this->getFgColor());
    }
    this->fillCircle(
            this->getDrawX() + (this->tempPosition+0.5)*pageSelectorWidth - PAGE_SELECTOR_RAD,
            this->getDrawY() + this->getDrawableHeight()/2 - PAGE_SELECTOR_RAD,
            this->selectedColor);
    return true;
}

void PageSelectorCanvas::fillCircle(uint16_t x, uint16_t y, uint16_t color) {
    for (uint16_t i=0; i<6; i++) {
        for (uint16_t j=0; j<4; j++) {
            uint8_t color8Bit;
            Color16Bit color16Bit;
            color8Bit = (*(cirleImage+i*4+j))&0xF0;
            color16Bit = tft->alphaBlend(color8Bit, color, bgColor);
            if (color16Bit != bgColor)
                tft->drawPixel(x+j*2, y+i, color16Bit);
            color8Bit = ((*(cirleImage+i*4+j))<<4)&0xF0;
            color16Bit = tft->alphaBlend(color8Bit, color, bgColor);
            if (color16Bit != bgColor)
                tft->drawPixel(x+j*2+1, y+i, color16Bit);
        }
    }
}

void PageSelectorCanvas::onValueChange(PageSelectorCanvasStateChangeCallback callback) {
    this->onValueChangeCallback = callback;
}