// logoControl.h
// logo control class
#include "define.h"
#include "logoControl.h"

// 初期化
Logo::Logo(TFT_eSprite* sprite, TFT_eSprite* draw_target)
{
  _sprite = sprite;
  _draw_target = draw_target;
  _width = sprite->width();
  _height = sprite->height();

  _fade_mode = LogoFadeMode::None;

  y_offset = 0;
  x_offset = 0;
  is_circle = true;
  is_rainbow = false;
  is_fixed_color = true;
  fixed_color = TFT_WHITE;
  sat = 0.7f;
  value = 1.0f;
  _hue = 0.0f;
}

// ロゴの色変化と描画
void Logo::MoveAndDraw()
{
  uint16_t c;
  if (is_rainbow)
  {
    _hue += LOGO_RAINBOW_SPEED;
    if (_hue > 1.0) _hue -= 1.0;
    c = HSVToColor(_hue, sat, value);
  }
  else if (is_fixed_color)
  {
    c = fixed_color;
  }
  else
  {
    c = color565(value, value, value);
  }

  float alpha = 0.0f;
  if (_fade_mode == LogoFadeMode::FadeIn)
  {
    _current_fade_count++;
    if (_current_fade_count > LOGO_FADE_TIME) _current_fade_count = LOGO_FADE_TIME;
    alpha = (float)_current_fade_count / LOGO_FADE_TIME;
  }
  else if (_fade_mode == LogoFadeMode::FadeOut)
  {
    _current_fade_count++;
    if (_current_fade_count > LOGO_FADE_TIME) _current_fade_count = LOGO_FADE_TIME;
    alpha = 1.0 - ((float)_current_fade_count / LOGO_FADE_TIME);
  }
  if (alpha <= 0.0f) return;

  if (is_circle)
  {
    Utility::pushSprite4ToSpriteCircleBlended(_sprite, _draw_target, ((SCREEN_WIDTH/2)-_width)/2+x_offset, ((SCREEN_HEIGHT/2)-_height)/2+y_offset, c, LOGO_ALPHA * alpha, _width/2);
  }
  else
  {
    Utility::pushSprite4ToSpriteBlended(_sprite, _draw_target, ((SCREEN_WIDTH/2)-_width)/2+x_offset, ((SCREEN_HEIGHT/2)-_height)/2+y_offset, c, LOGO_ALPHA * alpha);
  }
}

// フェード開始指定
void Logo::StartFade(bool is_fade_in)
{
  if (is_fade_in)
  {
    _fade_mode = LogoFadeMode::FadeIn;
    _current_fade_count = -LOGO_FADE_TIME;
  }
  else
  {
    _fade_mode = LogoFadeMode::FadeOut;
    _current_fade_count = 0;
  }
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