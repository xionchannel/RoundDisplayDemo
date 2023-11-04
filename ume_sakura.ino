
//#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)
#include "sprites.h"
#include "chara.h"
#include "utility.h"

TFT_eSPI    tft = TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);         // Create object "tft"

TFT_eSprite bg = TFT_eSprite(&tft); // bg buffer
// the pointer is used by pushSprite() to push it onto the TFT

uint16_t bg_color = TFT_BLACK;

#define COUNT 10
Chara* sp[COUNT];

#define ANIM_COUNT 32
TFT_eSprite* spr_ume1[ANIM_COUNT];
TFT_eSprite* spr_ume2[ANIM_COUNT];
TFT_eSprite spr_bg = TFT_eSprite(&tft);
TFT_eSprite spr_logo = TFT_eSprite(&tft);

bool respawn = true;
int32_t respawn_time_max = 50;
int32_t current_respawn_time = 25;

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  TFT_eSprite spt = TFT_eSprite(&tft);
  // ロゴの初期化
  {
    spt.setColorDepth(4);
    spt.createSprite(LOGO_WIDTH, LOGO_HEIGHT);
    spt.createPalette(logo_palette);
    spt.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, (uint16_t*)logo_graphic);

    spr_logo.setColorDepth(4);
    spr_logo.createSprite(LOGO_WIDTH, LOGO_HEIGHT);
    //pushSprite4ToSprite(&spt, &spr_logo, 0, 0, 0);
    spr_logo.createPalette(logo_palette);
    spr_logo.pushImage(0, 0, LOGO_WIDTH, LOGO_HEIGHT, (uint16_t*)logo_graphic);

    spt.deleteSprite();
  }

  // 背景の初期化
  {
    spt.setColorDepth(4);
    spt.createSprite(SP_WIDTH, SP_HEIGHT);
    spt.createPalette(ground_palette);
    Utility::createPaletteMultiply(&spt, 0.4f, 0.2f, 0.4f);
    spt.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t*)ground_graphic);

    spr_bg.setColorDepth(16);
    spr_bg.createSprite(SP_WIDTH, SP_HEIGHT);
    Utility::pushSprite4ToSprite(&spt, &spr_bg, 0, 0);
  }

  // 紅梅スプライトの初期化
  {
    spt.setColorDepth(4);
    spt.createSprite(SP_WIDTH, SP_HEIGHT);
    spt.createPalette(ume1_palette);
    spt.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t*)ume1_graphic);
    for (uint8_t i=0; i<ANIM_COUNT; i++)
    {
      spr_ume1[i] = new TFT_eSprite(&tft);
      spr_ume1[i]->setColorDepth(16);
      if (i==0)
      {
        spr_ume1[i]->createSprite(SP_WIDTH, SP_HEIGHT);
        Utility::pushSprite4ToSprite(&spt, spr_ume1[i], 0, 0, 0);
      }
      else
      {
        spr_ume1[i]->createSprite(SP_WIDTH, SP_HEIGHT);
        spr_ume1[i]->fillRect(0, 0, SP_WIDTH, SP_HEIGHT, TFT_TRANSPARENT);
        spr_ume1[0]->setPivot(SP_WIDTH/2, SP_HEIGHT/2);
        spr_ume1[0]->pushRotated(spr_ume1[i], ((double)i)/((double)ANIM_COUNT) * 360.0, TFT_TRANSPARENT);
      }
      //spt.deleteSprite();
    }
  }

  // 白梅スプライトの初期化
  {
    //TFT_eSprite spt = TFT_eSprite(&tft);
    //spt.setColorDepth(4);
    //spt.createSprite(SP_WIDTH, SP_HEIGHT);
    spt.createPalette(ume2_palette);
    //spt.pushImage(0, 0, SP_WIDTH, SP_HEIGHT, (uint16_t*)ume2_graphic);
    for (uint8_t i=0; i<ANIM_COUNT; i++)
    {
      spr_ume2[i] = new TFT_eSprite(&tft);
      spr_ume2[i]->setColorDepth(16);
      if (i==0)
      {
        spr_ume2[i]->createSprite(SP_WIDTH, SP_HEIGHT);
        Utility::pushSprite4ToSprite(&spt, spr_ume2[i], 0, 0, 0);
      }
      else
      {
        spr_ume2[i]->createSprite(SP_WIDTH, SP_HEIGHT);
        spr_ume2[i]->fillRect(0, 0, SP_WIDTH, SP_HEIGHT, TFT_TRANSPARENT);
        spr_ume2[0]->setPivot(SP_WIDTH/2, SP_HEIGHT/2);
        spr_ume2[0]->pushRotated(spr_ume2[i], ((double)i)/((double)ANIM_COUNT) * 360.0, TFT_TRANSPARENT);
      }
      spt.deleteSprite();
    }
  }

  // フレームバッファの初期化
  bg.setColorDepth(16);
  bg.createSprite(tft.width()/2, tft.height()/2);
  //bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  draw_bg(&bg);

  for (uint8_t i=0; i<COUNT; i++)
  {
    if (i%2==0)
    {
      sp[i] = new Chara(&spr_ume1[0], ANIM_COUNT, &bg);
    }
    else
    {
      sp[i] = new Chara(&spr_ume2[0], ANIM_COUNT, &bg);
    }
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
  }

  tft.startWrite();
  //bg.fillRect(0, 0, bg.width(), bg.height(), bg_color);
  draw_bg(&bg);
  for (uint8_t i=0; i<COUNT; i++)
  {
    sp[i]->MoveAndDraw(respawn);
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