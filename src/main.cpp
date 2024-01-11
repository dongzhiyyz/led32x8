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

#include "api_id.h"
#include "font.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include "soc/rtc_wdt.h" //设置看门狗用
#include "arduinoFFT.h"

/********************************************************/

void sys_timer_isr();
bool sys_interval_decision(u16 cycle_ms, u16 sys_ms);
void led_show();
void led_clear();
void show_real_time();
s8 get_real_time();
wifi_sts_t wifi_connect();
bool wifi_disconnect();
u32 int2rgb(u32 value);
void adc_timer_isr();
void drawBar(u16 idx, s16 value, u8 *flag); // 绘制函数，按序号和幅度值绘制条形块

/********************************************************/

const      u32 SYS_FREQ      = 1000;            // 1kHz
hw_timer_t *sys_timer        = NULL;
volatile   u8 sys_scheduling = 0;
volatile   u32 sys_cnt       = 0;
sys_mode_t sys_mode          = SYS_WIFI_START;
sys_mode_t sys_pre_mode      = sys_mode;

const u16 SYS_INTERVAL_0 = 600;
const u16 SYS_INTERVAL_OFS_0 = SYS_INTERVAL_0 >> 1;

// WiFi
char wifi_ssid[32] = "这是一个WiFi";
char wifi_pass[32] = "yy13093z";

// time_scheduling
// hw_timer_t *timer_real_time = NULL;
// volatile u8 time_scheduling = 0;

// Real-time
time_t time_base                  = 0;
time_t time_offset                = 0;
u32    get_net_time_cnt           = 0;      // 获取网络时间cnt
const  u32 GET_NET_TIME_CNT_LIMIT = 86400;  // 24h * 60 * 60

const char api_weather_lives[]    = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=";
const char api_weather_forecast[] = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=&extensions=all";
const char api_time[]             = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";


// Define the array of leds
char led_show_text[20];
CRGB leds[LED_NUM];
u32 leds_data[LED_COL][LED_ROW];

// ADC FFT
const u8 ADC_CHANNEL  = 4;
const u16 ADC_FREQ    = 8000;                 // Hz, 声音采样频率
const u16 ADC_SAMPLES = 4 * LED_COL;          // 采样点数，必须为2的整数次幂
const float FFT_FPS   = SYS_FREQ * 1.0 / 30;  // 30fps

double fftReal[ADC_SAMPLES]; // FFT采样输入样本数组
double fftImag[ADC_SAMPLES]; // FFT运算输出数组
hw_timer_t *adc_timer = NULL;
arduinoFFT FFT        = arduinoFFT(fftReal, fftImag, ADC_SAMPLES, ADC_FREQ);  // 创建FFT对象
u16        adc_cnt    = 0;
u8         fft_flag   = 0;

/********************************************************/

void setup()
{
  Serial.begin(115200);

  // led init
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_NUM); // GRB ordering is typical
  FastLED.setBrightness(5);

  // 看门狗 init
  rtc_wdt_protect_off(); // 看门狗写保护关闭 关闭后可以喂狗
  // rtc_wdt_protect_on();    // 看门狗写保护打开 打开后不能喂狗
  // rtc_wdt_disable();       // 禁用看门狗
  rtc_wdt_enable();                       // 启用看门狗
  rtc_wdt_set_time(RTC_WDT_STAGE0, 1000); // 设置看门狗超时 1s.则reset重启

  // adc init
  pinMode(ADC_CHANNEL, INPUT); // 初始化麦克风接口为输入模式，表示读取麦克风数据
  adc_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(adc_timer, &adc_timer_isr, true);
  timerAlarmWrite(adc_timer, 1000000 / ADC_FREQ, true);
  timerAlarmDisable(adc_timer);

  // sys timer init
  sys_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(sys_timer, &sys_timer_isr, true);
  timerAlarmWrite(sys_timer, 1000000 / SYS_FREQ, true);
  timerAlarmEnable(sys_timer);
}

