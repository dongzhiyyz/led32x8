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
#include <AHT20.h>
#include <EEPROM.h>



/********************************************************/
/********************************************************/



void sys_mode_change();
void sys_freq_change(u16 freq);
void sysmode_wifi_connecting();
void sysmode_wifi_stadus();
void sysmode_real_time();
void sysmode_time_weather();
void sysmode_fft();
void sysmode_rain();
void sysmode_temp_hum();
// void sysmode_test();

void IRAM_ATTR sys_timer_isr();
void IRAM_ATTR real_time_timer_isr();
void IRAM_ATTR key_press_isr();
bool sys_interval(u16 cycle_ms, u16 sys_ms = 0);
void led_refresh();
void led_clear(u8 x0 = 0, u8 y0 = 0, u8 x1 = LED_COL, u8 y1 = LED_ROW);
u32 real_time_final();
void get_net_time();
u8 http_time();
wifi_sts_t wifi_connect();
bool wifi_disconnect();
u32 int2rgb(u32 value);
void fft_draw_bar(u16 idx, s16 value, u8 *flag); // 绘制函数，按序号和幅度值绘制条形块
void pattern_move(u8 x0, u8 y0, u8 x1, u8 y1, move_dir_t dir, u8 loop = 0);



/********************************************************/
/********************************************************/



#define SYS_DEBUG1 1

#define dbg_reset digitalWrite(12, 0)
#define debug_set digitalWrite(12, 1)

#if (SYS_DEBUG1 == 0)
s8         sys_mode                 = SYS_WIFI_START;
#else
s8         sys_mode                 = SYS_TEST;
#endif
s8         sys_mode_pre             = sys_mode;
u8         sys_mode_change_init     = 0;
const      u16 FREQ_SYS             = 1000;      // 1kHz
u16        sys_freq_cur             = FREQ_SYS;
hw_timer_t *sys_timer               = NULL;
vu8        DRAM_ATTR sys_scheduling = 0;
vu32       DRAM_ATTR sys_cnt        = 0;

s8    sys_test_cnt           = 0;
const u16 SYS_INTERVAL_0     = 450;
const u16 SYS_INTERVAL_OFS_0 = SYS_INTERVAL_0 >> 1;

// user key
const u8 PIN_ADD_KEY               = 32;
const u8 PIN_SUB_KEY               = 33;
u8    sys_key_cnt                  = 1;
u8    sys_key_cnt_pre              = 1;
vu16  DRAM_ATTR sys_key_press_cnt  = 0;
u8    DRAM_ATTR sys_key_press_init = 0;

// Real-time
const u16 EEPROM_COLOR_OFS_REG   = 0x00;
const u32 EEPROM_COLOR_OFS_LIMIT = 60 * 20;  //20min
const u32 GET_NET_TIME_CNT_LIMIT = 86400;    // 24h * 60 * 60

hw_timer_t *real_time_timer = NULL;  // 获取网络时间cnt
time_t     time_base        = 0;
time_t     time_offset      = 0;
u32        time_color_ofs   = 0;
u32        get_net_time_cnt = 0;

// const char api_weather_lives[]    = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=";
// const char api_weather_forecast[] = "https://restapi.amap.com/v3/weather/weatherInfo?city=闵行&key=&extensions=all";
const char api_time[] = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";

// Define the array of leds
char led_show_text[20];
CRGB DRAM_ATTR leds[LED_NUM];
u32 DRAM_ATTR leds_data[LED_COL][LED_ROW];

// ADC FFT
const u8 PIN_ADC_CHANNEL = 34;
const u16 FREQ_ADC       = 1000;
const u16 ADC_SAMPLES    = 4 * LED_COL;          // 采样点数，必须为2的整数次幂
const float FFT_FPS      = FREQ_ADC * 1.0 / 30;  // 30fps

