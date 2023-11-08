
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "sprites.h"
#include "chara.h"
#include "utility.h"

TFT_eSPI tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer

uint16_t bg_color = TFT_BLACK;

#define COUNT 30
Chara* sp[COUNT];

#define ANIM_COUNT 32
TFT_eSprite** spr_ume1;
TFT_eSprite** spr_ume2;
TFT_eSprite** spr_sakura;
TFT_eSprite** spr_hippo;
TFT_eSprite spr_bg = TFT_eSprite(&tft);
TFT_eSprite spr_logo = TFT_eSprite(&tft);

int32_t sequence_time_max = 200;
int32_t current_sequence_time = 50;

enum Sequence
{
  RedUme = 0,
  WhiteUme,
  Sakura,
  PreHippo,
  Hippo,
  PostHippo,
  Max
};
enum Sequence current_sequence = RedUme;


void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // ロゴの初期化
  {
    TFT_eSprite spt = TFT_eSprite(&tft);
    spt.setColorDepth(4);
    spt.createSprite(LOGO_WIDTH, LOGO_HEIGHT);
    spt.createPalette(logo_palette);
    spt.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, (uint16_t*)logo_graphic);

    spr_logo.setColorDepth(4);
    spr_logo.createSprite(LOGO_WIDTH, LOGO_HEIGHT);
    spr_logo.createPalette(logo_palette);
    spr_logo.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, (uint16_t*)logo_graphic);

    spt.deleteSprite();
  }

  // 背景の初期化
  {
    TFT_eSprite spt = TFT_eSprite(&tft);
    spt.setColorDepth(4);
    spt.createSprite(SP_WIDTH, SP_HEIGHT);
    spt.createPalette(ground_palette);
    Utility::createPaletteMultiply(&spt, 0.4f, 0.2f, 0.4f);
    spt.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t*)ground_graphic);

    spr_bg.setColorDepth(16);
    spr_bg.createSprite(SP_WIDTH, SP_HEIGHT);
    Utility::pushSprite4ToSprite(&spt, &spr_bg, 0, 0);

    spt.deleteSprite();
  }

  // 紅梅スプライトの初期化
  spr_ume1 = Chara::CreateFlowerSprites(&tft, ume1_palette, (uint16_t*)ume1_graphic, 0, SP_WIDTH, SP_HEIGHT, ANIM_COUNT);

  // 白梅スプライトの初期化
  spr_ume2 = Chara::CreateFlowerSprites(&tft, ume2_palette, (uint16_t*)ume1_graphic, 0, SP_WIDTH, SP_HEIGHT, ANIM_COUNT);

  // 桜スプライトの初期化
  spr_sakura = Chara::CreateRotPatternSprites(&tft, sakura_palette, (uint16_t*)sakura_graphic, 0, SAKURA_WIDTH, SAKURA_HEIGHT, SAKURA_X_COUNT, SAKURA_Y_COUNT, 3);

  // ヒッポースプライトの初期化
  spr_hippo = Chara::CreatePatternSprites(&tft, hippo_palette, (uint16_t*)hippo_graphic, 4, SP_WIDTH, SP_HEIGHT, HIPPO_X_COUNT, HIPPO_Y_COUNT);

  // フレームバッファの初期化
  bg.setColorDepth(16);
  bg.createSprite(tft.width()/2, tft.height()/2);
  //bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  draw_bg(&bg);

  for (uint8_t i=0; i<COUNT; i++)
  {
    if (i<10)
    {
      sp[i] = new Chara(&spr_ume1[0], ANIM_COUNT, &bg, i, true);
    }
    else
    {
      sp[i] = new Chara(&spr_sakura[0], SAKURA_ANIM_COUNT, &bg, i, true);
      sp[i]->active = false;
    }
  }

  delay(500);

  //tft.startWrite();
  //pushSpriteScaled(&bg, &tft, 0, 0, tft.width()/2, tft.height()/2);
  //tft.endWrite();
}