void loop()
{
  while (sys_scheduling)
  {
    rtc_wdt_feed(); // 喂狗函数
    sys_scheduling = 0;

    if (sys_pre_mode != sys_mode)
    {
      printf("sys mode change,running mode: %d\n", sys_mode);
      sys_pre_mode = sys_mode;
      sys_cnt = 0;
    }

    switch (sys_mode)
    {
    case SYS_WIFI_START:
    {
      wifi_connect();
      sys_mode = SYS_WIFI_CONNECTING;
      break;
    }

    case SYS_WIFI_CONNECTING:
    {
      switch (wifi_connect())
      {
      case WIFI_CONNECTING:
        // show logo
        if (sys_interval_decision(SYS_INTERVAL_0, 0))
        {
          led_show_pattern(leds_data, &pattern_wifi_connecting3);
          led_show();
        }
        else if (sys_interval_decision(SYS_INTERVAL_0, 200))
        {
          led_show_pattern(leds_data, &pattern_wifi_connecting1);
          led_show();
        }
        else if (sys_interval_decision(SYS_INTERVAL_0, 400))
        {
          led_show_pattern(leds_data, &pattern_wifi_connecting2);
          led_show();
        }
        break;

      case WIFI_CONNECTED:
      case WIFI_CONNECT_FAILED:
        sys_mode = SYS_WIFI_STADUS;
        break;

      default:
        break;
      }

      if (sys_cnt > SYS_FREQ * 10) // time out 10s
      {
        printf("wifi connect time out!\n");
        sys_mode = SYS_WIFI_STADUS;
        wifi_disconnect();
      }
      break;
    }

    case SYS_WIFI_STADUS:
    {
      if (wifi_connect() == WIFI_CONNECTED)
      {
        // show ok logo
        if (sys_interval_decision(SYS_INTERVAL_0, 0))
        {
          led_show_pattern(leds_data, &pattern_wifi_connect_ok);
          led_show();
        }
        else if (sys_interval_decision(SYS_INTERVAL_0, SYS_INTERVAL_OFS_0))
        {
          led_clear();
        }
      }
      else
      {
        // show ng logo
        if (sys_interval_decision(SYS_INTERVAL_0, 0))
        {
          led_show_pattern(leds_data, &pattern_wifi_connect_ng);
          led_show();
        }
        else if (sys_interval_decision(SYS_INTERVAL_0, SYS_INTERVAL_OFS_0))
        {
          led_clear();
        }
      }

      if (sys_cnt > SYS_FREQ * 2.5)
      {
        led_clear();
        if (wifi_connect() == WIFI_CONNECTED)
        {
          get_net_time_cnt = GET_NET_TIME_CNT_LIMIT;
          sys_mode = SYS_REAL_TIME;
        }
        else
          sys_mode = SYS_FFT;
      }

      break;
    }

    case SYS_REAL_TIME:
    {
      if (sys_cnt >= SYS_FREQ * 1)
      {
        sys_cnt = 0;
        time_offset++;
        get_net_time_cnt++;
        show_real_time();
        led_show();
      }
      break;
    }

    case SYS_FFT:
    {
      if (adc_cnt >= ADC_SAMPLES && sys_cnt >= FFT_FPS)
      {
        /*FFT运算*/
        FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
        FFT.Compute(FFT_FORWARD);                        /* Compute FFT */
        FFT.ComplexToMagnitude();                        /* Compute magnitudes */

        fill_rainbow(leds, LED_NUM, 0, 4); // 设置彩虹渐变，先填充满，然后根据取值大小填充黑色，表示熄灭灯
        for (int i = 0; i < LED_COL; i++)
          drawBar(i, (fftReal[i * 4 + 0] + fftReal[i * 4 + 1] + fftReal[i * 4 + 2] + fftReal[i * 4 + 3]) / 4 / 200, &fft_flag); // 选取频谱中取平均后的4个值,传递时间标志到绘制函数

        FastLED.show();
        adc_cnt = 0;
        break;
      }
    }

    case SYS_ERR:
    {
      printf("sys err! restart");
      ESP.restart();
      break;
    }

    default:
      sys_mode = SYS_ERR;
      break;
    }
  }
}

void ARDUINO_ISR_ATTR sys_timer_isr()
{
  sys_cnt++;
  sys_scheduling++;
}