double     fftReal[ADC_SAMPLES];                                    // FFT采样输入样本数组
double     fftImag[ADC_SAMPLES];                                    // FFT运算输出数组
arduinoFFT FFT = arduinoFFT(fftReal, fftImag, ADC_SAMPLES, 10000);  // 创建FFT对象

// Temperature + humidity
AHT20 aht20;



/********************************************************/
/********************************************************/




void setup()
{
  pinMode(12, OUTPUT);
  Serial.begin(115200);

  // led init
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_NUM); // GRB ordering is typical
  FastLED.setBrightness(5);

  // 看门狗 init
  // rtc_wdt_protect_off(); // 看门狗写保护关闭 关闭后可以喂狗
  // rtc_wdt_protect_on();    // 看门狗写保护打开 打开后不能喂狗
  // rtc_wdt_disable();       // 禁用看门狗
  // rtc_wdt_enable();                       // 启用看门狗
  // rtc_wdt_set_time(RTC_WDT_STAGE0, 1000); // 设置看门狗超时 1s.则reset重启

  // flash save data
  EEPROM.begin(128);
  time_color_ofs = EEPROM.readUInt(EEPROM_COLOR_OFS_REG);
  printf("time_color_ofs: %u\n", time_color_ofs);

  // adc init
  pinMode(PIN_ADC_CHANNEL, ANALOG); // 初始化麦克风接口为输入模式，表示读取麦克风数据
  // analogReadResolution(12);
  // analogSetAttenuation(ADC_0db);

  Wire.begin(26, 27); // Join I2C bus
  // Check if the AHT20 will acknowledge
  if (aht20.begin() == false)  
    Serial.println("AHT20 not detected. Please check wiring.");

  randomSeed(analogRead(25));
  pinMode(PIN_ADD_KEY, INPUT_PULLDOWN);
  pinMode(PIN_SUB_KEY, INPUT_PULLDOWN);
  attachInterrupt(PIN_ADD_KEY, &key_press_isr, RISING);
  attachInterrupt(PIN_SUB_KEY, &key_press_isr, RISING);

  // realtime timer init
  real_time_timer = timerBegin(1, 80, true);
  timerAttachInterrupt(real_time_timer, &real_time_timer_isr, true);
  timerAlarmWrite(real_time_timer, 1000000, true); // 1s
  timerAlarmEnable(real_time_timer);

  // sys timer init
  sys_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(sys_timer, &sys_timer_isr, true);
  timerAlarmWrite(sys_timer, 1000000 / FREQ_SYS, true);
  timerAlarmEnable(sys_timer);
  sys_freq_cur = 1000000 / timerAlarmRead(sys_timer);
}

void loop()
{
  while (sys_scheduling)
  {

    sys_scheduling = 0;
    
    switch (sys_mode)
    {
#if (SYS_DEBUG1 == 0)
    case SYS_LOGO:

      break;

    case SYS_WIFI_START:
      wifi_connect();
      sys_mode = SYS_WIFI_CONNECTING;
      break;

    case SYS_WIFI_CONNECTING:
      sysmode_wifi_connecting();
      break;

    case SYS_WIFI_STADUS:
      sysmode_wifi_stadus();
      break;

    case SYS_REAL_TIME:
      sysmode_real_time();
      break;

    case SYS_TIME_WEATHER:
      sysmode_time_weather();
      break;

    case SYS_FFT:
      sysmode_fft();
      break;

    case SYS_RAIN:
      sysmode_rain();
      break;

    case SYS_TEMP_HUM:
      sysmode_temp_hum();
      break;
#endif

    case SYS_TEST:
      // sysmode_time_weather();
      // sysmode_fft();
      // sysmode_temp_hum();

      if (sys_interval(400))
      {
        led_show_pattern(leds_data, &pattern_pacman1, 12, 0);
        led_refresh();
      }
      
      else if (sys_interval(400, 200))
      {
        led_show_pattern(leds_data, &pattern_pacman2, 12, 0);
        led_refresh();
      }
      break;

    case SYS_ERR:
      printf("sys err! restart");
      ESP.restart();
      break;

    default:
      sys_mode = SYS_ERR;
      break;
    }

    // rtc_wdt_feed(); // 喂狗函数
    sys_mode_change();
    if (sys_mode_pre != sys_mode)
    {
      printf("sys mode change,running mode: %d\n", sys_mode);
      sys_mode_pre = sys_mode;
      sys_cnt = 0;
    }

  }
}

