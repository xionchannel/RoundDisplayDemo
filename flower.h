// flower.h
// flower control class
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

class Flower
{
public:
  int32_t x;
  int32_t y;

  Flower(TFT_eSprite* sprite, int32_t count, TFT_eSprite *draw_target);
  void MoveAndDraw();

private:
  TFT_eSprite* _draw_target;
  TFT_eSprite* _spr;
  int32_t _pattern_count;
  int32_t _pattern_num;
  int32_t _accel_x;
  int32_t _accel_y;
  int32_t _time_count;
};
