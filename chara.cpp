// chara.cpp
// chara control class
#include "define.h"
#include "chara.h"

// 花スプライトリストの初期化 （指定の数分回転したパターンを生成する）
TFT_eSprite** Chara::CreateFlowerSprites(TFT_eSPI *tft, const uint16_t* palette, const uint16_t* graphic, const uint16_t transp, const uint16_t width, const uint16_t height, const uint16_t count)
{
  TFT_eSprite** spr = new TFT_eSprite*[count];
  TFT_eSprite spt = TFT_eSprite(tft);
  spt.setColorDepth(4);
  spt.createSprite(width, height);
  spt.createPalette(palette);
  spt.pushImage(0, 0, width, height, graphic);
  for (uint16_t i=0; i<count; i++)
  {
    spr[i] = new TFT_eSprite(tft);
    spr[i]->setColorDepth(16);
    if (i==0)
    {
      spr[i]->createSprite(width, height);
      Utility::pushSprite4ToSprite(&spt, spr[i], 0, 0, transp);
    }
    else
    {
      spr[i]->createSprite(width, height);
      spr[i]->fillRect(0, 0, width, height, TFT_TRANSPARENT);
      spr[0]->setPivot(width/2, height/2);
      spr[0]->pushRotated(spr[i], ((double)i)/((double)count) * 360.0, TFT_TRANSPARENT);
    }
  }
  spt.deleteSprite();
  return &spr[0];
}

// パターンチェンジ用スプライトの生成
TFT_eSprite** Chara::CreatePatternSprites(TFT_eSPI *tft, const uint16_t* palette, const uint16_t* graphic, const uint16_t transp, const uint16_t width, const uint16_t height, const uint16_t xcount, const uint16_t ycount)
{
  TFT_eSprite** spr = new TFT_eSprite*[xcount * ycount];
  TFT_eSprite spt = TFT_eSprite(tft);
  spt.setColorDepth(4);
  spt.createSprite(width * xcount, height * ycount);
  spt.createPalette(palette);
  spt.pushImage(0, 0, width * xcount, height * ycount, graphic);
  uint16_t i=0;
  for (uint16_t y=0; y<ycount; y++)
  {
    for (uint16_t x=0; x<xcount; x++)
    {
      spr[i] = new TFT_eSprite(tft);
      spr[i]->setColorDepth(16);
      spr[i]->createSprite(width, height);
      Utility::pushSprite4ToSpriteClip(&spt, spr[i], x*width, y*height, x*width+width-1, y*height+height-1, 0, 0, transp);
      i++;
    }
  }
  spt.deleteSprite();
  return &spr[0];
}

// 回転パターンチェンジ用スプライトの生成
TFT_eSprite** Chara::CreateRotPatternSprites(TFT_eSPI *tft, const uint16_t* palette, const uint16_t* graphic, const uint16_t transp, const uint16_t width, const uint16_t height, const uint16_t xcount, const uint16_t ycount, const uint16_t anim_count)
{
  TFT_eSprite** spr = new TFT_eSprite*[xcount * ycount * anim_count];
  TFT_eSprite** spr_src = CreatePatternSprites(tft, palette, graphic, transp, width, height, xcount, ycount);

  for (uint32_t i=0; i<anim_count; i++)
  {
    for (uint32_t p=0; p<xcount*ycount; p++)
    {
      uint32_t index = i*xcount*ycount+p;
      spr[index] = new TFT_eSprite(tft);
      spr[index]->setColorDepth(16);
      spr[index]->createSprite(width, height);
      spr[index]->fillRect(0, 0, width, height, TFT_TRANSPARENT);
      spr_src[p]->setPivot(width/2, height/2);
      spr_src[p]->pushRotated(spr[index], ((double)i)/((double)anim_count) * 360.0, TFT_TRANSPARENT);
    }
  }

  free(spr_src);
  return &spr[0];
}

