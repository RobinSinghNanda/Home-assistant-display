#include "ImageCanvas.hpp"
#include <rom/miniz.h>
#include "Log.hpp"

ImageCanvas::ImageCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {
    this->path = "";
    this->fgColor = DARK_THEME_ICON_COLOR;
    this->imageType = ImageTypeJpg;
    this->maskColor = 0xFFFF;
}

void ImageCanvas::setPath(String path) {
  if (path.indexOf(".jpg") >= 0) {
    invalidateIfNotEqual(this->path, path);
    this->imageType = ImageTypeJpg;
  } else if (path.indexOf(".bin") >= 0) {
    invalidateIfNotEqual(this->path, path);
    this->imageType = ImageTypeBin;
  } else {
    String s_icon = "/"+path;
    s_icon.replace(":","/");
    invalidateIfNotEqual(this->path, s_icon);
    this->imageType = ImageTypeBin;
  }
  this->invalidate();
}

void ImageCanvas::setMaskColor(uint16_t maskColor) {
  invalidateIfNotEqual(this->maskColor, maskColor);
}

uint16_t ImageCanvas::getMaskColor() {
  return this->maskColor;
}

void ImageCanvas::setScale(uint8_t scale) {
  if (scale != 0 && scale < 8) {
    invalidateIfNotEqual(this->scale, scale);
  }
}

uint8_t ImageCanvas::getScale() {
  return this->scale;
}

bool ImageCanvas::draw() {
  Canvas::draw();
  if (this->path == "/") {
    this->drawBackground();
    return false;
  }
  fs::File file = SPIFFS.open( this->path, "r");
 
  if ( !file ) {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "ERROR: File \"%s\" not found!", this->path);
    return 0;
  }
  if (this->imageType == ImageTypeBin) {
    this->renderBin();
    return 1;
  }
  boolean decoded = JpegDec.decodeFsFile(this->path);
  if (decoded) {
    this->renderJPEG();
    return 1;
  } else {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "Jpeg file decoding failed for \"%s\"", this->path);
  }
  return 1;
}

