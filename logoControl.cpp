// logoControl.h
// logo control class
#include "logoControl.h"

// 初期化
Logo::Logo(TFT_eSprite* sprite, TFT_eSprite* draw_target)
{
  _sprite = sprite;
  _draw_target = draw_target;
  _hue = 0.0;
}

// ロゴの色変化と描画
void Logo::MoveAndDraw(float time_percent)
{
  float hue = fmod(time_percent * LOGO_RAINBOW_COUNT, 1.0f);
  float alpha = 1.0f;
  if (time_percent < 0.25f) alpha = time_percent / 0.25f;
  else if (time_percent > 0.75f) alpha = (1.0f - time_percent) / 0.25f;
  if (alpha > 1.0f) alpha = 1.0f;
  uint16_t c = HSVToColor(hue, 0.7f, 1.0f);

  Utility::pushSprite4ToSpriteBlended(_sprite, _draw_target, ((SCREEN_WIDTH/2)-LOGO_WIDTH)/2+1, ((SCREEN_HEIGHT/2)-LOGO_HEIGHT)/2, c, LOGO_ALPHA * alpha);
}

// HSV指定で色を作成
uint16_t Logo::HSVToColor(const float hue, const float sat, const float value)
{
  float hh, p, q, t, ff;
  int32_t i;
  float r, g, b;

  if (sat <= 0.0f)
  {
    r = g = b = value;
    return color565(r, g, b);
  }

  hh = hue;
  if (hh >= 1.0f) hh = 0.0f;
  hh *= 6;
  i = (int32_t)hh;
  ff = hh - i;
  p = value * (1.0f - sat);
  q = value * (1.0f - (sat * ff));
  t = value * (1.0f - (sat * (1.0f - ff)));

  switch(i)
  {
    case 0:
      r = value;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = value;
      b = p;
      break;
    case 2:
      r = p;
      g = value;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = value;
      break;
    case 4:
      r = t;
      g = p;
      b = value;
      break;
    case 5:
    default:
      r = value;
      g = p;
      b = q;
      break;
  }
  return color565(r, g, b);
}

uint16_t Logo::color565(float r_, float g_, float b_)
{
  uint8_t r = r_ * 255;
  uint8_t g = g_ * 255;
  uint8_t b = b_ * 255;
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}