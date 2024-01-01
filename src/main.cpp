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

#include "main.h"
#include "font.h"

// Define the array of leds
CRGB leds[NUM_LEDS];
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
        leds[led_cnt] = leds_data[i][j];
      else
        leds[led_cnt] = leds_data[i][LED_ROW - 1 - j];
      led_cnt++;
    }
  }
  FastLED.show();
}

void setup()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
  FastLED.setBrightness(5);
}

void loop()
{
  // Turn the LED on, then pause
  for (u16 i = 0; i < LED_COL; i++)
  {
    for (u16 j = 0; j < LED_ROW; j++)
    {
      leds_data[i][j] = (u32)CRGB::Green;
    }
  }

  // sprintf((char *)&text, "%d:%d", cnt, cnt);
  // led_show_char(leds_data, 0, 0, text, 3);

  led_show();
  delay(200);

  for (u16 i = 0; i < LED_COL; i++)
  {
    for (u16 j = 0; j < LED_ROW; j++)
    {
      leds_data[i][j] = (u32)CRGB::Red;
    }
  }
  led_show();
  delay(200);

  cnt++;

  // for (u16 i = 0; i < NUM_LEDS; i++)
  // {

  //   if ((i >> 3) & 0x01)
  //     // leds[i] = leds_data[i];
  //     leds[i] = leds_data[NUM_LEDS - i - 1];
  //   else
  //     // leds[((i >> 3) + 1) * 8 - (i % 8) - 1] = leds_data[i];
  //     leds[((i >> 3) + 1) * 8 - (i % 8) - 1] = leds_data[NUM_LEDS - i - 1];
  //   // leds[NUM_LEDS - i - 1] = leds_data[i];
  // }

  // Now turn the LED off, then pause
  // for (u16 i = 0; i < NUM_LEDS; i++)
  // {
  //   leds[i] = CRGB::Black;
  // }
  // FastLED.show();
  // delay(500);

  // fill_rainbow(leds,NUM_LEDS,0,1);
  // FastLED.show();
  // delay(500);
}