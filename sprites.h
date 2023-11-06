#include <TFT_eSPI.h>

#ifndef _SPRITES_
#define _SPRITES_

extern const uint16_t ume1_palette[];
extern const uint8_t ume1_graphic[] PROGMEM;
extern const uint16_t ume2_palette[];
//extern const uint8_t ume2_graphic[] PROGMEM;

#define HIPPO_X_COUNT 4
#define HIPPO_Y_COUNT 2
#define HIPPO_COUNT (HIPPO_X_COUNT * HIPPO_Y_COUNT)
extern const uint16_t hippo_palette[];
extern const uint8_t hippo_graphic[] PROGMEM;

#define SAKURA_WIDTH 16
#define SAKURA_HEIGHT 16
#define SAKURA_X_COUNT 4
#define SAKURA_Y_COUNT 2
#define SAKURA_COUNT (SAKURA_X_COUNT * SAKURA_Y_COUNT)
#define SAKURA_ANIM_COUNT (3 * SAKURA_COUNT)
extern const uint16_t sakura_palette[];
extern const uint8_t sakura_graphic[] PROGMEM;

extern const uint16_t ground_palette[];
extern const uint8_t ground_graphic[] PROGMEM;

#define LOGO_WIDTH 100
#define LOGO_HEIGHT 100
extern const uint16_t logo_palette[];
extern const uint8_t logo_graphic[] PROGMEM;

#endif
