#ifndef __MAIN_H__
#define __MAIN_H__

#define FASTLED_INTERNAL

#include <FastLED.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef volatile uint8_t vu8;
typedef volatile uint16_t vu16;
typedef volatile uint32_t vu32;
typedef volatile int8_t vs8;
typedef volatile int16_t vs16;
typedef volatile int32_t vs32;


// LED
const u8 DATA_PIN = 23;
const u8 LED_ROTATION = 1;
const u8 LED_ROW = 8;
const u8 LED_COL = 32;
const u16 LED_NUM = LED_ROW * LED_COL;

const s8 SYS_LOGO            = 0;
const s8 SYS_WIFI_START      = 4;
const s8 SYS_WIFI_CONNECTING = 5;
const s8 SYS_WIFI_STADUS     = 6;
const s8 SYS_REAL_TIME       = 7;
const s8 SYS_TIME_WEATHER    = 8;
const s8 SYS_FFT             = 9;
const s8 SYS_RAIN            = 10;
const s8 SYS_TEMP_HUM        = 11;

const s8 SYS_TEST            = 0x70;
const s8 SYS_ERR             = 0x7f;

const u8 SYS_FIRST = SYS_REAL_TIME;
const u8 SYS_FINAL = SYS_TEMP_HUM;


typedef enum
{
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WIFI_CONNECT_FAILED,
  WIFI_IDLE,
} wifi_sts_t;


typedef enum
{
  MOVE_UP,
  MOVE_DOWN,
  MOVE_LEFT,
  MOVE_RIGHT,
} move_dir_t;


#endif //__MAIN_H__