void sys_mode_change()
{  
  if (!sys_key_press_cnt)
    sys_key_press_cnt = sys_freq_cur >> 5;

  if (sys_key_cnt == sys_key_cnt_pre)
    return;

  printf("sys_key_cnt: %d, sys_key_cnt_pre: %d\n", sys_key_cnt, sys_key_cnt_pre);
  if (sys_key_cnt > sys_key_cnt_pre)
  {
    sys_mode++;
    if (sys_mode > SYS_FINAL)
      sys_mode = SYS_FIRST;
  }
  else
  {
    sys_mode--;
    if (sys_mode < SYS_FIRST)
      sys_mode = SYS_FINAL;
  }

  if (sys_mode == SYS_FFT)
    sys_freq_change(FREQ_ADC);

  if (sys_mode_pre == SYS_FFT)
    sys_freq_change(FREQ_SYS);

  led_clear();
  sys_key_cnt_pre = sys_key_cnt = 1;
  sys_mode_change_init = 0;
}

void sys_freq_change(u16 freq)
{
  if (sys_freq_cur == freq)
  {
    printf("sys freq is same\n");
    return;
  }

  sys_freq_cur = freq;
  timerAlarmWrite(sys_timer, 1000000 / freq, true);
  printf("sys freq change to: %dHz\n", freq);
}

void sysmode_wifi_connecting()
{
  switch (wifi_connect())
  {
  case WIFI_CONNECTING:
    // show logo
    if (sys_interval(SYS_INTERVAL_0, 0))
    {
      led_show_pattern(leds_data, &pattern_wifi_connecting3);
      led_refresh();
    }
    else if (sys_interval(SYS_INTERVAL_0, 150))
    {
      led_show_pattern(leds_data, &pattern_wifi_connecting1);
      led_refresh();
    }
    else if (sys_interval(SYS_INTERVAL_0, 300))
    {
      led_show_pattern(leds_data, &pattern_wifi_connecting2);
      led_refresh();
    }
    break;

  case WIFI_CONNECTED:
  case WIFI_CONNECT_FAILED:
    sys_mode = SYS_WIFI_STADUS;
    break;

  default:
    break;
  }

  if (sys_cnt > FREQ_SYS * 10) // time out 10s
  {
    printf("wifi connect time out!\n");
    sys_mode = SYS_WIFI_STADUS;
    wifi_disconnect();
  }
}

void sysmode_wifi_stadus()
{
  if (wifi_connect() == WIFI_CONNECTED)
  {
    // show ok logo
    if (sys_interval(SYS_INTERVAL_0, 0))
    {
      led_show_pattern(leds_data, &pattern_wifi_connect_ok);
      led_refresh();
    }
    else if (sys_interval(SYS_INTERVAL_0, SYS_INTERVAL_OFS_0))
    {
      led_clear();
    }
  }
  else
  {
    // show ng logo
    if (sys_interval(SYS_INTERVAL_0, 0))
    {
      led_show_pattern(leds_data, &pattern_wifi_connect_ng);
      led_refresh();
    }
    else if (sys_interval(SYS_INTERVAL_0, SYS_INTERVAL_OFS_0))
    {
      led_clear();
    }
  }

  if (sys_cnt > FREQ_SYS * 1.5)
  {
    if (wifi_connect() == WIFI_CONNECTED)
    {
      get_net_time_cnt = GET_NET_TIME_CNT_LIMIT;
      sys_mode = SYS_REAL_TIME;
    }
    else
      sys_mode = SYS_RAIN;
    led_clear();
  }
}


