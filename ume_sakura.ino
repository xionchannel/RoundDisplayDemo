
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "ume.h"

#define SCREEN_WIDTH 244
#define SCREEN_HEIGHT 244

// Width and height of sprite
#define SP_WIDTH  32
#define SP_HEIGHT 32

TFT_eSPI    tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite spr = TFT_eSprite(&tft);  // Create Sprite object "spr" with pointer to "tft" object
TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer
// the pointer is used by pushSprite() to push it onto the TFT

#define BGCOLOR 4
#define MOVESPEED 3

#define COUNT 20
int32_t x[COUNT],y[COUNT];
int32_t xx[COUNT],yy[COUNT];
int32_t cpx[COUNT],cpy[COUNT];
int32_t spx[COUNT],spy[COUNT];
int32_t skipCol[COUNT];
int32_t xmin, ymin, xmax, ymax;
//uint16_t* tft_buffer; // backbuffer of sprite
//bool buffer_loaded = false;

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  bg.setColorDepth(4);
  bg.createSprite(tft.width()/2, tft.height()/2);
  bg.createPalette(ume_palette);
  bg.fillRect(0, 0, bg.width(), bg.height(), BGCOLOR);
  //bg.pushImage(32, 32, SP_WIDTH, SP_HEIGHT, (uint16_t *)ume);

  // Create a sprite of defined size
  spr.setColorDepth(4);
  spr.createSprite(SP_WIDTH, SP_HEIGHT);
  spr.createPalette(ume_palette);
  spr.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t *)ume);

  for (uint8_t i=0; i<COUNT; i++)
  {
    x[i] = random(tft.width()/2 - SP_WIDTH);
    //y[i] = random(tft.height()/2 - SP_HEIGHT);
    y[i] = -SP_HEIGHT;
    //getRandomPos(i, &x[i], &y[i]);
    spx[i] = random(MOVESPEED) + 1;
    spy[i] = (random(MOVESPEED) + 1) * 2;
    xx[i] = (random(2) ? 1 : -1);
    //yy[i] = (random(2) ? 1 : -1);
    yy[i] = 1;
    skipCol[i] = 0;
  }
  xmin = 0-SP_WIDTH;
  ymin = 0-SP_HEIGHT;
  xmax = tft.width()/2;// - SP_WIDTH;
  ymax = tft.height()/2;// - SP_HEIGHT;

  delay(500);

  //tft.setViewport(SP_WIDTH*2, SP_HEIGHT*2, SCREEN_WIDTH, SCREEN_HEIGHT);

  //tft_buffer = (uint16_t*) malloc( SP_WIDTH * SP_HEIGHT * 2 );
  //bg.readRect(x, y, SP_WIDTH, SP_HEIGHT, tft_buffer);
  tft.startWrite();
  for (uint8_t i=0; i<COUNT; i++)
  {
    spr.pushToSprite(&bg, x[i], y[i], BGCOLOR);
  }
  pushSpriteScaled(&bg, &tft, 0, 0, tft.width()/2, tft.height()/2);
  tft.endWrite();
}

void loop() {
  tft.startWrite();
  uint8_t i;
  #if false
  for (i=0; i<COUNT; i++)
  {
    bg.fillRect(x[i], y[i], SP_WIDTH, SP_HEIGHT, BGCOLOR);
  }
  #else
  bg.fillRect(0, 0, bg.width(), bg.height(), BGCOLOR);
  #endif
  for (i=0; i<COUNT; i++)
  {
    //bg.fillRect(x[i], y[i], SP_WIDTH, SP_HEIGHT, BGCOLOR);
    int32_t px = x[i];
    int32_t py = y[i];

    bool warp = false;
    {
      x[i] += xx[i] * spx[i];
      y[i] += yy[i] * spy[i];
      if (x[i] < xmin)
      {
        //x[i] = xmin;
        //xx[i] *= -1;
        x[i] = xmax;
        warp = true;
      }
      else if (x[i] > xmax)
      {
        x[i] = xmin;
        //xx[i] *= -1;
        warp = true;
      }
      if (y[i] < ymin)
      {
        y[i] = ymin;
        yy[i] *= -1;
      }
      else if (y[i] > ymax)
      {
        y[i] = ymin;
        x[i] = random(tft.width()/2 - SP_WIDTH);
        spx[i] = random(MOVESPEED) + 1;
        spy[i] = (random(MOVESPEED) + 1) * 2;
        //yy[i] *= -1;
        warp = true;
      }

      /*if (skipCol[i] == 0)
      {
        if (checkCollide(x[i], y[i], COUNT, i))
        {
          //if (random(2))
            xx[i] *= -1;
          //else
            yy[i] *= -1;
          skipCol[i] = 5;
        }
      }
      else
      {
        skipCol[i]--;
      }*/
    }
    cpx[i] = x[i]<px ? x[i] : px;
    cpy[i] = y[i]<py ? y[i] : py;

    //{
    //  pushSpriteScaled(&bg, &tft, px, py, SP_WIDTH, SP_HEIGHT);
    //}
    //if (!warp)
    {
      pushToSprite(&spr, &bg, x[i], y[i], 0);
    }
  }
  #if false
  for (i=0; i<COUNT; i++)
  {
    pushSpriteScaled(&bg, &tft, cpx[i], cpy[i], SP_WIDTH+spx[i], SP_HEIGHT+spy[i]);
  }
  #else
  pushSpriteScaled(&bg, &tft, 0, 0, bg.width(), bg.height());
  #endif
  tft.endWrite();
}

// 他のSpriteと衝突チェック
bool checkCollide(const int32_t x_, const int32_t y_, const uint8_t maxIdx_, const uint8_t selfIdx_)
{
  for (uint32_t i=0; i<maxIdx_; i++)
  {
    if (i == selfIdx_) continue;

    int32_t sx = abs(x_ - x[i]);
    int32_t sy = abs(y_ - y[i]);
    if (sx < SP_WIDTH && sy < SP_HEIGHT) return true;
  }
  return false;
}

// 他のSpriteと衝突しない座標を返す
void getRandomPos(const uint8_t idx_, int32_t *x_, int32_t *y_)
{
  while(true)
  {
    *x_ = random(tft.width()/2 - SP_WIDTH);
    *y_ = random(tft.height()/2 - SP_HEIGHT);
    bool collide = false;
    for (uint8_t i=0; i<idx_; i++)
    {
      if (checkCollide(*x_, *y_, idx_, idx_))
      {
        collide = true;
        break;
      }
    }
    if (!collide) break;
  }
  return;
}

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