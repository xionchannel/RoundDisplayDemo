// chara.h
// chara control class
#include <TFT_eSPI.h>

#define SCREEN_WIDTH 244  // 画面端のゴミ回避のため少し大きめにする
#define SCREEN_HEIGHT 244 // 画面端のゴミ回避のため少し大きめにする

#define SP_WIDTH  32
#define SP_HEIGHT 32

#define SCREEN_XMIN (-SP_WIDTH)
#define SCREEN_YMIN (-SP_HEIGHT)
#define SCREEN_XMAX (SCREEN_WIDTH/2)
#define SCREEN_YMAX (SCREEN_HEIGHT/2)

#define MOVESPEED 3

class Chara
{
public:
  float x;
  float y;

  Chara(TFT_eSprite** sprites, int32_t count, TFT_eSprite *draw_target);
  void MoveAndDraw(bool respawn);

private:
  TFT_eSprite* _draw_target;
  TFT_eSprite** _spr;
  int32_t _pattern_count;
  int32_t _pattern_num;
  int32_t _pattern_anim_time;
  int32_t _pattern_anim_time_max;
  float _speed;
  float _accel_x;
  float _accel_y;
  float _time_count1;
  float _time_count2;
};
