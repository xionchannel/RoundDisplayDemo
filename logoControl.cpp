// logoControl.h
// logo control class
#include "logoControl.h"

// 初期化
Logo::Logo(TFT_eSprite* sprite, TFT_eSprite* draw_target)
{
  _sprite = sprite;
  _draw_target = draw_target;
  _hue = 0.0;
}

// ロゴの色変化と描画
void Logo::MoveAndDraw(float time_percent)
{
  float hue = time_percent;
  float alpha = 1;
  if (time_percent < 0.25) alpha = time_percent / 0.25;
  else if (time_percent > 0.75) alpha = (1.0 - time_percent) / 0.25;
  if (alpha > 1) alpha = 1;
  uint16_t c = HSVToColor(hue, 1, 1);

  Utility::pushSprite4ToSpriteBlended(_sprite, _draw_target, ((SCREEN_WIDTH/2)-LOGO_WIDTH)/2+1, ((SCREEN_HEIGHT/2)-LOGO_HEIGHT)/2, c, LOGO_ALPHA * alpha);
}

// HSV指定で色を作成
uint16_t Logo::HSVToColor(float hue, float sat, float value)
{
  return TFT_WHITE;
}