void sysmode_real_time()
{
  if (sys_cnt >= FREQ_SYS * 0.1)
  {
    sys_cnt = 0;
    get_net_time();
    
    time_t real_t;
    struct tm *p_time;

    real_t = time_base + time_offset;
    p_time = localtime(&real_t);
    strftime(led_show_text, sizeof(led_show_text), "%H:%M:%S", p_time);
    led_show_char(leds_data, 2, 1, led_show_text, LED_SZIE_45, int2rgb(real_time_final()));

    led_refresh();
  }
}

void sysmode_time_weather()
{
  if (sys_cnt >= FREQ_SYS * 0.1)
  {
    sys_cnt = 0;
    // get_net_time();

    time_t real_t;
    struct tm *p_time;

    real_t = time_base + time_offset;
    p_time = localtime(&real_t);
    strftime(led_show_text, sizeof(led_show_text), "%H:%M", p_time);
    led_show_char(leds_data, 12, 1, led_show_text, LED_SZIE_45, int2rgb(real_time_final()));
    if (time_offset & 1)
      for (u8 i = 0; i < LED_ROW; i++)
        leds_data[20][i] = 0;

    led_refresh();
  }
}

void sysmode_fft()
{
  static u32 t = 0, dt = 20;
  static u8 fft_flag = 0;

  if (sys_cnt >= FFT_FPS)
  {
    sys_cnt = 0;
    debug_set;

    // 13ms, 10khz
    for (u8 i = 0; i < ADC_SAMPLES; i++)
    {
      fftReal[i] = analogRead(PIN_ADC_CHANNEL); // 读取模拟值，信号采样
      fftImag[i] = 0;
    }
    dbg_reset;

    // u16 min_ = 4095;
    // for (u8 i = 0; i < ADC_SAMPLES; i++)
    //   min_ = _min(min_, fftReal[i]);

    // for (u8 i = 0; i < ADC_SAMPLES; i++)
    //   fftReal[i] -= min_;

    // for (u8 i = 0; i < ADC_SAMPLES; i++)
    //   printf("adc: %f\n", fftReal[i]);

    /*FFT运算*/
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
    FFT.Compute(FFT_FORWARD);                        /* Compute FFT */
    FFT.ComplexToMagnitude();                        /* Compute magnitudes */
    // for (u8 i = 0; i < ADC_SAMPLES; i++)
    //   printf("fft: %f\n", fftReal[i]);

    if ((millis() - t) > dt)
    {               // 读取时间，判断是否达到掉落时长
      fft_flag = 1; // 达到则标记为1
      t = millis(); // 更新时间
    }
    fill_rainbow(leds, LED_NUM, 0, 1);              // 设置彩虹渐变，先填充满，然后根据取值大小填充黑色，表示熄灭灯
    // fft_draw_bar(0, fftReal[1] / 40000, &fft_flag); 

    for (u8 i = 0; i < (LED_COL >> 1); i++)
      fft_draw_bar(i, (fftReal[i * 2 + 4] + fftReal[i * 2 + 5]) / 450.0, &fft_flag);
    for (u8 i = (LED_COL >> 1); i < LED_COL; i++)
      fft_draw_bar(i, (fftReal[i * 2 + 4] + fftReal[i * 2 + 5]) / 280.0, &fft_flag);

    FastLED.show(); // 8ms
  }
}

void sysmode_rain()
{
  if (sys_interval(100))
  {
    pattern_move(0, 0, 31, 7, MOVE_DOWN);
    for (u8 i = 0; i < 10; i++)
    {
      leds_data[random(0, LED_COL)][LED_ROW - 1] = int2rgb(random(10, 0xffffff));
    }
    led_refresh();
  }
}

