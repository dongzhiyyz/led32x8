#ifndef __FONT_H__
#define __FONT_H__

#include "main.h"

typedef enum
{
    LED_SZIE_35 = 0x0305,
    LED_SZIE_45 = 0x0405,
    LED_SZIE_48 = 0x0408,

} led_size;

typedef struct{
    u8 col;
    u8 row;
    u32 dat[];
} pattern;


void led_show_char(u32 (*leds)[8], u8 x, u8 y, char *p, led_size size, u32 fc = 0x01ffffff, u32 bc = 0, u8 mode = 0);
void led_show_pattern(u32 (*leds)[8], const pattern *src, u8 x = 0, u8 y = 0, u32 fc = 0x01ffffff, u32 bc = 0);

const pattern pattern_humidity = {    
    0x05, 0x08, 
    0x000000, 0x29e2ff, 0x29e2ff, 0x63dbff, 0x63dbff, 0x000000, 0x000000, 0x000000, 
    0x29e2ff, 0x29e2ff, 0x29e2ff, 0x29e2ff, 0x63dbff, 0x63dbff, 0x63dbff, 0x000000, 
    0x29e2ff, 0xe1ffef, 0x29e2ff, 0x29e2ff, 0x63dbff, 0x63dbff, 0x63dbff, 0x63dbff, 
    0x29e2ff, 0x29e2ff, 0xe1ffef, 0xe1ffef, 0x63dbff, 0x63dbff, 0x63dbff, 0x000000, 
    0x000000, 0x29e2ff, 0x29e2ff, 0x63dbff, 0x63dbff, 0x000000, 0x000000, 0x000000,
};

