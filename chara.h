// chara.h
// chara control class
#include <TFT_eSPI.h>
#include "utility.h"

#ifndef _CHARA_
#define _CHARA_

#define SP_WIDTH  32
#define SP_HEIGHT 32

#define SCREEN_XMIN (-SP_WIDTH)
#define SCREEN_YMIN (-SP_HEIGHT)
#define SCREEN_XMAX (SCREEN_WIDTH/2)
#define SCREEN_YMAX (SCREEN_HEIGHT/2)

#define MOVESPEED 3.0
#define MOVESPEED_HIPPO 2.0
#define HIPPO_ANIM_SPEED 3

#define TOUCH_ACCEL_SCALE 0.9f
#define TOUCH_ACCEL_MAX 8.0f

class Chara
{
public:
  uint32_t id;
  float x;
  float y;
  bool active;
  bool out_of_screen;
  bool is_flower;
  int32_t width;
  int32_t height;

  Chara(TFT_eSprite** sprites, int32_t count, TFT_eSprite *draw_target, uint32_t index, bool is_flower_);
  void SetPatterns(TFT_eSprite** sprites, int32_t count, bool is_flower_);
  bool Move(bool respawn);
  void Draw();
  void Touched(float x_, float y_);

  static TFT_eSprite** CreateFlowerSprites(TFT_eSPI *tft, const uint16_t* palette, const uint16_t* graphic, const uint16_t transp, const uint16_t width, const uint16_t height, const uint16_t count);
  static TFT_eSprite** CreatePatternSprites(TFT_eSPI *tft, const uint16_t* palette, const uint16_t* graphic, const uint16_t transp, const uint16_t width, const uint16_t height, const uint16_t xcount, const uint16_t ycount);
  static TFT_eSprite** CreateRotPatternSprites(TFT_eSPI *tft, const uint16_t* palette, const uint16_t* graphic, const uint16_t transp, const uint16_t width, const uint16_t height, const uint16_t xcount, const uint16_t ycount, const uint16_t anim_count);

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
  float _accel_x_touch;
  float _accel_y_touch;

  bool MoveFlower(bool respawn);
  bool MoveHippo(bool respawn);
  void SpawnFlower();
  void SpawnHippo();
  bool IsOutOfScreen();
};

#endif
