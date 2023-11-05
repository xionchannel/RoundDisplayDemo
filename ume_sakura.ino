
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "sprites.h"
#include "chara.h"
#include "utility.h"

TFT_eSPI tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer

uint16_t bg_color = TFT_BLACK;

#define COUNT 10
Chara* sp[COUNT];

#define ANIM_COUNT 32
TFT_eSprite** spr_ume1;
TFT_eSprite** spr_ume2;
TFT_eSprite** spr_sakura1;
TFT_eSprite spr_bg = TFT_eSprite(&tft);
TFT_eSprite spr_logo = TFT_eSprite(&tft);

bool respawn = true;
int32_t respawn_time_max = 50;
int32_t current_respawn_time = 25;

enum Sequence
{
  RedUme = 0,
  WhiteUme,
  Sakura,
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
  spr_ume1 = Chara::CreateFlowerSprites(&tft, ume1_palette, (uint16_t*)ume1_graphic, SP_WIDTH, SP_HEIGHT, ANIM_COUNT);

  // 白梅スプライトの初期化
  spr_ume2 = Chara::CreateFlowerSprites(&tft, ume2_palette, (uint16_t*)ume1_graphic, SP_WIDTH, SP_HEIGHT, ANIM_COUNT);

  // 桜スプライトの初期化
  spr_sakura1 = Chara::CreatePatternSprites(&tft, sakura_palette, (uint16_t*)sakura_graphic, SAKURA_WIDTH, SAKURA_HEIGHT, SAKURA_X_COUNT, SAKURA_Y_COUNT);

  // フレームバッファの初期化
  bg.setColorDepth(16);
  bg.createSprite(tft.width()/2, tft.height()/2);
  //bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  draw_bg(&bg);

  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i] = new Chara(&spr_ume1[0], ANIM_COUNT, &bg);
  }

  delay(500);

  //tft.startWrite();
  //pushSpriteScaled(&bg, &tft, 0, 0, tft.width()/2, tft.height()/2);
  //tft.endWrite();
}

void loop() {
  current_respawn_time--;
  if (current_respawn_time <= 0)
  {
    respawn = !respawn;
    current_respawn_time = random(respawn_time_max)+1;

    if (respawn)
      current_sequence = static_cast<Sequence>((static_cast<uint8_t>(current_sequence) + 1) % static_cast<uint8_t>(Sequence::Max));
  }

  tft.startWrite();
  //bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  draw_bg(&bg);
  for (uint8_t i=0; i<COUNT; i++)
  {
    bool out_of_screen = sp[i]->MoveAndDraw(respawn);
    if (!respawn && out_of_screen)
    {
      if (current_sequence == Sequence::RedUme)
        sp[i]->SetPatterns(&spr_ume1[0], ANIM_COUNT);
      else if (current_sequence == Sequence::WhiteUme)
        sp[i]->SetPatterns(&spr_ume2[0], ANIM_COUNT);
      else
        sp[i]->SetPatterns(&spr_sakura1[0], SAKURA_COUNT);
    }
  }
  
  // logo
  uint16_t c = tft.color565(random(2)*255, random(2)*255, random(2)*255);
  if (c == 0) c = TFT_RED;
  c = tft.alphaBlend(128, c, TFT_WHITE);
  //Utility::pushSprite4ToSpriteBlended(&spr_logo, &bg, ((SCREEN_WIDTH/2)-LOGO_WIDTH)/2+1, ((SCREEN_HEIGHT/2)-LOGO_HEIGHT)/2, c, 0.5f);
  
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