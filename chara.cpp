// chara.cpp
// chara control class
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
Chara::Chara(TFT_eSprite** sprites, int32_t count, TFT_eSprite *draw_target, uint32_t index)
{
  SetPatterns(sprites, count);

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
}

// スプライトパターンを差し替える
void Chara::SetPatterns(TFT_eSprite** sprites, int32_t count)
{
  _pattern_count = count;
  _pattern_num = random(count);
  _pattern_anim_time = 0;
  _pattern_anim_time_max = random(3);
  _spr = sprites;
}

// 移動、返り値は画面外にいる場合trueを返す
bool Chara::Move(bool respawn)
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

  //if (!out_of_screen)
  //{
  //  (_spr[_pattern_num])->pushToSprite(_draw_target, x, y, TFT_TRANSPARENT);
  //}

  if (_pattern_anim_time <= 0)
  {
    _pattern_num = (_pattern_num + 1) % _pattern_count;
  }
  _pattern_anim_time--;
  if (_pattern_anim_time < 0) _pattern_anim_time = _pattern_anim_time_max;
  //(*_spr[_pattern_num]).setPivot(SP_WIDTH/2, SP_HEIGHT/2);
  //(*_spr[_pattern_num]).pushRotated(_draw_target, random(360), TFT_TRANSPARENT);
  return out_of_screen;
}

void Chara::Draw()
{
  if (!active) return;
  if (out_of_screen) return;
  (_spr[_pattern_num])->pushToSprite(_draw_target, x, y, TFT_TRANSPARENT);
}