#include "ume.h"

/* This was generated using a script based on the SparkFun BMPtoArray python script
 See https://github.com/sparkfun/BMPtoArray for more info */

const uint16_t ume_palette[] = {
  0x0000, 0x49e5, 0xb926, 0xf70d, 0x0008
};

// width is 32, height is 32
const uint8_t ume[] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x22, 0x22, 0x10, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x22, 0x22, 0x22, 0x21, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x22, 0x22, 0x22, 0x22, 0x10, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 
0x00, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x21, 
0x11, 0x11, 0x10, 0x00, 0x00, 

0x00, 0x01, 0x22, 0x22, 0x22, 0x12, 0x22, 0x22, 0x22, 0x22, 0x12, 
0x22, 0x22, 0x21, 0x00, 0x00, 

0x00, 0x12, 0x22, 0x22, 0x22, 0x21, 0x22, 0x22, 0x22, 0x21, 0x22, 
0x22, 0x22, 0x22, 0x10, 0x00, 

0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x33, 0x32, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x21, 0x00, 

0x12, 0x22, 0x22, 0x22, 0x22, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 
0x22, 0x22, 0x22, 0x22, 0x10, 

0x12, 0x22, 0x22, 0x22, 0x22, 0x33, 0x32, 0x33, 0x32, 0x33, 0x32, 
0x22, 0x22, 0x22, 0x22, 0x10, 

0x12, 0x22, 0x22, 0x22, 0x22, 0x33, 0x32, 0x22, 0x22, 0x33, 0x32, 
0x22, 0x22, 0x22, 0x22, 0x10, 

0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x10, 

0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x32, 0x23, 0x22, 0x32, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x10, 

0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x32, 0x23, 0x22, 0x32, 0x22, 
0x22, 0x22, 0x22, 0x22, 0x10, 

0x01, 0x22, 0x22, 0x22, 0x11, 0x22, 0x23, 0x23, 0x23, 0x22, 0x21, 
0x12, 0x22, 0x22, 0x21, 0x00, 

0x00, 0x12, 0x22, 0x11, 0x22, 0x22, 0x23, 0x23, 0x23, 0x22, 0x22, 
0x21, 0x12, 0x22, 0x10, 0x00, 

0x00, 0x01, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x21, 0x11, 0x00, 0x00, 

0x00, 0x00, 0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x10, 0x00, 0x00, 

0x00, 0x00, 0x12, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x10, 0x00, 0x00, 

0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x21, 0x00, 0x00, 

0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x21, 0x00, 0x00, 

0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x21, 0x00, 0x00, 

0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x21, 0x00, 0x00, 

0x00, 0x00, 0x12, 0x22, 0x22, 0x22, 0x22, 0x21, 0x22, 0x22, 0x22, 
0x22, 0x22, 0x10, 0x00, 0x00, 

0x00, 0x00, 0x01, 0x22, 0x22, 0x22, 0x22, 0x11, 0x12, 0x22, 0x22, 
0x22, 0x21, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x11, 0x22, 0x22, 0x11, 0x00, 0x01, 0x12, 0x22, 
0x21, 0x10, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x01, 0x11, 0x11, 0x10, 0x00, 0x00, 0x11, 0x11, 
0x11, 0x00, 0x00, 0x00, 0x00, 

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00};