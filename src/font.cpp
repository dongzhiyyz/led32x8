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
    if (x < 0 || x >= LED_COL)
        return;
    if (y < 0 || y >= LED_ROW)
        return;

    u8 char_ = 0;
    u8 hight = 0; // 字高
    u8 char_cnt = 0;
    u8 idx_x, idx_y = 0;

    if (size == 3)
        hight = 5;
    else if (size == 4)
        hight = 8;

    while (*p != '\0')
    {   
        for (u8 i = 0; i < size; i++)
        {
            if (size == 3)
                char_ = ascii_0305[*p - ' '][i]; // 得到偏移后的值
            else if (size == 4)
                char_ = ascii_0408[*p - ' '][i];

            for (u8 bit = 0; bit < hight; bit++)
            {
                idx_x = char_cnt + x + i;
                idx_y = y + bit;
                if (idx_x >= LED_COL || idx_y >= LED_ROW)
                    break;

                if (char_ & (1 << bit))
                {
                    if (fc != 0x01ffffff)
                        leds[idx_x][idx_y] = fc;
                }
                else
                {
                    if (!mode)
                        leds[x][y] = bc;
                }
            }
        }
        p++;
        char_cnt += size;
    }
}
