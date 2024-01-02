#ifndef __MAIN_H__
#define __MAIN_H__


#include <FastLED.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;


#define LED_ROW 8
#define LED_COL 32
#define LED_NUM (LED_ROW * LED_COL)
#define DATA_PIN 23
#define LED_ROTATION  1


typedef enum
{
  SYS_WIFI_START,
  SYS_WIFI_CONNECTING,
  SYS_WIFI_STADUS,
  SYS_REAL_TIME,
  SYS_FFT,

  SYS_ERR = 0xff,
} sys_mode_t;

typedef enum
{
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WIFI_CONNECT_FAILED,
  WIFI_IDLE,
} wifi_sts_t;



#endif //__MAIN_H__