void sysmode_temp_hum()
{
  static u8 show_tmp = 0;

  if (sys_interval(3000) || sys_mode_change_init == 0)
  {
    sys_cnt = 0;
    if (aht20.available() == true)
    {
      led_clear();
      show_tmp = !show_tmp;
      sys_mode_change_init = 1;

      if (show_tmp)
      {
        float temperature = aht20.getTemperature();
        temperature = constrain(temperature, -99, 99);
        led_show_pattern(leds_data, &pattern_temp, 5, 0);
        sprintf(led_show_text, "%.1fC", temperature);
        led_show_char(leds_data, 11, 1, led_show_text, LED_SZIE_45, int2rgb(real_time_final()));
      }
      else
      {
        float humidity = aht20.getHumidity();
        humidity = constrain(humidity, -99, 99);
        led_show_pattern(leds_data, &pattern_humidity, 5, 0);
        sprintf(led_show_text, "%.1f%%", humidity);
        led_show_char(leds_data, 11, 1, led_show_text, LED_SZIE_45, int2rgb(real_time_final()));
      }
      led_refresh();
    }
  }
}

void IRAM_ATTR sys_timer_isr()
{
  sys_cnt++;
  sys_scheduling++;
  if (sys_key_press_cnt && sys_key_press_init == 1)
  {
    sys_key_press_cnt--;
    if (sys_key_press_cnt == 0)
    {
      if (digitalRead(PIN_ADD_KEY) == 1)
        sys_key_cnt++;

      else if (digitalRead(PIN_SUB_KEY) == 1)
        sys_key_cnt--;

      sys_key_press_init = 0;
    }
  }
}

void IRAM_ATTR real_time_timer_isr()
{
  time_offset++;
  get_net_time_cnt++;
}

void IRAM_ATTR key_press_isr()
{
  sys_key_press_init = 1;
}

bool sys_interval(u16 cycle_ms, u16 ofs_ms)
{
  return ((sys_cnt * 1000) % (sys_freq_cur * cycle_ms) == ofs_ms * 1000);
}

void led_refresh()
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

void led_clear(u8 x0, u8 y0 ,u8 x1, u8 y1)
{
  
  for (u8 i = 0; i < LED_COL; i++)
  {
    for (u8 j = 0; j < LED_ROW; j++)
    {
      leds_data[i][j] = 0;
    }
  }
  // led_refresh();
}

void get_net_time()
{
  if (get_net_time_cnt >= GET_NET_TIME_CNT_LIMIT)
  {
    wifi_sts_t res = wifi_connect();
    if (res == WIFI_CONNECTED)
    {
      get_net_time_cnt = 0;
      printf("get network time...\n");
      if (!http_time())
      {
        time_offset = 0;
        printf("get network time ok!\n");
      }
      wifi_disconnect();
    }

    else if (res == WIFI_CONNECT_FAILED)
    {
      get_net_time_cnt = 0;
      printf("get network time ng!\n");
    }
  }

  if (get_net_time_cnt % EEPROM_COLOR_OFS_LIMIT == 0)
  {
    if (EEPROM.readUInt(EEPROM_COLOR_OFS_REG) != real_time_final())
    {
      EEPROM.writeUInt(EEPROM_COLOR_OFS_REG, real_time_final());
      EEPROM.commit();
      // if (EEPROM.commit())
      //   printf("save time_offset ok\n");
      // else
      //   printf("save time_offset ng\n");
    }
  }
}

u32 real_time_final()
{
  u32 tmp = time_offset * 194 + time_color_ofs;
  tmp %= 0xffffff;
  return tmp;
}