bool sys_interval_decision(u16 cycle_ms, u16 ofs_ms)
{
  return ((sys_cnt * 1000) % (SYS_FREQ * cycle_ms) == ofs_ms * 1000);
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

void led_clear()
{
  for (u8 i = 0; i < LED_COL; i++)
  {
    for (u8 j = 0; j < LED_ROW; j++)
    {
      leds_data[i][j] = 0;
    }
  }
  led_show();
}

void show_real_time()
{
  if (get_net_time_cnt >= GET_NET_TIME_CNT_LIMIT)
  {
    wifi_sts_t res = wifi_connect();
    if (res == WIFI_CONNECTED)
    {
      get_net_time_cnt = 0;
      printf("get network time...\n");
      if (!get_real_time())
      {
        time_offset = 0;
        printf("get network time ok!\n");
      }
      wifi_disconnect();
    }

    else if (res == WIFI_CONNECT_FAILED)
    {
      get_net_time_cnt = 0;
    }
  }

  // printf("show time start\n");
  static time_t real_t;
  static struct tm *p_time;

  real_t = time_base + time_offset;
  p_time = localtime(&real_t);
  strftime(led_show_text, sizeof(led_show_text), "%H:%M:%S", p_time);
  led_show_char(leds_data, 2, 2, led_show_text, LED_SZIE_45, int2rgb((u32)(time_offset * 194)));
  printf("real time %s\n", led_show_text);

  // led_show_char(leds_data, 0, 0, led_show_text, LED_SZIE_48, (u32)CRGB::MediumBlue);
  // printf("show time ok\n");
  // printf("time_base: %d, time_offset: %d\n", time_base, time_offset);
}

s8 get_real_time()
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
        time_base = atoi(ss) + 8 * 60 * 60; // 中国GMT+8
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

wifi_sts_t wifi_connect()
{
  static bool connecting;

  if (!connecting && WiFi.status() != WL_CONNECTED)
  {
    connecting = 1;
    WiFi.begin(wifi_ssid, wifi_pass);
    printf("wifi connect start, ssid = %s\n", wifi_ssid);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    if (connecting)
      printf("wifi connected!\n");
    connecting = 0;
    return WIFI_CONNECTED;
  }

  else if (WiFi.status() == WL_CONNECT_FAILED)
  {
    if (connecting)
      printf("wifi connect ng!\n");
    connecting = 0;
    return WIFI_CONNECT_FAILED;
  }

  if (connecting)
    return WIFI_CONNECTING;

  return WIFI_IDLE;
}

bool wifi_disconnect()
{
  bool res = WiFi.disconnect(true);
  if (res)
    printf("close wifi ok\n");
  else
    printf("close wifi ng\n");

  return res;
}

u32 int2rgb(u32 value)
{
  value = value & 0xffffff;
  double hue = (double)value / 16777215.0;
  double saturation = 1.0;
  double brightness = 1.0;

  // HSV to RGB conversion
  double c = brightness * saturation;
  double x = c * (1 - fabs(fmod(hue * 6, 2) - 1));
  double m = brightness - c;

  double red, green, blue;
  if (hue < 1.0 / 6.0)
  {
    red = c;
    green = x;
    blue = 0;
  }
  else if (hue < 2.0 / 6.0)
  {
    red = x;
    green = c;
    blue = 0;
  }
  else if (hue < 3.0 / 6.0)
  {
    red = 0;
    green = c;
    blue = x;
  }
  else if (hue < 4.0 / 6.0)
  {
    red = 0;
    green = x;
    blue = c;
  }
  else if (hue < 5.0 / 6.0)
  {
    red = x;
    green = 0;
    blue = c;
  }
  else
  {
    red = c;
    green = 0;
    blue = x;
  }

  return (u32)((red + m) * 255) << 16 | (u32)((green + m) * 255) << 8 | (u32)((blue + m) * 255);
}

void adc_timer_isr()
{
  if (adc_cnt >= ADC_SAMPLES)
    return;

  fftReal[adc_cnt] = analogRead(ADC_CHANNEL); // 读取模拟值，信号采样
  fftImag[adc_cnt] = 0;
  adc_cnt++;
}

void drawBar(u16 idx, s16 value, u8 *flag) // 绘制函数，按序号和幅度值绘制条形块
{
  static s16 volume[8]; // 保存下降数据
  constrain(value, 0, 8);   // 幅度限制在0-8范围内

  if (volume[idx] < value) // 采集到的数据比之前大则更新，实现上冲效果
    volume[idx] = value;

  if (idx % 2)
  { // 余2运算判断序号是否为奇数
    for (int i = 0; i < 8 - volume[idx]; i++)
      leds[idx * 8 + i] = CRGB::Black;
  }
  else
  {
    for (int i = volume[idx]; i < 8; i++)
      leds[idx * 8 + i] = CRGB::Black;
  }

  if (*flag)
  {
    volume[idx] -= 1; // 达到时间则减小1，表示下落
    if (idx == 7)
      *flag = 0; // 第0-7列均更新完毕则清除标记
  }
}

// void loop1()
// {
//   static uint32_t t = 0, dt = 70;
//   static uint8_t flag = 0;

//   FastLED.show(); // 显示灯条

//   if ((millis() - t) > dt)
//   {               // 读取时间，判断是否达到掉落时长
//     flag = 1;     // 达到则标记为1
//     t = millis(); // 更新时间
//   }
// }
