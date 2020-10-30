#include "ImageCanvas.hpp"

ImageCanvas::ImageCanvas(Canvas * canvas, uint16_t id) : Canvas(canvas, id) {
    this->path = "";
    this->fgColor = DARK_THEME_ICON_COLOR;
    this->imageType = ImageTypeJpg;
}

void ImageCanvas::setPath(String path) {
  if (path.indexOf(".jpg") >= 0) {
    invalidateIfNotEqual(this->path, path);
    this->path = path;
    this->imageType = ImageTypeJpg;
  } else {
    String s_icon = "/"+path+".bin";
    s_icon.replace(":","/");
    invalidateIfNotEqual(this->path, s_icon);
    this->path = s_icon;
    this->imageType = ImageTypeBin;
  }
  this->invalidate();
}


void ImageCanvas::setDarkMode(bool darkMode) {
  invalidateIfNotEqual(this->darkMode, darkMode);
  this->darkMode = darkMode;
  this->invalidate();
}

bool ImageCanvas::draw() {
  fs::File file = SPIFFS.open( this->path, "r");    // File handle reference for SPIFFS
  //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !file ) {
    Serial.print("ERROR: File \""); Serial.print(this->path); Serial.println ("\" not found!");
    return 0;
  }
  if (this->imageType == ImageTypeBin) {
    this->renderBin();
    return 1;
  }
  boolean decoded = JpegDec.decodeFsFile(this->path);  // or pass the filename (leading / distinguishes SPIFFS files)
                                   // Note: the filename can be a String or character array type
  if (decoded) {
    // print information about the image to the serial port
    //jpegInfo();

    // render the image onto the screen at given coordinates
    this->renderJPEG();
    return 1;
  }
  else {
    Serial.print("Jpeg file format not supported!");
    Serial.println(this->path);
  }
  return 1;
}



uint16_t ImageCanvas::alphaBlend(uint16_t fg, uint16_t bg, uint8 alpha) {

  // alpha for foreground multiplication
  // convert from 8bit to (6bit+1) with rounding
  // will be in [0..64] inclusive
  alpha = ( alpha + 2 ) >> 2;
  // "beta" for background multiplication; (6bit+1);
  // will be in [0..64] inclusive
  uint8 beta = MAX_ALPHA - alpha;
  // so (0..64)*alpha + (0..64)*beta always in 0..64

  return (uint16_t)((
            (  ( alpha * (uint32_t)( fg & MASK_RB )
                + beta * (uint32_t)( bg & MASK_RB )
            ) & MASK_MUL_RB )
          |
            (  ( alpha * ( fg & MASK_G )
                + beta * ( bg & MASK_G )
            ) & MASK_MUL_G )
         ) >> 6 );
}

void ImageCanvas::renderBin() {
  #ifdef CANVAS_DEBUG
    this->drawDebugBorder();
  #endif
  int xpos = this->getDrawX();
  int ypos = this->getDrawY();

  fs::File file = SPIFFS.open( this->path, "r");
  if ( !file ) {
    Serial.print("ERROR: File \""); Serial.print(this->path); Serial.println ("\" not found!");
    return;
  }

  uint16_t width = 0;
  uint16_t height = 0;

  if (file.available()) {
    int num = file.read();
    if (num == -1) {
      Serial.print("Failed to read the format");
      file.close();
      return;
    }
    if (num != 1) {
      Serial.print("Format not = 1");
      file.close();
      return;
    }
    width = num;
    num = file.read();
    if (num == -1) {
      Serial.print("Failed to read the width");
      file.close();
      return;
    }
    width = num;
    num = file.read();
    if (num == -1) {
      Serial.print("Failed to read the height");
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
  
  while (file.available() && num_pixels > 0) {
    pImg = file.read();
    uint8_t color8Bit;
    color8Bit = (pImg<<4)&0xF0;
    tft->pushColor(this->alphaBlend(this->fgColor, this->bgColor, color8Bit));
    color8Bit = pImg&0xF0;
    tft->pushColor(this->alphaBlend(this->fgColor, this->bgColor, color8Bit));
    num_pixels--;
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