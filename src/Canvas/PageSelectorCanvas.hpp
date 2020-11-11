#ifndef PAGE_SELECTOR_CANVAS_H_
#define PAGE_SELECTOR_CANVAS_H_

#include "Canvas.hpp"


#define PAGE_SELECTOR_SELECTED_RAD  3
#define PAGE_SELECTOR_RAD 3
#define PAGE_SELECTOR_SELECTED_COLOR convert2rgb565(0x03a9f4)




class PageSelectorCanvas;

typedef std::function<void(PageSelectorCanvas*, uint16_t, uint16_t)> PageSelectorCanvasStateChangeCallback;

class PageSelectorCanvas : public Canvas {
    public:
      PageSelectorCanvas(Canvas * canvas, uint16_t id);
      void setSelected(uint16_t selectedPage);
      uint16_t getSelected();
      void setNumPages(uint16_t numPages);
      uint16_t getNumPages();
      void onValueChange(PageSelectorCanvasStateChangeCallback callback);
      void setLimitStep(bool limitStep);
      bool getLimitStep();
      void setTempPosition(double tempPosition);
      double getTempPosition();
      void setSelectedColor(uint16_t color);
      uint16_t getSelectedColor();
    protected:
      uint16_t selectedColor;
      double tempPosition;
      uint16_t numPages;
      uint16_t selectedPage;
      uint16_t prevSelectedPage;
      double dragScale=1.0;
      bool limitStep = 0;
      virtual bool draw();
      PageSelectorCanvasStateChangeCallback onValueChangeCallback;
      bool onTouchEventCallback (TouchEvent event, TouchEventData eventData);
      void fillCircle (uint16_t x, uint16_t y, uint16_t color);
};

#endif