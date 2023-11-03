// flower.cpp
// flower control class
#include "flower.h"

Flower::Flower(TFT_eSprite* sprite, int32_t count, TFT_eSprite *draw_target)
{
  _pattern_count = count;
  _spr = sprite;
  _draw_target = draw_target;
}

void Flower::MoveAndDraw()
{
  x += _accel_x;
  y += _accel_y;
  if (x < SCREEN_XMIN)
  {
    x = SCREEN_XMAX;
  }
  else if (x > SCREEN_XMAX)
  {
    x = SCREEN_XMIN;
  }
  if (y < SCREEN_YMIN)
  {
    y = SCREEN_YMIN;
    _accel_y *= -1;
  }
  else if (y > SCREEN_YMAX)
  {
    //if (respawn)
    {
      y = SCREEN_YMIN;
      x = random(SCREEN_WIDTH/2 - SP_WIDTH);
      _accel_x = random(MOVESPEED) + 1;
      _accel_y = (random(MOVESPEED) + 1) * 2 * (random(2)>1 ? 1 : -1);
    }
    //else
    //{
    //  y[i] = ymax;
    //}
  }

  _spr->pushToSprite(_draw_target, x, y, TFT_TRANSPARENT);
}
