// utility.h
// TFT_eSprite用のユーティリティ関数群

#include <TFT_eSPI.h>

#ifndef _UTILITY_
#define _UTILITY_

#define SCREEN_WIDTH 244  // 画面端のゴミ回避のため少し大きめにする
#define SCREEN_HEIGHT 244 // 画面端のゴミ回避のため少し大きめにする
#define SCREEN_WIDTH_HALF (SCREEN_WIDTH/2)
#define SCREEN_HEIGHT_HALF (SCREEN_HEIGHT/2)
#define SCREEN_CENTER_X (SCREEN_WIDTH_HALF/2)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT_HALF/2)

// スプライトの表示範囲計算用の距離2乗数値
#define SCREEN_DISTANCE (SCREEN_CENTER_X*SCREEN_CENTER_X)

class Utility
{
public:
  static void splitColor(const uint16_t c, uint16_t* r, uint16_t* g, uint16_t* b);
  static uint16_t scaleColor(const uint16_t color, const float scale);
  static uint16_t addColor(const uint16_t color_a, const uint16_t color_b);
  static void pushSpriteScaled(TFT_eSprite* spr_, TFT_eSPI* tft_, const int32_t x, const int32_t y, const int32_t w, const int32_t h);
  static void pushToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_);
  static void pushSprite4ToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_);
  static void pushSprite4ToSpriteClip(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t xst, const int32_t yst, const int32_t xed, const int32_t yed, const int32_t x, const int32_t y, const uint8_t transp_);
  static void pushSprite4ToSpriteMasked(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint8_t transp_);
  static void pushSprite4ToSpriteBlended(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y, const uint16_t color_, const float alpha_);
  static void pushSprite4ToSprite(TFT_eSprite* src_, TFT_eSprite* dst_, const int32_t x, const int32_t y);
  static void createPaletteMultiply(TFT_eSprite* target_, const float r_, const float g_, const float b_);

};

#endif