void loop() {
  // Hippoの前後はリスポーンを禁止する
  bool respawn = !(current_sequence == Sequence::PreHippo || current_sequence == Sequence::PostHippo);

  uint8_t active_count = 0;
  for (uint8_t i=0; i<COUNT; i++)
  {
    if (sp[i]->active && !sp[i]->out_of_screen) active_count++;
  }
  bool force_to_next = (active_count<2 && !respawn); // Hippoの前後はアクティブ数が少なくなれば次シーケンスへ強制遷移

  // シーケンス遷移処理
  current_sequence_time--;
  if (current_sequence_time <= 0 || force_to_next)
  {
    current_sequence_time = sequence_time_max;
    current_sequence = static_cast<Sequence>((static_cast<uint8_t>(current_sequence) + 1) % static_cast<uint8_t>(Sequence::Max));
  }

  // 描画処理ここから
  tft.startWrite();
  draw_bg(&bg);

  // スプライトの処理
  for (uint8_t i=0; i<COUNT; i++)
  {
    // Sakura, Hippoのときだけスプライトの数を最大にする
    if (!sp[i]->active) {
      if (current_sequence == Sequence::Sakura || current_sequence == Sequence::Hippo)
      {
        sp[i]->active = true;
        if (current_sequence != Sequence::Hippo)
        {
          sp[i]->SetPatterns(&spr_sakura[0], SAKURA_ANIM_COUNT, true);
        }
        else
        {
          sp[i]->SetPatterns(&spr_hippo[0], HIPPO_COUNT, false);
        }
        continue;
      }
    }

    // スプライト移動処理
    bool out_of_screen = sp[i]->Move(respawn);
    if (!out_of_screen)
    {
      if (current_sequence == Sequence::Hippo)
      {
        // Hippoのときだけ相互衝突で向きを変える
        for (uint8_t j=0; j<COUNT; j++)
        {
          if (i==j) continue;
          if (bounce_check(sp[i]->x, sp[i]->y, sp[j]->x, sp[j]->y, 20))
          {
            sp[i]->Bounce();
            sp[j]->now_bounced = BOUNCE_DELAY;
          }
        }
      }
      continue;
    }

    // スプライトが画面外になったときの処理
    //if (out_of_screen)
    {
      if (current_sequence == Sequence::RedUme)
      {
        if (sp[i]->id>=10) sp[i]->active = false;
        else sp[i]->SetPatterns(&spr_ume1[0], ANIM_COUNT, true);
      }
      else if (current_sequence == Sequence::WhiteUme)
      {
        if (sp[i]->id>=10) sp[i]->active = false;
        else sp[i]->SetPatterns(&spr_ume2[0], ANIM_COUNT, true);
      }
      else if (current_sequence == Sequence::Sakura)
      {
        sp[i]->SetPatterns(&spr_sakura[0], SAKURA_ANIM_COUNT, true);
      }
      else if (current_sequence == Sequence::Hippo)
      {
        sp[i]->SetPatterns(&spr_hippo[0], HIPPO_COUNT, false);
      }
      else
      {
        if (sp[i]->id>=10) sp[i]->active = false;
      }
    }
  }

  // スプライトの描画処理
  sort_charas(&sp[0], COUNT);
  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i]->Draw();
  }
  
  // ロゴ表示処理
  uint16_t c = tft.color565(random(2)*255, random(2)*255, random(2)*255);
  if (c == 0) c = TFT_RED;
  c = tft.alphaBlend(128, c, TFT_WHITE);
  //Utility::pushSprite4ToSpriteBlended(&spr_logo, &bg, ((SCREEN_WIDTH/2)-LOGO_WIDTH)/2+1, ((SCREEN_HEIGHT/2)-LOGO_HEIGHT)/2, c, 0.5f);
  
  // フレームバッファの拡大転送
  Utility::pushSpriteScaled(&bg, &tft, 0, 0, bg.width(), bg.height());
  tft.endWrite();
}

void draw_bg(TFT_eSprite *target)
{
  for (uint8_t i=0; i<=(SCREEN_HEIGHT/2)/SP_HEIGHT; i++)
  {
    for (uint8_t j=0; j<=(SCREEN_WIDTH/2)/SP_WIDTH; j++)
    {
      spr_bg.pushToSprite(target, j*SP_WIDTH, i*SP_HEIGHT);
    }
  }
}

bool bounce_check(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t size)
{
  if (abs(x1-x2) < size && abs(y1-y2) < size) return true;
  return false;
}

int sort_desc(const void *cmp1, const void *cmp2)
{
  Chara* b = *(Chara**)cmp1;
  Chara* a = *(Chara**)cmp2;
  int32_t ay = a->x + a->y * SCREEN_WIDTH;
  int32_t by = b->x + b->y * SCREEN_WIDTH;
  return ay > by ? -1 : (ay < by ? 1 : 0);
}

void sort_charas(Chara** src, uint8_t count)
{
  qsort(src, count, sizeof(src[0]), sort_desc);
}