u8 http_time()
{
  u8 ok = 0;
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
        ok = 1;
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
      ok = 2;
  }
  else
  {
    printf("[HTTP] GET Time failed, error: %s\n", http.errorToString(httpCode).c_str());
    ok = 3;
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
  if (value > 0xffffff)
    value %= 0xffffff;

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

void fft_draw_bar(u16 idx, s16 value, u8 *flag) // 绘制函数，按序号和幅度值绘制条形块
{
  static s16 volume[LED_COL];   // 保存下降数据
  value = constrain(value, 1, LED_ROW); // 幅度限制在0-8范围内

  if (volume[idx] < value) // 采集到的数据比之前大则更新，实现上冲效果
    volume[idx] = value;

  if (idx % 2)
  { // 余2运算判断序号是否为奇数
    for (u8 i = 0; i < LED_ROW - volume[idx]; i++)
      leds[idx * LED_ROW + i] = CRGB::Black;
  }
  else
  {
    for (u8 i = volume[idx]; i < LED_ROW; i++)
      leds[idx * LED_ROW + i] = CRGB::Black;
  }

  if (*flag)
  {
    volume[idx] -= 1; // 达到时间则减小1，表示下落
    if (idx == LED_COL - 1)
      *flag = 0; // 第0-7列均更新完毕则清除标记
  }
}

/*
start (x0, y0)
end   (x1, y1)
*/
void pattern_move(u8 x0, u8 y0, u8 x1, u8 y1, move_dir_t dir, u8 loop)
{
  u32 buf[max(LED_ROW, LED_COL)];

  x0 = constrain(x0, 0, LED_COL - 1);
  x1 = constrain(x1, x0, LED_COL - 1);
  y0 = constrain(y0, 0, LED_ROW - 1);
  y1 = constrain(y1, y0, LED_ROW - 1);

  switch (dir)
  {
  case MOVE_UP:
  {
    if (y1 - y0 == 0)
      return;

    if (loop)
    {
      for (u8 i = x0; i <= x1; i++)
      {
        buf[i] = leds_data[i][y1 - 1];
      }
    }
    for (u8 i = x0; i <= x1; i++)
    {
      for (u8 j = y1; j > y0; j--)
      {
        leds_data[i][j] = leds_data[i][j - 1];
      }
    }
    for (u8 i = x0; i <= x1; i++)
    {
      leds_data[i][y0] = loop ? buf[i] : 0;
    }

    break;
  }

  case MOVE_DOWN:
  {
    if (y1 - y0 == 0)
      return;

    if (loop)
    {
      for (u8 i = x0; i <= x1; i++)
      {
        buf[i] = leds_data[i][y0];
      }
    }
    for (u8 i = x0; i <= x1; i++)
    {
      for (u8 j = y0; j < y1; j++)
      {
        leds_data[i][j] = leds_data[i][j + 1];
      }
    }
    for (u8 i = x0; i <= x1; i++)
    {
      leds_data[i][y1] = loop ? buf[i] : 0;
    }

    break;
  }

  case MOVE_RIGHT:
  {
    if (x1 - x0 == 0)
      return;

    if (loop)
    {
      for (u8 i = y0; i <= y1; i++)
      {
        buf[i] = leds_data[x1][i];
      }
    }
    for (u8 i = x1; i > x0; i--)
    {
      for (u8 j = y0; j <= y1; j++)
      {
        leds_data[i][j] = leds_data[i - 1][j];
      }
    }
    for (u8 i = y0; i <= y1; i++)
    {
      leds_data[x0][i] = loop ? buf[i] : 0;
    }

    break;
  }

  case MOVE_LEFT:
  {
    if (x1 - x0 == 0)
      return;

    if (loop)
    {
      for (u8 i = y0; i <= y1; i++)
      {
        buf[i] = leds_data[x0][i];
      }
    }
    for (u8 i = x0; i < x1; i++)
    {
      for (u8 j = y0; j <= y1; j++)
      {
        leds_data[i][j] = leds_data[i + 1][j];
      }
    }
    for (u8 i = y0; i <= y1; i++)
    {
      leds_data[x1][i] = loop ? buf[i] : 0;
    }

    break;
  }

  default:
    break;
  }
}

