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
#include "soc/rtc_wdt.h" //设置看门狗用

/********************************************************/

void led_show();
void sys_timer_isr();
s8 get_time();
void show_real_time();

/********************************************************/
typedef enum
{
  sys_wifi_start,
  sys_wifi_connecting,
  sys_wifi_stadus,
  sys_real_time,
  sys_fft,

  sys_err = 0xff,
} SystemMode;

/********************************************************/

const u32 SYS_FREQ = 1000; // 10kHz
hw_timer_t *sys_timer = NULL;
volatile u8 sys_scheduling = 0;
volatile u32 sys_cnt = 0;
SystemMode sys_mode = sys_wifi_start;
SystemMode sys_pre_mode = sys_mode;

// WiFi
u8 wifi_connecting = 0;

// time_scheduling
// hw_timer_t *timer_real_time = NULL;
// volatile u8 time_scheduling = 0;

// Real-time
time_t time_base = 0;
time_t time_offset = 0;
u32 get_net_time_cnt = 0;                 // 获取网络时间cnt
const u32 GET_NET_TIME_CNT_LIMIT = 86400; // 24h * 60 * 60

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

  rtc_wdt_protect_off(); // 看门狗写保护关闭 关闭后可以喂狗
  // rtc_wdt_protect_on();    // 看门狗写保护打开 打开后不能喂狗
  // rtc_wdt_disable();       // 禁用看门狗
  rtc_wdt_enable();                        // 启用看门狗
  rtc_wdt_set_time(RTC_WDT_STAGE0, 10000); // 设置看门狗超时 10s.则reset重启

  // real timer init
  // timer_real_time = timerBegin(1, 80, true);
  // timerAttachInterrupt(timer_real_time, &timer_isr, true);
  // timerAlarmWrite(timer_real_time, 1000000, true); // us

  // sys timer init
  sys_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(sys_timer, &sys_timer_isr, true);
  timerAlarmWrite(sys_timer, 1000000 / SYS_FREQ, true); // 100us 10kHz
  timerAlarmEnable(sys_timer);
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

  while (sys_scheduling)
  {
    rtc_wdt_feed(); // 喂狗函数
    sys_scheduling = 0;
    if (sys_pre_mode != sys_mode)
    {
      printf("sys_mode change!, now mode is: %d\n", sys_mode);
      sys_pre_mode = sys_mode;
      sys_cnt = 0;
    }

    switch (sys_mode)
    {
    case sys_wifi_start:
      printf("这是一个WiFi yy13093z\n");
      wifi_connecting = 1;
      WiFi.begin("这是一个WiFi", "yy13093z");
      sys_mode = sys_wifi_connecting;
      break;

    case sys_wifi_connecting:
      switch (WiFi.status())
      {
      case WL_CONNECTED:
        log_printf("wifi connect ok!\n");
        sys_mode = sys_wifi_stadus;
        wifi_connecting = 0;
        break;

      case WL_CONNECT_FAILED:
        printf("wifi connect ng!\n");
        sys_mode = sys_wifi_stadus;
        wifi_connecting = 0;
        break;
      }

      if (sys_cnt > SYS_FREQ * 10) // time out 10s
      {
        printf("wifi connect time out!\n");
        sys_mode = sys_wifi_stadus;
        WiFi.disconnect(true);
        wifi_connecting = 0;
      }
      break;

    case sys_wifi_stadus:
      if (WiFi.status() == WL_CONNECTED)
      {
        // show ok log
      }
      else
      {
        // show ng log
      }
      if (sys_cnt > SYS_FREQ * 2)
      {
        if (WiFi.status() == WL_CONNECTED)
        {
          get_net_time_cnt = GET_NET_TIME_CNT_LIMIT;
          sys_mode = sys_real_time;
        }
        else
          sys_mode = sys_fft;
      }
      break;

    case sys_real_time:
      if (sys_cnt > SYS_FREQ * 1)
      {
        time_offset++;
        get_net_time_cnt++;
        sys_cnt = 0;
        show_real_time();
        led_show();
      }
      break;

    case sys_fft:

      break;

    case sys_err:
      printf("sys err! restart");
      ESP.restart();
      break;

    default:
      sys_mode = sys_err;
      break;
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

void ARDUINO_ISR_ATTR sys_timer_isr()
{
  sys_cnt++;
  sys_scheduling++;
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
  if (get_net_time_cnt >= GET_NET_TIME_CNT_LIMIT)
  {
    if (!wifi_connecting)
    {
      WiFi.begin();
      wifi_connecting = 1;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      time_offset = 0;
      wifi_connecting = 0;
      get_net_time_cnt = 0;

      printf("get time\n");
      get_time();
      if (WiFi.disconnect(true))
        printf("close wifi ok\n");
      else
        printf("close wifi ng\n");
    }
    else if (WiFi.status() == WL_CONNECT_FAILED)
    {
      wifi_connecting = 0;
      get_net_time_cnt = 0;
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