// chara.cpp
// chara control class
#include "chara.h"

Chara::Chara(TFT_eSprite** sprites, int32_t count, TFT_eSprite *draw_target)
{
  _pattern_count = count;
  _pattern_num = random(count);
  _pattern_anim_time = 0;
  _spr = sprites;
  _draw_target = draw_target;

  x = random(SCREEN_WIDTH/2 - SP_WIDTH);
  y = SCREEN_YMIN;
  _speed = random(MOVESPEED) + 1;
  //int32_t flag = (random(2) > 0 ? -1 : 1);
  //_accel_x = _speed * flag;
  _accel_x = random(MOVESPEED * 2.0) - MOVESPEED;
  _accel_y = random(MOVESPEED * 2.0) + 1.0;
  _time_count1 = random(6.28f);
  _time_count2 = random(6.28f);
}

void Chara::MoveAndDraw(bool respawn)
{
  float _x1 = sin(_time_count1);
  float _x2 = sin(_time_count2);
  _time_count1 += 0.025f * _speed;
  _time_count2 += 0.05f * _speed;
  if (_time_count1 > 6.28f) _time_count1 -= 6.28f;
  if (_time_count2 > 6.28f) _time_count2 -= 6.28f;

  x += _accel_x + _x1 * _x2;
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
    if (respawn)
    {
      y = SCREEN_YMIN;
      x = random(SCREEN_WIDTH/2 - SP_WIDTH);
      _speed = random(MOVESPEED) + 1;
      //int32_t flag = (random(2) > 0 ? -1 : 1);
      //_accel_x = _speed * flag;
      _accel_x = random(MOVESPEED * 2.0) - MOVESPEED;
      _accel_y = random(MOVESPEED * 2.0) + 1.0;
    }
    else
    {
      y = SCREEN_YMAX;
    }
  }

  (*_spr[_pattern_num]).pushToSprite(_draw_target, x, y, TFT_TRANSPARENT);
  if (_pattern_anim_time == 0)
  {
    _pattern_num = (_pattern_num + 1) % _pattern_count;
  }
  _pattern_anim_time--;
  if (_pattern_anim_time < 0) _pattern_anim_time = 3;
  //(*_spr[_pattern_num]).setPivot(SP_WIDTH/2, SP_HEIGHT/2);
  //(*_spr[_pattern_num]).pushRotated(_draw_target, random(360), TFT_TRANSPARENT);
}
