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
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

/********************************************************/

void led_show();
void timer_callback();
s8 get_time();
void show_real_time();

/********************************************************/

// WiFi
u8 g_bConnecting = 0;

// time_scheduling
hw_timer_t *timer_real_time = NULL;
volatile u8 time_scheduling = 0;

// Real-time
time_t time_base = 0;
time_t time_offset = 0;
u32 update_time_cnt = 0;
const u32 UPTIME_CNT_LIMIT = 86400; // 24h * 60 * 60

const char api_weather_lives[] = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=59c271adfce4a653f2629d54de1ac514";
const char api_weather_forecast[] = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=59c271adfce4a653f2629d54de1ac514&extensions=all";
const char api_time[] = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";

/********************************************************/

// Define the array of leds
char led_show_text[20];
CRGB leds[LED_NUM];
u32 leds_data[LED_COL][LED_ROW];

/********************************************************/

void setup()
{
  Serial.begin(115200);

  // led init
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_NUM); // GRB ordering is typical
  FastLED.setBrightness(5);

  // globle timer init
  timer_real_time = timerBegin(0, 80, true);
  timerAttachInterrupt(timer_real_time, &timer_callback, true);
  timerAlarmWrite(timer_real_time, 1000000, true); // us

  printf("这是一个WiFi yy13093z\n");
  WiFi.begin("这是一个WiFi", "yy13093z");
}

void loop()
{
  // Turn the LED on, then pause
  // for (u16 i = 0; i < LED_COL; i++)
  // {
  //   for (u16 j = 0; j < LED_ROW; j++)
  //   {
  //     leds_data[i][j] = 0;
  //   }
  // }

  if (WiFi.status() == WL_CONNECTED)
  {
    // LCD_Fill(0, 0, LCD_W, LCD_H, 0x79C2);
    // LCD_WifiConnectOK();
    // delay(1000);
    // LCD_Fill(0, 0, LCD_W, LCD_H, BG_COLOR);
    printf("wifi connect ok!\n");
    g_bConnecting = 1;
    time_scheduling = 1;
    update_time_cnt = UPTIME_CNT_LIMIT;
    // update_weather_count = UPWEATHER_CNT_LIMIT;
    // update_time_count = UPTIME_CNT_LIMIT - 3;
    // update_weather2_count = UPTIME_CNT_LIMIT - 6;
    // u8 aa[] = "天气预报";
    // LCD_Fill(0, 0, LCD_W, LCD_H, 0x0000);
    // LCD_ShowChinese(20, 50, aa, 0x1234, 0x4567, 16);
    timerAlarmEnable(timer_real_time);
    while (1)
    {
      if (time_scheduling)
      {
        time_scheduling = 0;
        // sprintf(led_show_text, "%04d:", update_time_cnt);
        // led_show_char(leds_data, 1, 2, led_show_text, LED_SZIE_45, (u32)CRGB::SkyBlue);
        show_real_time();
        led_show();
      }
    }
  }
}

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

void timer_callback()
{
  time_offset++;
  update_time_cnt++;
  time_scheduling = 1;
}

s8 get_time()
{
  s8 ok = 0;
  HTTPClient http; // 声明HTTPClient对象
  StaticJsonDocument<150> timeDoc;

  http.begin(api_time);      // 准备启用连接
  int httpCode = http.GET(); // 发起GET请求

  if (httpCode > 0) // 如果状态码大于0说明请求过程无异常
  {
    if (httpCode == HTTP_CODE_OK) // 请求被服务器正常响应，等同于httpCode == 200
    {
      char buf[200];
      http.getString().toCharArray(buf, sizeof(buf));             // 读取服务器返回的响应正文数据, 如果正文数据很多该方法会占用很大的内存
      DeserializationError error = deserializeJson(timeDoc, buf); // 反序列化JSON数据
      if (error)                                                  // 检查反序列化是否成功
      {
        printf(error.c_str());
        ok = -1;
      }
      else
      {
        const char *s = timeDoc["data"]["t"];
        char ss[10];
        for (u8 i = 0; i < 10; i++)
        {
          ss[i] = s[i];
        }
        time_base = atoi(ss) + 8 * 60 * 60; // 中国GMT+8, +1 delay
      }
    }
    else
      ok = -2;
  }
  else
  {
    printf("[HTTP] GET Time failed, error: %s\n", http.errorToString(httpCode).c_str());
    ok = -3;
  }
  http.end(); // 结束当前连接
  return ok;
}

void show_real_time()
{
  if (update_time_cnt >= UPTIME_CNT_LIMIT)
  {
    if (!g_bConnecting)
    {
      WiFi.begin();
      g_bConnecting = 1;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      g_bConnecting = 0;
      update_time_cnt = 0;
      // {
      //   LCD_Fill(20, 88, LCD_W, LCD_H, BG_COLOR);
      //   for (u8 i = 1; i < 4; i++)
      //   {
      //     String date = forecastInfo["casts"][i]["date"];
      //     String dayweather = forecastInfo["casts"][i]["dayweather"];
      //     String daytemp = forecastInfo["casts"][i]["daytemp"];
      //     date = date.substring(5);
      //     daytemp += "°";
      //     LCD_ShowString(75 + i * 13, 20, date.c_str(), 0x5555, 0x8888, 12, 1);
      //     LCD_ShowChinese(75 + i * 13, 60, dayweather.c_str(), 0x5555, 0x8888, 12, 1);
      //     LCD_ShowString(75 + i * 13, 100, daytemp.c_str(), 0x5555, 0x8888, 12, 1);
      //   }
      // }

      printf("get time\n");
      // update_time_count = 0;
      time_offset = 0;
      get_time();
      if (WiFi.disconnect(true))
        printf("close wifi ok\n");
      else
        printf("close wifi ng\n");
    }

    else if (WiFi.status() == WL_CONNECT_FAILED)
    {
      g_bConnecting = 0;
      update_time_cnt = 0;
    }
  }

  // printf("show time start\n");
  static time_t real_t;
  static struct tm *p_time;

  real_t = time_base + time_offset;
  p_time = localtime(&real_t);
  strftime(led_show_text, sizeof(led_show_text), "%H:%M:%S", p_time);
  // led_show_char(leds_data, 1, 2, led_show_text, LED_SZIE_45, (u32)CRGB::SkyBlue);
  led_show_char(leds_data, 0, 0, led_show_text, LED_SZIE_48, (u32)CRGB::SkyBlue);
  // printf("show time ok\n");
  // printf("time_base: %d, time_offset: %d\n", time_base, time_offset);
}