const pattern pattern_temp = {
    0x05, 0x08, 
    0x000000, 0x436cff, 0x436cff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x436cff, 0xff0004, 0xff0004, 0x436cff, 0x436cff, 0x436cff, 0x436cff, 0x000000, 
    0x436cff, 0xff0004, 0xff0004, 0xff0004, 0xff0004, 0xbbffee, 0xbbffee, 0x436cff, 
    0x436cff, 0xff0004, 0xff0004, 0x436cff, 0x436cff, 0x436cff, 0x436cff, 0x000000, 
    0x000000, 0x436cff, 0x436cff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const pattern pattern_test = {
    0x20, 0x08, 
    0x102c46, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xbb9eb1, 
    0x09eab7, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x34ddf4, 
    0x5eea22, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x69dac8, 
    0x299395, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x607b2e, 
    0x000000, 0x998f5d, 0x000000, 0x000000, 0x000000, 0x000000, 0xe69c66, 0x000000, 
    0x000000, 0x92c0b2, 0x000000, 0x000000, 0x000000, 0x000000, 0x7f5f63, 0x000000, 
    0x000000, 0xe4c498, 0x000000, 0x000000, 0x000000, 0x000000, 0x1964c4, 0x000000, 
    0x000000, 0x05aa6b, 0x000000, 0x000000, 0x000000, 0x000000, 0xd1f89d, 0x000000, 
    0x000000, 0x000000, 0x164d2c, 0x000000, 0x000000, 0xb3fa0f, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x667868, 0x000000, 0x000000, 0xd88e2d, 0x000000, 0x000000, 
    0x000000, 0x000000, 0xe53d02, 0x000000, 0x000000, 0xda0e64, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x692dff, 0x000000, 0x000000, 0xf12fc6, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0xaa9e7f, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x97df1d, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0xf286b9, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xff0670, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x8e816b, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xf308b0, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0xcba367, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x76d5f7, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x8a0aad, 0x000000, 0x000000, 0xefee30, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x7b604b, 0x000000, 0x000000, 0x733130, 0x000000, 0x000000, 
    0x000000, 0x000000, 0xaa8068, 0x000000, 0x000000, 0x0213b5, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x8b6aa0, 0x000000, 0x000000, 0x29bccf, 0x000000, 0x000000, 
    0x000000, 0x13c244, 0x000000, 0x000000, 0x000000, 0x000000, 0xc3be62, 0x000000, 
    0x000000, 0x71c99b, 0x000000, 0x000000, 0x000000, 0x000000, 0xcc2dde, 0x000000, 
    0x000000, 0xecb394, 0x000000, 0x000000, 0x000000, 0x000000, 0x39d523, 0x000000, 
    0x000000, 0x19c3ca, 0x000000, 0x000000, 0x000000, 0x000000, 0xcb0a07, 0x000000, 
    0x3a81d3, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x707c74, 
    0xaf740b, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x58e2eb, 
    0xb835a0, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x5bd62c, 
    0x4348f3, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x933e3f,
};

const pattern pattern_wifi_connecting1 ={
    0x20, 0x08, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const pattern pattern_wifi_connecting2 ={
    0x20, 0x08, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 
    0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};

const pattern pattern_wifi_connecting3 ={
    0x20, 0x08, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 
    0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 
    0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 
    0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x46ffe9, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x46ffe9, 0x46ffe9, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};


const pattern pattern_wifi_connect_ok ={
    0x20, 0x08, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x24ff20, 0x000000, 
    0x000000, 0x24ff20, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x24ff20, 0x000000, 
    0x24ff20, 0x000000, 0x24ff20, 0x000000, 0x24ff20, 0x000000, 0x24ff20, 0x000000, 
    0x000000, 0x24ff20, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x24ff20, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x24ff20, 0x000000, 
    0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x24ff20, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
};

const pattern pattern_wifi_connect_ng ={
    0x20, 0x08, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0xff1717, 0x000000, 
    0x000000, 0xff1717, 0x000000, 0x000000, 0xff1717, 0x000000, 0xff1717, 0x000000, 
    0xff1717, 0x000000, 0xff1717, 0x000000, 0xff1717, 0x000000, 0xff1717, 0x000000, 
    0x000000, 0xff1717, 0x000000, 0x000000, 0xff1717, 0x000000, 0xff1717, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0xff1717, 0x000000, 
    0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 
    0x000000, 0x000000, 0xff1717, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0xff1717, 0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 
    0x000000, 0xff1717, 0x000000, 0x000000, 0x000000, 0xff1717, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
};

const unsigned char ascii_0305[][3] = {
    {0x00, 0x00, 0x00}, /*" ",0*/
    {0x00, 0x1d, 0x00}, /*"!",1*/
    {0, 0, 0},          /*""",2*/
    {0, 0, 0},          /*"#",3*/
    {0, 0, 0},          /*"$",4*/
    {0x13, 0x04, 0x19}, /*"%",5*/
    {0, 0, 0},          /*"&",6*/
    {0x00, 0x18, 0x00}, /*"'",7*/
    {0x0e, 0x11, 0x00}, /*"(",8*/
    {0x00, 0x11, 0x0e}, /*")",9*/
    {0x0a, 0x04, 0x0a}, /*"*",10*/
    {0x04, 0x0e, 0x04}, /*"+",11*/
    {0x01, 0x02, 0x00}, /*",",12*/
    {0x04, 0x04, 0x04}, /*"-",13*/
    {0x01, 0x00, 0x00}, /*".",14*/
    {0x02, 0x04, 0x08}, /*"/",15*/
    {0x1f, 0x11, 0x1f}, /*"0",16*/
    {0x09, 0x1f, 0x01}, /*"1",17*/
    {0x17, 0x15, 0x1d}, /*"2",18*/
    {0x15, 0x15, 0x1f}, /*"3",19*/
    {0x1c, 0x04, 0x1f}, /*"4",20*/
    {0x1d, 0x15, 0x17}, /*"5",21*/
    {0x1f, 0x15, 0x17}, /*"6",22*/
    {0x10, 0x10, 0x1f}, /*"7",23*/
    {0x1f, 0x15, 0x1f}, /*"8",24*/
    {0x1d, 0x15, 0x1f}, /*"9",25*/
    {0x00, 0x0a, 0x00}, /*":",26*/
    {0x01, 0x0a, 0x00}, /*";",27*/
    {0x04, 0x0a, 0x00}, /*"<",28*/
    {0x0a, 0x0a, 0x0a}, /*"=",29*/
    {0x00, 0x0a, 0x04}, /*">",30*/
    {0x10, 0x15, 0x18}, /*"?",31*/
    {0, 0, 0},          /*"@",32*/
    {0x0f, 0x14, 0x0f}, /*"A",33*/
    {0x1f, 0x15, 0x0e}, /*"B",34*/
    {0x0e, 0x11, 0x11}, /*"C",35*/
    {0x1f, 0x11, 0x0e}, /*"D",36*/
    {0x1f, 0x15, 0x15}, /*"E",37*/
    {0x1f, 0x14, 0x14}, /*"F",38*/
    {0x0e, 0x15, 0x17}, /*"G",39*/
    {0, 0, 0},          /*"H",40*/
    {0, 0, 0},          /*"I",41*/
    {0, 0, 0},          /*"J",42*/
    {0, 0, 0},          /*"K",43*/
    {0, 0, 0},          /*"L",44*/
    {0, 0, 0},          /*"M",45*/
    {0, 0, 0},          /*"N",46*/
    {0, 0, 0},          /*"O",47*/
    {0, 0, 0},          /*"P",48*/
    {0, 0, 0},          /*"Q",49*/
    {0, 0, 0},          /*"R",50*/
    {0, 0, 0},          /*"S",51*/
    {0, 0, 0},          /*"T",52*/
    {0, 0, 0},          /*"U",53*/
    {0, 0, 0},          /*"V",54*/
    {0, 0, 0},          /*"W",55*/
    {0, 0, 0},          /*"X",56*/
    {0, 0, 0},          /*"Y",57*/
    {0, 0, 0},          /*"Z",58*/
    {0, 0, 0},          /*"[",59*/
    {0, 0, 0},          /*"\",60*/
    {0, 0, 0},          /*"]",61*/
    {0, 0, 0},          /*"^",62*/
    {0, 0, 0},          /*"_",63*/
    {0, 0, 0},          /*"`",64*/
    {0, 0, 0},          /*"a",65*/
    {0, 0, 0},          /*"b",66*/
    {0, 0, 0},          /*"c",67*/
    {0, 0, 0},          /*"d",68*/
    {0, 0, 0},          /*"e",69*/
    {0, 0, 0},          /*"f",70*/
    {0, 0, 0},          /*"g",71*/
    {0, 0, 0},          /*"h",72*/
    {0, 0, 0},          /*"i",73*/
    {0, 0, 0},          /*"j",74*/
    {0, 0, 0},          /*"k",75*/
    {0, 0, 0},          /*"l",76*/
    {0, 0, 0},          /*"m",77*/
    {0, 0, 0},          /*"n",78*/
    {0, 0, 0},          /*"o",79*/
    {0, 0, 0},          /*"p",80*/
    {0, 0, 0},          /*"q",81*/
    {0, 0, 0},          /*"r",82*/
    {0, 0, 0},          /*"s",83*/
    {0, 0, 0},          /*"t",84*/
    {0, 0, 0},          /*"u",85*/
    {0, 0, 0},          /*"v",86*/
    {0, 0, 0},          /*"w",87*/
    {0, 0, 0},          /*"x",88*/
    {0, 0, 0},          /*"y",89*/
    {0, 0, 0},          /*"z",90*/
    {0, 0, 0},          /*"{",91*/
    {0, 0, 0},          /*"|",92*/
    {0, 0, 0},          /*"}",93*/
    {0, 0, 0},          /*"~",94*/

};

const unsigned char ascii_0405[][4] = {
    {0x00, 0x00, 0x00, 0x00}, /*" ",0*/
    {0x00, 0x1d, 0x00, 0x00}, /*"!",1*/
    {0, 0, 0, 0},             /*""",2*/
    {0, 0, 0, 0},             /*"#",3*/
    {0, 0, 0, 0},             /*"$",4*/
    {0x13, 0x04, 0x19, 0x00}, /*"%",5*/
    {0, 0, 0, 0},             /*"&",6*/
    {0x00, 0x18, 0x00, 0x00}, /*"'",7*/
    {0x0e, 0x11, 0x00, 0x00}, /*"(",8*/
    {0x00, 0x11, 0x0e, 0x00}, /*")",9*/
    {0x0a, 0x04, 0x0a, 0x00}, /*"*",10*/
    {0x04, 0x0e, 0x04, 0x00}, /*"+",11*/
    {0x01, 0x02, 0x00, 0x00}, /*",",12*/
    {0x04, 0x04, 0x04, 0x00}, /*"-",13*/
    {0x00, 0x01, 0x00, 0x00}, /*".",14*/
    {0x02, 0x04, 0x08, 0x00}, /*"/",15*/
    {0x1f, 0x11, 0x1f, 0x00}, /*"0",16*/
    {0x09, 0x1f, 0x01, 0x00}, /*"1",17*/
    {0x17, 0x15, 0x1d, 0x00}, /*"2",18*/
    {0x15, 0x15, 0x1f, 0x00}, /*"3",19*/
    {0x1c, 0x04, 0x1f, 0x00}, /*"4",20*/
    {0x1d, 0x15, 0x17, 0x00}, /*"5",21*/
    {0x1f, 0x15, 0x17, 0x00}, /*"6",22*/
    {0x10, 0x10, 0x1f, 0x00}, /*"7",23*/
    {0x1f, 0x15, 0x1f, 0x00}, /*"8",24*/
    {0x1d, 0x15, 0x1f, 0x00}, /*"9",25*/
    {0x00, 0x0a, 0x00, 0x00}, /*":",26*/
    {0x01, 0x0a, 0x00, 0x00}, /*";",27*/
    {0x04, 0x0a, 0x00, 0x00}, /*"<",28*/
    {0x0a, 0x0a, 0x0a, 0x00}, /*"=",29*/
    {0x00, 0x0a, 0x04, 0x00}, /*">",30*/
    {0x10, 0x15, 0x18, 0x00}, /*"?",31*/
    {0, 0, 0, 0},             /*"@",32*/
    {0x0f, 0x14, 0x0f, 0x00}, /*"A",33*/
    {0x1f, 0x15, 0x0e, 0x00}, /*"B",34*/
    {0x0e, 0x11, 0x11, 0x00}, /*"C",35*/
    {0x1f, 0x11, 0x0e, 0x00}, /*"D",36*/
    {0x1f, 0x15, 0x15, 0x00}, /*"E",37*/
    {0x1f, 0x14, 0x14, 0x00}, /*"F",38*/
    {0x0e, 0x15, 0x17, 0x00}, /*"G",39*/
    {0, 0, 0, 0},             /*"H",40*/
    {0, 0, 0, 0},             /*"I",41*/
    {0, 0, 0, 0},             /*"J",42*/
    {0, 0, 0, 0},             /*"K",43*/
    {0, 0, 0, 0},             /*"L",44*/
    {0, 0, 0, 0},             /*"M",45*/
    {0, 0, 0, 0},             /*"N",46*/
    {0, 0, 0, 0},             /*"O",47*/
    {0, 0, 0, 0},             /*"P",48*/
    {0, 0, 0, 0},             /*"Q",49*/
    {0, 0, 0, 0},             /*"R",50*/
    {0, 0, 0, 0},             /*"S",51*/
    {0, 0, 0, 0},             /*"T",52*/
    {0, 0, 0, 0},             /*"U",53*/
    {0, 0, 0, 0},             /*"V",54*/
    {0, 0, 0, 0},             /*"W",55*/
    {0, 0, 0, 0},             /*"X",56*/
    {0, 0, 0, 0},             /*"Y",57*/
    {0, 0, 0, 0},             /*"Z",58*/
    {0, 0, 0, 0},             /*"[",59*/
    {0, 0, 0, 0},             /*"\",60*/
    {0, 0, 0, 0},             /*"]",61*/
    {0, 0, 0, 0},             /*"^",62*/
    {0, 0, 0, 0},             /*"_",63*/
    {0, 0, 0, 0},             /*"`",64*/
    {0, 0, 0, 0},             /*"a",65*/
    {0, 0, 0, 0},             /*"b",66*/
    {0, 0, 0, 0},             /*"c",67*/
    {0, 0, 0, 0},             /*"d",68*/
    {0, 0, 0, 0},             /*"e",69*/
    {0, 0, 0, 0},             /*"f",70*/
    {0, 0, 0, 0},             /*"g",71*/
    {0, 0, 0, 0},             /*"h",72*/
    {0, 0, 0, 0},             /*"i",73*/
    {0, 0, 0, 0},             /*"j",74*/
    {0, 0, 0, 0},             /*"k",75*/
    {0, 0, 0, 0},             /*"l",76*/
    {0, 0, 0, 0},             /*"m",77*/
    {0, 0, 0, 0},             /*"n",78*/
    {0, 0, 0, 0},             /*"o",79*/
    {0, 0, 0, 0},             /*"p",80*/
    {0, 0, 0, 0},             /*"q",81*/
    {0, 0, 0, 0},             /*"r",82*/
    {0, 0, 0, 0},             /*"s",83*/
    {0, 0, 0, 0},             /*"t",84*/
    {0, 0, 0, 0},             /*"u",85*/
    {0, 0, 0, 0},             /*"v",86*/
    {0, 0, 0, 0},             /*"w",87*/
    {0, 0, 0, 0},             /*"x",88*/
    {0, 0, 0, 0},             /*"y",89*/
    {0, 0, 0, 0},             /*"z",90*/
    {0, 0, 0, 0},             /*"{",91*/
    {0, 0, 0, 0},             /*"|",92*/
    {0, 0, 0, 0},             /*"}",93*/
    {0, 0, 0, 0},             /*"~",94*/

};

const unsigned char ascii_0408[][4] = {
    {0x00, 0x00, 0x00, 0x00}, /*" ",0*/
    {0x00, 0x7D, 0x00, 0x00}, /*"!",1*/
    {0x60, 0x00, 0x60, 0x00}, /*""",2*/
    {0x7F, 0x14, 0x7F, 0x14}, /*"#",3*/
    {0x19, 0x2F, 0x79, 0x26}, /*"$",4*/
    {0x33, 0x34, 0x0B, 0x33}, /*"%",5*/
    {0x0E, 0x79, 0x59, 0x26}, /*"&",6*/
    {0x00, 0x00, 0x60, 0x00}, /*"'",7*/
    {0x00, 0x3E, 0x41, 0x00}, /*"(",8*/
    {0x00, 0x41, 0x3E, 0x00}, /*")",9*/
    {0x1C, 0x3E, 0x1C, 0x00}, /*"*",10*/
    {0x08, 0x3E, 0x08, 0x00}, /*"+",11*/
    {0x00, 0x00, 0x03, 0x00}, /*",",12*/
    {0x08, 0x08, 0x08, 0x08}, /*"-",13*/
    {0x00, 0x01, 0x00, 0x00}, /*".",14*/
    {0x01, 0x06, 0x18, 0x60}, /*"/",15*/
    {0x3E, 0x41, 0x41, 0x3E}, /*"0",16*/
    {0x00, 0x21, 0x7F, 0x01}, /*"1",17*/
    {0x23, 0x45, 0x49, 0x31}, /*"2",18*/
    {0x22, 0x49, 0x49, 0x36}, /*"3",19*/
    {0x0E, 0x32, 0x7F, 0x02}, /*"4",20*/
    {0x79, 0x49, 0x49, 0x46}, /*"5",21*/
    {0x3E, 0x49, 0x49, 0x26}, /*"6",22*/
    {0x60, 0x47, 0x48, 0x70}, /*"7",23*/
    {0x36, 0x49, 0x49, 0x36}, /*"8",24*/
    {0x32, 0x49, 0x49, 0x3E}, /*"9",25*/
    {0x00, 0x36, 0x36, 0x00}, /*":",26*/
    {0x01, 0x12, 0x00, 0x00}, /*";",27*/
    {0x08, 0x14, 0x22, 0x41}, /*"<",28*/
    {0x14, 0x14, 0x14, 0x14}, /*"=",29*/
    {0x41, 0x22, 0x14, 0x08}, /*">",30*/
    {0x20, 0x45, 0x48, 0x30}, /*"?",31*/
    {0x38, 0x47, 0x49, 0x3F}, /*"@",32*/
    {0x3F, 0x44, 0x44, 0x3F}, /*"A",33*/
    {0x7F, 0x49, 0x49, 0x36}, /*"B",34*/
    {0x3E, 0x41, 0x41, 0x22}, /*"C",35*/
    {0x7F, 0x41, 0x41, 0x3E}, /*"D",36*/
    {0x7F, 0x49, 0x49, 0x41}, /*"E",37*/
    {0x7F, 0x48, 0x48, 0x40}, /*"F",38*/
    {0x3E, 0x41, 0x49, 0x2E}, /*"G",39*/
    {0x7F, 0x08, 0x08, 0x7F}, /*"H",40*/
    {0x41, 0x7F, 0x41, 0x00}, /*"I",41*/
    {0x06, 0x01, 0x01, 0x7E}, /*"J",42*/
    {0x7F, 0x08, 0x14, 0x63}, /*"K",43*/
    {0x7F, 0x01, 0x01, 0x01}, /*"L",44*/
    {0x7F, 0x30, 0x30, 0x7F}, /*"M",45*/
    {0x7F, 0x30, 0x0C, 0x7F}, /*"N",46*/
    {0x7F, 0x41, 0x41, 0x7F}, /*"O",47*/
    {0x7F, 0x48, 0x48, 0x30}, /*"P",48*/
    {0x3E, 0x45, 0x43, 0x3F}, /*"Q",49*/
    {0x7F, 0x4C, 0x4A, 0x31}, /*"R",50*/
    {0x32, 0x49, 0x49, 0x26}, /*"S",51*/
    {0x40, 0x7F, 0x40, 0x40}, /*"T",52*/
    {0x7E, 0x01, 0x01, 0x7E}, /*"U",53*/
    {0x78, 0x07, 0x07, 0x78}, /*"V",54*/
    {0x7F, 0x06, 0x06, 0x7F}, /*"W",55*/
    {0x63, 0x1C, 0x1C, 0x63}, /*"X",56*/
    {0x10, 0x0F, 0x10, 0x60}, /*"Y",57*/
    {0x43, 0x4D, 0x59, 0x61}, /*"Z",58*/
    {0x00, 0x7F, 0x41, 0x00}, /*"[",59*/
    {0x18, 0x06, 0x01, 0x00}, /*"\",60*/
    {0x00, 0x41, 0x7F, 0x00}, /*"]",61*/
    {0x20, 0x40, 0x20, 0x00}, /*"^",62*/
    {0x01, 0x01, 0x01, 0x01}, /*"_",63*/
    {0x00, 0x60, 0x00, 0x00}, /*"`",64*/
    {0x02, 0x0D, 0x0D, 0x0F}, /*"a",65*/
    {0x7F, 0x09, 0x09, 0x06}, /*"b",66*/
    {0x06, 0x09, 0x09, 0x09}, /*"c",67*/
    {0x06, 0x09, 0x09, 0x7F}, /*"d",68*/
    {0x06, 0x0D, 0x0D, 0x04}, /*"e",69*/
    {0x08, 0x1F, 0x28, 0x08}, /*"f",70*/
    {0x04, 0x0A, 0x0A, 0x0F}, /*"g",71*/
    {0x7F, 0x08, 0x0F, 0x00}, /*"h",72*/
    {0x00, 0x17, 0x00, 0x00}, /*"i",73*/
    {0x00, 0x00, 0x17, 0x00}, /*"j",74*/
    {0x7F, 0x04, 0x0A, 0x09}, /*"k",75*/
    {0x01, 0x7F, 0x01, 0x00}, /*"l",76*/
    {0x0F, 0x08, 0x0F, 0x0F}, /*"m",77*/
    {0x0F, 0x08, 0x08, 0x07}, /*"n",78*/
    {0x06, 0x09, 0x09, 0x06}, /*"o",79*/
    {0x0F, 0x09, 0x09, 0x06}, /*"p",80*/
    {0x06, 0x09, 0x09, 0x0F}, /*"q",81*/
    {0x0F, 0x04, 0x08, 0x08}, /*"r",82*/
    {0x05, 0x0D, 0x0B, 0x0A}, /*"s",83*/
    {0x08, 0x1F, 0x09, 0x00}, /*"t",84*/
    {0x0E, 0x01, 0x01, 0x0F}, /*"u",85*/
    {0x0C, 0x03, 0x03, 0x0C}, /*"v",86*/
    {0x03, 0x0E, 0x03, 0x0C}, /*"w",87*/
    {0x09, 0x06, 0x06, 0x09}, /*"x",88*/
    {0x0C, 0x02, 0x02, 0x0F}, /*"y",89*/
    {0x09, 0x0B, 0x0D, 0x09}, /*"z",90*/
    {0x08, 0x77, 0x41, 0x00}, /*"{",91*/
    {0x00, 0x7F, 0x00, 0x00}, /*"|",92*/
    {0x41, 0x77, 0x08, 0x00}, /*"}",93*/
    {0x00, 0x40, 0x80, 0x40}, /*"~",94*/

};

#endif //__FONT_H__