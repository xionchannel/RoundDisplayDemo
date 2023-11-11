// utility.cpp
// TFT_eSprite用のユーティリティ関数群

#include "utility.h"

// カラーをr,g,bにして返す
void Utility::splitColor(const uint16_t c, uint16_t* r, uint16_t* g, uint16_t* b)
{
  *r = (c >> 8) & 0xF8; *r |= (*r >> 5);
  *g = (c >> 3) & 0xFC; *g |= (*g >> 6);
  *b = (c << 3) & 0xF8; *b |= (*b >> 5);
}

// カラーをスケーリングする
uint16_t Utility::scaleColor(const uint16_t color, const float scale)
{
  uint16_t r,g,b;
  splitColor(color, &r, &g, &b);
  r *= scale;
  g *= scale;
  b *= scale;
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// カラーを加算する
uint16_t Utility::addColor(const uint16_t color_a, const uint16_t color_b)
{
  uint16_t ar, ag, ab, br, bg, bb;
  splitColor(color_a, &ar, &ag, &ab);
  splitColor(color_b, &br, &bg, &bb);
  ar += br; if (ar>0xF8) ar=0xF8;
  ag += bg; if (ag>0xFC) ag=0xFC;
  ab += bb; if (ab>0xF8) ab=0xF8;
  return ((ar & 0xF8) << 8) | ((ag & 0xFC) << 3) | (ab >> 3);
}

// スケールをかけてSpriteをコピーする
//void pushSpriteScaled(TFT_eSprite* spr_, TFT_eSPI* tft_, int32_t x, int32_t y, int32_t w, int32_t h)
void Utility::pushSpriteScaled(TFT_eSprite* spr_, TFT_eSPI* tft_, const int32_t x, const int32_t y, const int32_t w, const int32_t h)
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

  uint16_t size = w*2;
  uint16_t line_buffer[size];
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
    tft_->pushImage(x*2, (y+i)*2, size, 1, line_buffer);
    tft_->pushImage(x*2, (y+i)*2+1, size, 1, line_buffer);
    #else
    tft_->setWindow(x*2, (y+i)*2, x*2+size-1, (y+i)*2);
    tft_->pushPixels(line_buffer, size);
    tft_->setWindow(x*2, (y+i)*2+1, x*2+size-1, (y+i)*2+1);
    tft_->pushPixels(line_buffer, size);
    #endif
  }
}

// 円形コピー用のラインバッファサイズを計算して確保して返す
CopyBuffer** Utility::calcCircleBuffers(const int32_t center_, const int32_t range_)
{
  CopyBuffer** b = new CopyBuffer*[range_];
  for (uint32_t i=0; i<range_; i++)
  {
    b[0] = new CopyBuffer();
    // 円の交差をやって x, width を記録する
  }
  return &b[0];
}

// バッファ配列をもらってそれに従って２倍にコピーする
void Utility::pushSpriteScaledBuffers(TFT_eSprite* spr_, TFT_eSPI* tft_, CopyBuffer** buff_, const int32_t count_)
{
  for (uint32_t i=0; i<count_; i++)
  {
    CopyBuffer* b = buff_[i];
    uint16_t size = b->width * 2;
    if (size <= 0) continue;
    uint16_t line_buffer[size];
    for (uint32_t j=0; j<b->width; j++)
    {
      uint16_t rp = spr_->readPixel(j+b->x, i);
      rp = (rp>>8 | rp<<8);
      line_buffer[j*2] = line_buffer[j*2+1] = rp;
    }
    #if true
    tft_->pushImage(b->x*2, i*2, size, 1, line_buffer);
    tft_->pushImage(b->x*2, i*2+1, size, 1, line_buffer);
    #else
    tft_->setWindow(b->x*2, i*2, b->x*2+size-1, i*2);
    tft_->pushPixels(line_buffer, size);
    tft_->setWindow(b->x*2, i*2+1, b->x*2+size-1, i*2+1);
    tft_->pushPixels(line_buffer, size);
    #endif
  }
}

