// utility.h
// TFT_eSprite用のユーティリティ関数群

// スケールをかけてSpriteをコピーする
//void pushSpriteScaled(TFT_eSprite* spr_, TFT_eSPI* tft_, int32_t x, int32_t y, int32_t w, int32_t h)
void pushSpriteScaled(TFT_eSprite* spr_, TFT_eSPI* tft_, const int32_t x, const int32_t y, const int32_t w, const int32_t h)
{
  // 溢れ対応
  /*
  int32_t sx,sy;
  sx = 0; sy = 0;
  if (x<0) {
    sx=0; w+=x; x=0;
    if (w<=0) return;
  }
  if (y<0) {
    sy=0; h+=y; y=0;
    if (h<=0) return;
  }
  int32_t tw = tft_->width()/2;
  int32_t th = tft_->height()/2;
  if (x>tw) return;
  if (y>th) return;
  if (x+w>tw)
  {
    int32_t sub = tw-x-w;
    w -= sub;
    sx = sub;
    if (w<=0) return;
  }
  if (y+h>th)
  {
    int32_t sub = th-y-h;
    h -= sub;
    sy = sub;
    if (h<=0) return;
  }*/

  uint16_t line_buffer[w * 2];
  for (uint32_t i=0; i<h; i++)
  {
    for (uint32_t j=0; j<w; j++)
    {
      //uint16_t rp = spr_->readPixel(j+x+sx, i+y+sy);
      uint16_t rp = spr_->readPixel(j+x, i+y);
      rp = (rp>>8 | rp<<8);
      line_buffer[j*2] = line_buffer[j*2+1] = rp;
    }
    #if true
    tft_->pushImage(x*2, (y+i)*2, w*2, 1, line_buffer);
    tft_->pushImage(x*2, (y+i)*2+1, w*2, 1, line_buffer);
    #else
    tft_->setWindow(x*2, (y+i)*2, x*2+w*2-1, (y+i)*2);
    tft_->pushPixels(line_buffer, w*2);
    tft_->setWindow(x*2, (y+i)*2+1, x*2+w*2-1, (y+i)*2+1);
    tft_->pushPixels(line_buffer, w*2);
    #endif
  }
}

// 透明色を考慮してSpriteをコピーする
void pushToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_)
{
  #if false
  for (uint32_t i=0; i<src_->height(); i++)
  {
    for (uint32_t j=0; j<src_->width(); j++)
    {
      uint16_t rp = src_->readPixelValue(j, i);
      if (transp_ == rp)
      {
        rp = dst_->readPixelValue(x+j, y+i);
      }
      dst_->drawPixel(x+j, y+i, rp);
    }
  }
  #else
  uint8_t line_buffer[src_->width()/2];
  for (uint32_t i=0; i<src_->height(); i++)
  {
    bool flip = false;
    uint8_t p;
    for (uint32_t j=0; j<src_->width(); j++)
    {
      uint8_t rp = src_->readPixelValue(j, i);
      if (transp_ == rp)
      {
        rp = dst_->readPixelValue(x+j, y+i);
      }
      if (flip)
      {
        line_buffer[j/2] = rp | (p<<4);
      }
      p = rp;
      flip = !flip;
    }
    //dst_->setWindow(x, y+i, x+src_->width()-1, y+i);
    //dst_->pushPixels(line_buffer, src_->width()/2);
    dst_->pushImage(x, y+i, src_->width(), 1, (uint16_t*)line_buffer);
  }
  #endif
}

// 4bitから16bitへSpriteをコピーする
void pushSprite4ToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_)
{
  for (uint32_t i=0; i<src_->height(); i++)
  {
    for (uint32_t j=0; j<src_->width(); j++)
    {
      //uint8_t rp = src_->readPixelValue(j, i);
      //uint16_t c = src_->getPaletteColor(rp);
      uint16_t rp = src_->readPixel(j, i);
      //rp = rp>>8 | rp<<8;
      if (src_->getPaletteColor(transp_) == rp)
      {
        rp = TFT_TRANSPARENT;
      }
      dst_->drawPixel(x+j, y+i, rp);
    }
  }
}
