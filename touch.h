// Touch driver
// lv_xiao_round_screen.hを参考にタッチ関連だけ抜き出しました
#include <lvgl.h>
#include <Wire.h>

#define SCREEN_WIDTH_RAW 240
#define SCREEN_HEIGHT_RAW 240

#define CHSC6X_I2C_ID 0x2e
#define CHSC6X_MAX_POINTS_NUM 1
#define CHSC6X_READ_POINT_LEN 5
#define TOUCH_INT D7

#ifndef XIAO_BL
#define XIAO_BL D6
#endif
#define XIAO_DC D3
#define XIAO_CS D1

uint8_t screen_rotation = 0;


/* touch driver : chsc6x */

bool chsc6x_is_pressed(void)
{
  if(digitalRead(TOUCH_INT) != LOW) {
    delay(3);
    if(digitalRead(TOUCH_INT) != LOW)
    return false;
  }
  return true;
}

void chsc6x_convert_xy(uint8_t *x, uint8_t *y)
{
  uint8_t x_tmp = *x, y_tmp = *y, _end = 0;
  for(int i=1; i<=screen_rotation; i++){
    x_tmp = *x;
    y_tmp = *y;
    _end = (i % 2) ? SCREEN_WIDTH_RAW : SCREEN_HEIGHT_RAW;
    *x = y_tmp;
    *y = _end - x_tmp;
  }
}

void chsc6x_get_xy(lv_coord_t * x, lv_coord_t * y)
{
  uint8_t temp[CHSC6X_READ_POINT_LEN] = {0};
  uint8_t read_len = Wire.requestFrom(CHSC6X_I2C_ID, CHSC6X_READ_POINT_LEN);
  if(read_len == CHSC6X_READ_POINT_LEN){
    Wire.readBytes(temp, read_len);
    if (temp[0] == 0x01) {
    chsc6x_convert_xy(&temp[2], &temp[4]);
    *x = temp[2];
    *y = temp[4];
    }
  }
}

void chsc6x_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
  lv_coord_t touchX, touchY;
  if( !chsc6x_is_pressed() )
  {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    chsc6x_get_xy(&touchX, &touchY);
    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void lv_xiao_touch_init(void)
{
  pinMode(TOUCH_INT, INPUT_PULLUP);
  Wire.begin(); // Turn on the IIC bus for touch driver

  /*Initialize the touch driver for lvgl*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = chsc6x_read;
  lv_indev_drv_register(&indev_drv);
}