// 初期化
Chara::Chara(TFT_eSprite** sprites, int32_t count, TFT_eSprite *draw_target, uint32_t index, bool is_flower_)
{
  SetPatterns(sprites, count, is_flower_);

  _draw_target = draw_target;

  id = index;
  active = true;
  out_of_screen = false;
  x = random(SCREEN_WIDTH/2 - SP_WIDTH);
  y = SCREEN_YMIN;
  _speed = random(MOVESPEED) + 1;
  //int32_t flag = (random(2) > 0 ? -1 : 1);
  //_accel_x = _speed * flag;
  _accel_x = random(MOVESPEED * 2.0) - MOVESPEED;
  _accel_y = random(MOVESPEED * 2.0) + 1.0;
  _time_count1 = random(6.28f);
  _time_count2 = random(6.28f);

  _accel_x_touch = 0.0f;
  _accel_y_touch = 0.0f;
}

// 指定の座標にタッチされたので、弾き飛ばされる加速を与える
void Chara::Touched(float x_, float y_)
{
  // タッチ座標を中心として、近いほど加速度が強いようにする
  float ax = (x - x_) / SCREEN_WIDTH;
  float sign_x = (ax>0)-(ax<0);
  float abs_x = abs(ax);
  if (abs_x > 1.0f) abs_x = 0.0f;
  else abs_x = 1.0f - abs_x;
  _accel_x_touch = abs_x * sign_x * TOUCH_ACCEL_MAX;

  float ay = (y - y_) / SCREEN_HEIGHT;
  float sign_y = (ay>0)-(ay<0);
  float abs_y = abs(ay);
  if (abs_y > 1.0f) abs_y = 0.0f;
  else abs_y = 1.0f - abs_y;
  _accel_y_touch = abs_y * sign_y * TOUCH_ACCEL_MAX;
}

// スプライトパターンを差し替える
void Chara::SetPatterns(TFT_eSprite** sprites, int32_t count, bool is_flower_)
{
  _spr = sprites;
  width = sprites[0]->width();
  height = sprites[0]->height();

  is_flower = is_flower_;
  if (is_flower)
  {
    _pattern_count = count;
    _pattern_num = random(count);
    _pattern_anim_time = 0;
    _pattern_anim_time_max = random(3);
    SpawnFlower();
  }
  else
  {
    _pattern_count = 2;
    _pattern_num = random(2);
    _pattern_anim_time = 0;
    _pattern_anim_time_max = HIPPO_ANIM_SPEED;
    SpawnHippo();
  }
}

// 移動、返り値は画面外にいる場合trueを返す
bool Chara::Move(bool respawn)
{
  // タッチ加速がある場合は処理する
  if (abs(_accel_x_touch) > 0.001f || abs(_accel_y_touch) > 0.001f)
  {
    x += _accel_x_touch;
    y += _accel_y_touch;
    _accel_x_touch *= TOUCH_ACCEL_SCALE;
    _accel_y_touch *= TOUCH_ACCEL_SCALE;
  }

  if (is_flower)
  {
    return MoveFlower(respawn);
  }
  else
  {
    return MoveHippo(respawn);
  }
}

// 花用の移動処理、返り値は画面外にいる場合trueを返す
bool Chara::MoveFlower(bool respawn)
{
  out_of_screen = false;
  if (!active) return true;

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
    out_of_screen = true;
  }
  else if (x > SCREEN_XMAX)
  {
    x = SCREEN_XMIN;
    out_of_screen = true;
  }
  if (y < SCREEN_YMIN)
  {
    y = SCREEN_YMIN;
    _accel_y *= -1;
    out_of_screen = true;
  }
  else if (y > SCREEN_YMAX)
  {
    out_of_screen = true;
    if (respawn)
    {
      SpawnFlower();
    }
    else
    {
      y = SCREEN_YMAX;
    }
  }

  if (_pattern_anim_time <= 0)
  {
    _pattern_num = (_pattern_num + 1) % _pattern_count;
  }
  _pattern_anim_time--;
  if (_pattern_anim_time < 0) _pattern_anim_time = _pattern_anim_time_max;

  return out_of_screen;
}

