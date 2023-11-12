// logoControl.h
// logo control class
#include <TFT_eSPI.h>
#include "utility.h"
#include "sprites.h"

#ifndef _LOGO_
#define _LOGO_

#define LOGO_ALPHA  0.75f
#define LOGO_RAINBOW_SPEED 0.025f

#define LOGO_FADE_TIME 30.0f

enum LogoFadeMode
{
  None = 0,
  FadeIn,
  FadeOut
};

class Logo
{
public:
  bool is_circle;
  bool is_rainbow;
  float sat;
  float value;
  int32_t y_offset;
  int32_t x_offset;

  Logo(TFT_eSprite* sprite, TFT_eSprite* draw_target);
  void MoveAndDraw();
  void StartFade(bool is_fade_in);

private:
  TFT_eSprite* _draw_target;
  TFT_eSprite* _sprite;
  int32_t _width;
  int32_t _height;
  int32_t _current_fade_count;
  LogoFadeMode _fade_mode;
  float _hue;

  uint16_t HSVToColor(const float hue, const float sat, const float value);
  uint16_t color565(float r_, float g_, float b_);
};

#endif
