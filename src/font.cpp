#include "font.h"

/******************************************************************************
函数说明：显示多个字符
入口数据：x,y显示坐标
        num 要显示的字符
        fc 字的颜色
        bc 字的背景色
        mode:  0非叠加模式  1叠加模式
返回值：  无
******************************************************************************/
void led_show_char(u32 (*leds)[8], u8 x, u8 y, u8 *p, u8 size, u32 fc, u32 bc, u8 mode)
{
    if (x < 0 || x >= 32)
        return;
    if (y < 0 || y >= LED_RAW)
        return;

    u16 s = 0;
    u8 num = 0;

    s = (x * 8 + y);
    while (*p != '\0')
    {
        for (u8 i = 0; i < 4; i++)
        {
            num = ascii_0408[*p - ' '][i]; // 得到偏移后的值
            for (u8 bit = 0; bit < 8; bit++)
            {
                if (num & (1 << bit))
                {
                    if (fc != 0x01ffffff)
                        leds[s + 8 * i + bit] = fc;
                    // else
                    // leds[x * 8 + y] = leds[x * 8 + y];
                    // continue;
                }
                else
                {
                    if (!mode)
                        leds[s + 8 * i + bit] = bc;
                }
            }
        }
        p++;
        s += 32;
    }
}
