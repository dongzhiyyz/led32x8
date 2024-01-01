/**
 *  y 7
 *   ↑
 *   |
 *   |
 *   |               32 * 8
 *   |
 *   |
 *   |
 * (0,0)----------------------------------> x 31
 *
 */

#include "font.h"

const char api_weather_lives[] = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=59c271adfce4a653f2629d54de1ac514";
const char api_weather_forecast[] = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=59c271adfce4a653f2629d54de1ac514&extensions=all";
const char api_time[] = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";

// Define the array of leds
CRGB leds[LED_NUM];
u32 leds_data[LED_COL][LED_ROW];

// void

u8 text[20];
u8 cnt = 0;

void led_show()
{
  u16 led_cnt = 0;
  for (u16 i = 0; i < LED_COL; i++)
  {
    for (u16 j = 0; j < LED_ROW; j++)
    {
      if (i & 0x1)
      {
        if (LED_ROTATION)
          leds[LED_NUM - 1 - led_cnt] = leds_data[i][LED_ROW - 1 - j];
        else
          leds[led_cnt] = leds_data[i][j];
      }
      else
      {
        if (LED_ROTATION)
          leds[LED_NUM - 1 - led_cnt] = leds_data[i][j];
        else
          leds[led_cnt] = leds_data[i][LED_ROW - 1 - j];
      }
      led_cnt++;
    }
  }
  FastLED.show();
}

void setup()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_NUM); // GRB ordering is typical
  FastLED.setBrightness(5);
}

void loop()
{
  // Turn the LED on, then pause
  for (u16 i = 0; i < LED_COL; i++)
  {
    for (u16 j = 0; j < LED_ROW; j++)
    {
      leds_data[i][j] = 0;
    }
  }

  sprintf((char *)&text, "%02d:%02d:%02d", cnt, cnt, cnt);
  led_show_char(leds_data, 1, 2, text, LED_SZIE_45, (u32)CRGB::SkyBlue);

  led_show();
  delay(1000);



  cnt++;
  if (cnt > 59)
    cnt = 0;

}