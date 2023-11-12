// logoControl.h
// logo control class
#include <TFT_eSPI.h>
#include "utility.h"
#include "sprites.h"

#ifndef _LOGO_
#define _LOGO_

#define LOGO_ALPHA  0.75f
#define LOGO_RAINBOW_COUNT 3

class Logo
{
public:
  Logo(TFT_eSprite* sprite, TFT_eSprite* draw_target);
  void MoveAndDraw(float time_percent);

private:
  TFT_eSprite* _draw_target;
  TFT_eSprite* _sprite;

  uint16_t HSVToColor(const float hue, const float sat, const float value);
  uint16_t color565(float r_, float g_, float b_);
};

#endif