// Hippo用の移動処理、返り値は画面外にいる場合trueを返す
bool Chara::MoveHippo(bool respawn)
{
  out_of_screen = false;
  if (!active) return true;

  _time_count1--;
  if (_time_count1 < 0)
  {
    if (random(2) == 0) _accel_x *= -1;
    else _accel_y *= -1;
    _time_count1 = _time_count2;
  }

  x += _accel_x;
  y += _accel_y;
  if (x < SCREEN_XMIN)
  {
    x = SCREEN_XMAX;
    SpawnHippo();
    out_of_screen = true;
  }
  else if (x > SCREEN_XMAX)
  {
    x = SCREEN_XMIN;
    SpawnHippo();
    out_of_screen = true;
  }
  if (y < SCREEN_YMIN)
  {
    y = SCREEN_YMIN;
    SpawnHippo();
    out_of_screen = true;
  }
  else if (y > SCREEN_YMAX)
  {
    out_of_screen = true;
    SpawnHippo();
    y = SCREEN_YMAX;
  }

  if (_pattern_anim_time <= 0)
  {
    _pattern_num = (_pattern_num + 1) % _pattern_count;
  }
  _pattern_anim_time--;
  if (_pattern_anim_time < 0) _pattern_anim_time = _pattern_anim_time_max;

  return out_of_screen;
}

void Chara::SpawnFlower()
{
  _accel_x_touch = 0.0f;
  _accel_y_touch = 0.0f;

  y = SCREEN_YMIN;
  x = random(SCREEN_WIDTH/2 - SP_WIDTH);
  _speed = random(MOVESPEED) + 1;
  //int32_t flag = (random(2) > 0 ? -1 : 1);
  //_accel_x = _speed * flag;
  _accel_x = random(MOVESPEED * 2.0) - MOVESPEED;
  _accel_y = random(MOVESPEED * 2.0) + 1.0;
}

void Chara::SpawnHippo()
{
  _accel_x_touch = 0.0f;
  _accel_y_touch = 0.0f;

  float flagx = random(2)>0 ? -1.0f : 1.0f;
  float flagy = random(2)>0 ? -1.0f : 1.0f;
  if (x <= SCREEN_XMIN)
  {
    flagx=1.0f;
    y = random(SCREEN_YMAX - SCREEN_YMIN) + SCREEN_YMIN;
  }
  else if (x >= SCREEN_XMAX)
  {
    flagx=-1.0f;
    y = random(SCREEN_YMAX - SCREEN_YMIN) + SCREEN_YMIN;
  }
  if (y <= SCREEN_YMIN)
  {
    flagy=1.0f;
    x = random(SCREEN_XMAX - SCREEN_XMIN) + SCREEN_XMIN;
  }
  else if (y >= SCREEN_YMAX)
  {
    flagy=-1.0f;
    x = random(SCREEN_XMAX - SCREEN_XMIN) + SCREEN_XMIN;
  }
  _accel_x = (random(MOVESPEED_HIPPO * 10.0)/10.0 + 1.0) * flagx;
  _accel_y = (random(MOVESPEED_HIPPO * 10.0)/10.0 + 1.0) * flagy;
  _time_count1 = _time_count2 = random(10) + 50;
}

void Chara::Draw()
{
  if (!active) return;
  if (out_of_screen) return;
  if (IsOutOfScreen()) return;

  int32_t pat = _pattern_num;
  if (!is_flower)
  {
    // Hippoの場合は移動方向に応じてパターンを変化させる
    if (abs(_accel_x) > abs(_accel_y))
    {
      if (_accel_x > 0) pat = 3 + _pattern_num * 4;
      else pat = 2 + _pattern_num * 4;
    }
    else
    {
      if (_accel_y < 0) pat = 1 + _pattern_num * 4;
      else pat = _pattern_num * 4;
    }
  }
  (_spr[pat])->pushToSprite(_draw_target, x, y, TFT_TRANSPARENT);
}

bool Chara::IsOutOfScreen()
{
  int32_t x_ = SCREEN_CENTER_X - x - width;
  int32_t y_ = SCREEN_CENTER_Y - y - height;
  int32_t dist = (x_ * x_) + (y_ * y_);
  return (dist > SCREEN_DISTANCE + height*height*4);
}