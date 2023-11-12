
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "sprites.h"
#include "chara.h"
#include "utility.h"
#include "logoControl.h"

TFT_eSPI tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer

uint16_t bg_color = TFT_BLACK;

#define COUNT 30  // スプライトの最大数
#define UME_COUNT 10  // 梅のときのスプライト最大数
Chara* sp[COUNT]; // スプライトコントロール用クラス
Logo* logo_control; // ロゴコントロール用クラス
Logo* space_control;  // スペース番号コントロール用クラス

CopyBuffer** buf; // 円形コピー用の範囲バッファ
int32_t buf_count;  // 上記のバッファの配列数

#define ANIM_COUNT 32
TFT_eSprite** spr_ume1; // 紅梅
TFT_eSprite** spr_ume2; // 白梅
TFT_eSprite** spr_sakura; // 桜の花びら
TFT_eSprite** spr_hippo;  // Hippo
TFT_eSprite spr_bg = TFT_eSprite(&tft); // 背景
TFT_eSprite spr_logo = TFT_eSprite(&tft); // サークルロゴ
TFT_eSprite spr_space = TFT_eSprite(&tft);  // スペース番号

int32_t sequence_time_max = 200;
int32_t current_sequence_time = 50;

// フレームレート管理
const unsigned long frame_ms = 50;// 1フレームあたりの単位時間(ms)
unsigned long merc; // フレーム管理時計用
unsigned long curr; // 現在時刻取をミリ秒で取得する用
unsigned long curr_micro; // 現在時刻をマイクロ秒で取得する用


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
    spr_logo.setColorDepth(4);
    spr_logo.createSprite(LOGO_WIDTH, LOGO_HEIGHT);
    spr_logo.createPalette(logo_palette);
    spr_logo.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, (uint16_t*)logo_graphic);
    logo_control = new Logo(&spr_logo, &bg);
  }

  // スペース番号の初期化
  {
    spr_space.setColorDepth(4);
    spr_space.createSprite(SPACE_WIDTH, SPACE_HEIGHT);
    spr_space.createPalette(space_palette);
    spr_space.pushImage(0, 0, SPACE_WIDTH, SPACE_HEIGHT, (uint16_t*)space_graphic);
    space_control = new Logo(&spr_space, &bg);
    space_control->is_circle = false;
    space_control->is_rainbow = false;

    space_control->StartFade(true);
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
  spr_hippo = Chara::CreatePatternSprites(&tft, hippo_palette, (uint16_t*)hippo_graphic, 4, HIPPO_WIDTH, HIPPO_HEIGHT, HIPPO_X_COUNT, HIPPO_Y_COUNT);

  // フレームバッファの初期化
  bg.setColorDepth(16);
  bg.createSprite(tft.width()/2, tft.height()/2);
  //bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  draw_bg(&bg);

  for (uint8_t i=0; i<COUNT; i++)
  {
    if (i<UME_COUNT)
    {
      sp[i] = new Chara(&spr_ume1[0], ANIM_COUNT, &bg, i, true);
    }
    else
    {
      sp[i] = new Chara(&spr_sakura[0], SAKURA_ANIM_COUNT, &bg, i, true);
      sp[i]->active = false;
    }
  }

  buf = Utility::calcCircleBuffers(SCREEN_CENTER_X, SCREEN_CENTER_X);
  buf_count = SCREEN_CENTER_X * 2;

  merc = millis();  // フレーム管理時計用の初期時刻

  //delay(500);

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
    // シーケンスの変化の瞬間の処理
    current_sequence_time = sequence_time_max;
    current_sequence = static_cast<Sequence>((static_cast<uint8_t>(current_sequence) + 1) % static_cast<uint8_t>(Sequence::Max));

    switch(current_sequence)
    {
      case Sequence::Hippo:
        space_control->StartFade(false);
        logo_control->StartFade(true);
        break;
      case Sequence::RedUme:
        logo_control->StartFade(false);
        space_control->StartFade(true);
        break;
    }
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
      continue;
    }

    // スプライトが画面外になったときの処理
    //if (out_of_screen)
    {
      if (current_sequence == Sequence::RedUme)
      {
        if (sp[i]->id>=UME_COUNT) sp[i]->active = false;
        else sp[i]->SetPatterns(&spr_ume1[0], ANIM_COUNT, true);
      }
      else if (current_sequence == Sequence::WhiteUme)
      {
        if (sp[i]->id>=UME_COUNT) sp[i]->active = false;
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
        if (sp[i]->id>=UME_COUNT) sp[i]->active = false;
      }
    }
  }

  // スプライトの描画処理
  sort_charas(&sp[0], COUNT);
  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i]->Draw();
  }
  
  // ロゴ、スペース番号表示処理
  logo_control->MoveAndDraw();
  space_control->MoveAndDraw();
  
  // フレームバッファの拡大転送
  //Utility::pushSpriteScaled(&bg, &tft, 0, 0, bg.width(), bg.height());
  Utility::pushSpriteScaledBuffers(&bg, &tft, buf, buf_count);
  tft.endWrite();

  // フレーム時間管理
  curr = millis(); // 現在時刻を更新
  curr_micro = micros(); // 現在時刻を取得
  while (curr < merc)
  {
    // あまり時間を消費する
    curr = millis();
  }
  merc += frame_ms; //フレーム管理時計を1フレーム分進める
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