void ImageCanvas::renderBin() {
  #ifdef CANVAS_DEBUG
    this->drawDebugBorder();
  #endif
  int xpos = this->getDrawX();
  int ypos = this->getDrawY();

  fs::File file = SPIFFS.open( this->path, "r");
  if ( !file ) {
    Log::log(LOG_LEVEL_ERROR, D_LOG_LCD "ERROR: File \"%s\" not found!", this->path);
    return;
  }

  uint16_t width = 0;
  uint16_t height = 0;
  uint16_t format = 0;
  if (file.available()) {
    int num = file.read();
    if (num == -1) {
      file.close();
      return;
    }
    format = num;
    if (num >= 5 || num <= 0) {
      file.close();
      return;
    }
    width = num;
    num = file.read();
    if (num == -1) {
      file.close();
      return;
    }
    width = num;
    num = file.read();
    if (num == -1) {
      file.close();
      return;
    }
    height = num;
  }

  if (this->hAlign == ALIGN_LEFT) {
    xpos = this->getDrawX();
  } else if (this->hAlign == ALIGN_CENTER) {
    xpos = this->getDrawX() + (this->getDrawableWidth() - width)/2;
  } else if (this->hAlign == ALIGN_RIGHT) {
    xpos = this->getDrawX() + this->getDrawableWidth() - width;
  }

  if (this->vAlign == ALIGN_TOP) {
    ypos = this->getDrawY();
  } else if (this->vAlign == ALIGN_MIDDLE) {
    ypos = this->getDrawY() + (this->getDrawableHeight() - height)/2;
  } else if (this->vAlign == ALIGN_BOTTOM) {
    ypos = this->getDrawY() + this->getDrawableHeight() - height;
  }
  uint8_t pImg;
  tft->startWrite();

  tft->setAddrWindow(xpos, ypos, width, height);

  uint32_t num_pixels = width*height;
  if (format == 1) {
    while (file.available() && num_pixels > 0) {
      pImg = file.read();
      uint8_t color8Bit;
      color8Bit = (pImg<<4)&0xF0;
      tft->pushColor(tft->alphaBlend(color8Bit, this->fgColor, this->bgColor));
      color8Bit = pImg&0xF0;
      tft->pushColor(tft->alphaBlend(color8Bit, this->fgColor, this->bgColor));
      num_pixels--;
    }
  } else if (format == 2) {
    while (file.available() && num_pixels > 0) {
      pImg = file.read();
      tft->pushColor(tft->alphaBlend(pImg, this->fgColor, this->bgColor));
      num_pixels--;
    }
  } else if (format == 3) {
    while (file.available() && num_pixels > 0) {
      pImg = file.read();
      uint16_t red = (pImg+1)*fgColor.getRed();
      uint16_t green = (pImg+1)*fgColor.getGreen();
      uint16_t blue = (pImg+1)*fgColor.getBlue();
      pImg = file.read();
      red += (pImg+1)*bgColor.getRed();
      green += (pImg+1)*bgColor.getGreen();
      blue += (pImg+1)*bgColor.getBlue();
      red >>= 8;
      green >>= 8;
      blue >>= 8;
      tft->pushColor(Color16Bit(red, green, blue));
      num_pixels--;
    }
  } else if (format == 4) {
    while (file.available() && num_pixels > 0) {
      pImg = file.read();
      uint16_t red = (pImg+1)*fgColor.getRed();
      uint16_t green = (pImg+1)*fgColor.getGreen();
      uint16_t blue = (pImg+1)*fgColor.getBlue();
      pImg = file.read();
      red += (pImg+1)*bgColor.getRed();
      green += (pImg+1)*bgColor.getGreen();
      blue += (pImg+1)*bgColor.getBlue();
      pImg = file.read();
      red += (pImg+1)*maskColor.getRed();
      green += (pImg+1)*maskColor.getGreen();
      blue += (pImg+1)*maskColor.getBlue();
      red >>= 8;
      green >>= 8;
      blue >>= 8;
      tft->pushColor(Color16Bit(red, green, blue));
      num_pixels--;
    }
  } 
  
  tft->endWrite();
  return;
}

void ImageCanvas::renderJPEG() {
  #ifdef CANVAS_DEBUG
    this->drawDebugBorder();
  #endif
  int xpos = this->getDrawX();
  int ypos = this->getDrawY();
  uint16_t drawRightX = this->getDrawRightX();
  uint16_t drawBottomY = this->getDrawBottomY();

  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  if (this->hAlign == ALIGN_LEFT) {
    xpos = this->getDrawX();
  } else if (this->hAlign == ALIGN_CENTER) {
    xpos = this->getDrawX() + (this->getDrawableWidth() - max_x)/2;
  } else if (this->hAlign == ALIGN_RIGHT) {
    xpos = this->getDrawX() + this->getDrawableWidth() - max_x;
  }

  if (this->vAlign == ALIGN_TOP) {
    xpos = this->getDrawY();
  } else if (this->vAlign == ALIGN_MIDDLE) {
    ypos = this->getDrawY() + (this->getDrawableHeight() - max_y)/2;
  } else if (this->vAlign == ALIGN_BOTTOM) {
    ypos = this->getDrawY() + this->getDrawableHeight() - max_y;
  }

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while (JpegDec.read()) {
    
    // save a pointer to the image block
    pImg = JpegDec.pImage ;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    tft->startWrite();

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= drawRightX && ( mcu_y + win_h ) <= drawBottomY)
    {

      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft->setAddrWindow(mcu_x, mcu_y, win_w, win_h);

      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) {
        tft->pushColor(*pImg);
        pImg++;
      }

    }
    else if ( (mcu_y + win_h) >= drawBottomY) JpegDec.abort(); // Image has run off bottom of screen so abort decoding

    tft->endWrite();
  }
}

String ImageCanvas::getPath() {
    return this->path;
}