// 透明色を考慮してSpriteをコピーする
void Utility::pushToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_)
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
void Utility::pushSprite4ToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_)
{
  for (uint32_t i=0; i<src_->height(); i++)
  {
    for (uint32_t j=0; j<src_->width(); j++)
    {
      uint16_t rp = src_->readPixel(j, i);
      if (src_->getPaletteColor(transp_) == rp)
      {
        rp = TFT_TRANSPARENT;
      }
      dst_->drawPixel(x+j, y+i, rp);
    }
  }
}

// 4bitから16bitへSpriteをコピーする
void Utility::pushSprite4ToSpriteClip(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t xst, const int32_t yst, const int32_t xed, const int32_t yed, const int32_t x, const int32_t y, const uint8_t transp_)
{
  for (uint32_t i=0; i<yed-yst+1; i++)
  {
    for (uint32_t j=0; j<xed-xst+1; j++)
    {
      uint16_t rp = src_->readPixel(j+xst, i+yst);
      if (src_->getPaletteColor(transp_) == rp)
      {
        rp = TFT_TRANSPARENT;
      }
      dst_->drawPixel(x+j, y+i, rp);
    }
  }
}

// 4bitから16bitへSpriteをコピーする
void Utility::pushSprite4ToSpriteMasked(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_)
{
  for (uint32_t i=0; i<src_->height(); i++)
  {
    for (uint32_t j=0; j<src_->width(); j++)
    {
      uint16_t rp = src_->readPixel(j, i);
      if (src_->getPaletteColor(transp_) == rp)
      {
        continue;
      }
      dst_->drawPixel(x+j, y+i, rp);
    }
  }
}

// 4bitから16bitへSpriteをブレンドする
void Utility::pushSprite4ToSpriteBlended(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint16_t color_, const float alpha_)
{
  for (uint32_t i=0; i<src_->height(); i++)
  {
    for (uint32_t j=0; j<src_->width(); j++)
    {
      uint16_t rps = src_->readPixel(j, i);
      uint8_t a = (rps >> 8) & 0xF8; a |= (a >> 5);
      if (a == 0) continue;
      float af = alpha_ * (((float)a)/255.0);
      uint16_t rpd = dst_->readPixel(x+j, y+i);
      //dst_->drawPixel(x+j, y+i, src_->alphaBlend(af * 255, color_, rpd));
      dst_->drawPixel(x+j, y+i, addColor(scaleColor(color_, af), rpd));
    }
  }
}

// 4bitから16bitへSpriteをコピーする(透明色なし)
void Utility::pushSprite4ToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y)
{
  for (uint32_t i=0; i<src_->height(); i++)
  {
    for (uint32_t j=0; j<src_->width(); j++)
    {
      uint16_t rp = src_->readPixel(j, i);
      dst_->drawPixel(x+j, y+i, rp);
    }
  }
}

// パレットカラーに乗算する
void Utility::createPaletteMultiply(TFT_eSprite* target_, const float r_, const float g_, const float b_)
{
  for (uint8_t i=0; i<16; i++)
  {
    uint32_t color888;
    uint16_t color565 = target_->getPaletteColor(i);
    {
      uint8_t r = (color565 >> 8) & 0xF8; r |= (r >> 5);
      uint8_t g = (color565 >> 3) & 0xFC; g |= (g >> 6);
      uint8_t b = (color565 << 3) & 0xF8; b |= (b >> 5);
      r *= r_;
      g *= g_;
      b *= b_;
      color888 = ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b << 0);
    }
    {
      uint16_t r = (color888 >> 8) & 0xF800;
      uint16_t g = (color888 >> 5) & 0x07E0;
      uint16_t b = (color888 >> 3) & 0x001F;
      color565 = (r | g | b);
    }
    target_->setPaletteColor(i, color565);
  }
}

