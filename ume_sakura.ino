
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "ume.h"
#include "chara.h"
#include "utility.h"

TFT_eSPI    tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer
// the pointer is used by pushSprite() to push it onto the TFT

#define BGCOLOR 4
uint16_t bg_color;

#define COUNT 15
Chara* sp[COUNT];

#define ANIM_COUNT 16
TFT_eSprite* spr[ANIM_COUNT];

bool respawn = true;
int32_t respawn_time_max = 50;
int32_t current_respawn_time = 25;

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // スプライトの初期化
  TFT_eSprite spt = TFT_eSprite(&tft);
  spt.setColorDepth(4);
  spt.createSprite(SP_WIDTH, SP_HEIGHT);
  spt.createPalette(ume_palette);
  spt.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t *)ume);
  for (uint8_t i=0; i<ANIM_COUNT; i++)
  {
    spr[i] = new TFT_eSprite(&tft);
    spr[i]->setColorDepth(16);
    if (i==0)
    {
      spr[i]->createSprite(SP_WIDTH, SP_HEIGHT);
      pushSprite4ToSprite(&spt, spr[i], 0, 0, 0);
    }
    else
    {
      //spt.setPaletteColor(2, TFT_WHITE);
      spr[i]->createSprite(SP_WIDTH, SP_HEIGHT);
      spr[i]->fillRect(0, 0, spr[i]->width(), spr[i]->height(), TFT_TRANSPARENT);
      spr[0]->setPivot(spr[0]->width()/2, spr[0]->height()/2);
      spr[0]->pushRotated(spr[i], ((double)i)/((double)ANIM_COUNT) * 360.0, TFT_TRANSPARENT);
    }
  }

  bg_color = spt.getPaletteColor(BGCOLOR);

  // フレームバッファの初期化
  bg.setColorDepth(16);
  bg.createSprite(tft.width()/2, tft.height()/2);
  bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);

  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i] = new Chara(&spr[0], ANIM_COUNT, &bg);
  }

  delay(500);

  //tft.startWrite();
  //pushSpriteScaled(&bg, &tft, 0, 0, tft.width()/2, tft.height()/2);
  //tft.endWrite();
}

void loop() {
  current_respawn_time--;
  if (current_respawn_time == 0)
  {
    respawn = !respawn;
    current_respawn_time = random(respawn_time_max);
  }

  tft.startWrite();
  bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i]->MoveAndDraw(respawn);
  }
  pushSpriteScaled(&bg, &tft, 0, 0, bg.width(), bg.height());
  tft.endWrite();
}
