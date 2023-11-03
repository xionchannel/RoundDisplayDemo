
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "ume.h"
#include "flower.h"
#include "utility.h"

TFT_eSPI    tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite spr = TFT_eSprite(&tft);  // Create Sprite object "spr" with pointer to "tft" object
TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer
// the pointer is used by pushSprite() to push it onto the TFT

#define BGCOLOR 4

#define COUNT 20
Flower* sp[COUNT];

bool respawn = true;
int32_t respawn_time_max = 50;
int32_t current_respawn_time = 50;

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
  //spt.setPaletteColor(2, TFT_WHITE);
  spt.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t *)ume);
  spr.setColorDepth(16);
  spr.createSprite(SP_WIDTH, SP_HEIGHT);
  pushSprite4ToSprite(&spt, &spr, 0, 0, 0);

  // フレームバッファの初期化
  bg.setColorDepth(16);
  bg.createSprite(tft.width()/2, tft.height()/2);
  bg.fillRect(0, 0, bg.width(), bg.height(), spt.getPaletteColor(BGCOLOR));

  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i] = new Flower(&spr, 1, &bg);
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
    current_respawn_time = respawn_time_max;
  }

  tft.startWrite();
  bg.fillRect(0, 0, bg.width(), bg.height(), BGCOLOR);
  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i]->MoveAndDraw();
  }
  pushSpriteScaled(&bg, &tft, 0, 0, bg.width(), bg.height());
  tft.endWrite();
}
