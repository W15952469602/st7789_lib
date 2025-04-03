#include "LCD.h"
#include "LCD_font.h"

static LCD_dev handle;

void SPI_WR_DAT(uint8_t dat)
{
    handle.SpiWrite(&dat, 1);
}

void SPI_WR_2DAT(uint16_t dat)
{
    uint8_t DAT_group[2] = {dat >> 8, dat};
    handle.SpiWrite(DAT_group, 2);
}

void SPI_WR_REG(uint8_t dat)
{
    handle.DCpinWrite(0);
    SPI_WR_DAT(dat);
    handle.DCpinWrite(1);
}

void LCD_SET_Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    SPI_WR_REG(0x2a); // 列地址设置
    SPI_WR_2DAT(x1);
    SPI_WR_2DAT(x2);
    SPI_WR_REG(0x2b); // 行地址设置
    SPI_WR_2DAT(y1);
    SPI_WR_2DAT(y2);
    SPI_WR_REG(0x2c);
}

void LCD_Init(uint16_t x, uint16_t y, void (*HalSpiWrite)(uint8_t*, uint8_t), void (*HalDCpinWrite)(uint8_t), void (*HalRstpinWrite)(uint8_t), void (*HalDelay)(uint32_t))
{
	handle.ScreenMaxX = x;
	handle.ScreenMaxY = y;
	handle.SpiWrite = HalSpiWrite;
    handle.DCpinWrite = HalDCpinWrite;
    handle.RstpinWrite = HalRstpinWrite;
    handle.Delay = HalDelay;
    handle.DCpinWrite(1);
    handle.RstpinWrite(0);
    handle.Delay(5000);
    handle.RstpinWrite(1);
    handle.Delay(20);
    //************* Start Initial Sequence **********//
    SPI_WR_REG(0x11);
    handle.Delay(200);
    //************* Start Initial Sequence **********//
    SPI_WR_REG(0x3A);
    SPI_WR_DAT(0x05);
    SPI_WR_REG(0xC5);
    SPI_WR_DAT(0x1A);
    SPI_WR_REG(0x36);
    SPI_WR_DAT(0x60);
    SPI_WR_REG(0xb2);
    SPI_WR_DAT(0x05);
    SPI_WR_DAT(0x05);
    SPI_WR_DAT(0x00);
    SPI_WR_DAT(0x33);
    SPI_WR_DAT(0x33);
    SPI_WR_REG(0xb7);
    SPI_WR_DAT(0x05);
    SPI_WR_REG(0xBB);
    SPI_WR_DAT(0x3F);
    SPI_WR_REG(0xC0);
    SPI_WR_DAT(0x2c);
    SPI_WR_REG(0xC2);
    SPI_WR_DAT(0x01);
    SPI_WR_REG(0xC3);
    SPI_WR_DAT(0x0F);
    SPI_WR_REG(0xC4);
    SPI_WR_DAT(0x20);
    SPI_WR_REG(0xC6);
    SPI_WR_DAT(0X01);
    SPI_WR_REG(0xd0);
    SPI_WR_DAT(0xa4);
    SPI_WR_DAT(0xa1);
    SPI_WR_REG(0xE8);
    SPI_WR_DAT(0x03);
    SPI_WR_REG(0xE9);
    SPI_WR_DAT(0x09);
    SPI_WR_DAT(0x09);
    SPI_WR_DAT(0x08);
    SPI_WR_REG(0xE0);
    SPI_WR_DAT(0xD0);
    SPI_WR_DAT(0x05);
    SPI_WR_DAT(0x09);
    SPI_WR_DAT(0x09);
    SPI_WR_DAT(0x08);
    SPI_WR_DAT(0x14);
    SPI_WR_DAT(0x28);
    SPI_WR_DAT(0x33);
    SPI_WR_DAT(0x3F);
    SPI_WR_DAT(0x07);
    SPI_WR_DAT(0x13);
    SPI_WR_DAT(0x14);
    SPI_WR_DAT(0x28);
    SPI_WR_DAT(0x30);
    SPI_WR_REG(0XE1);
    SPI_WR_DAT(0xD0);
    SPI_WR_DAT(0x05);
    SPI_WR_DAT(0x09);
    SPI_WR_DAT(0x09);
    SPI_WR_DAT(0x08);
    SPI_WR_DAT(0x03);
    SPI_WR_DAT(0x24);
    SPI_WR_DAT(0x32);
    SPI_WR_DAT(0x32);
    SPI_WR_DAT(0x3B);
    SPI_WR_DAT(0x14);
    SPI_WR_DAT(0x13);
    SPI_WR_DAT(0x28);
    SPI_WR_DAT(0x2F);
    SPI_WR_REG(0x20);
    SPI_WR_REG(0x11);
    SPI_WR_REG(0x29);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SET_Address(x, y, x, y); // 设置光标位置
    SPI_WR_2DAT(color);
}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_SET_Address(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            SPI_WR_2DAT(color);
        }
    }
}



void LCD_Set(uint16_t x, uint16_t y, uint16_t F, uint16_t B)
{
    handle.x_pos = x;
	handle.y_pos = y;
	handle.FColor = F;
	handle.BColor = B;
}

void LCD_Show_Char(uint16_t x, uint16_t y, uint16_t F, uint16_t B, char letter)
{
    uint8_t i, j, c;
    c = letter - ' ';
    LCD_SET_Address(x, y, x + 7, y + 13);
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (ascii[c][i] & (0x80 >> j))
                SPI_WR_2DAT(F);
            else
                SPI_WR_2DAT(B);
        }
    }
}

void LCD_ShowString(char *chr)
{
    uint8_t i = 0, j = 0, k = 0;
    while (chr[k] != '\0')
    {
        switch (chr[k])
        {
        case '\r':
            k++;
            i = 0;
            break;
        case '\n':
            k++;
            j++;
            break;
        default:
            LCD_Show_Char(handle.x_pos + i * 8, handle.y_pos + 14 * j, handle.FColor, handle.BColor, chr[k++]);
            i++;
            break;
        }
    }
}

void LCD_ShowString2(char *chr)
{
    uint8_t k = 0;
    while (chr[k] != '\0')
    {
        switch (chr[k])
        {
        case '\r':
            k++;
            handle.x_pos = 0;
            break;
        case '\n':
            k++;
            handle.y_pos += 14;
            break;
        default:
            LCD_Show_Char(handle.x_pos, handle.y_pos, handle.FColor, handle.BColor, chr[k++]);
            handle.x_pos += 8;
			if(handle.x_pos > (handle.ScreenMaxX - 8))
			{
				handle.x_pos = 0;
				handle.y_pos += 14;
			}
			if(handle.y_pos > (handle.ScreenMaxY-14))
			{
				LCD_Fill(0, 0, handle.ScreenMaxX, handle.ScreenMaxY, handle.BColor)
				handle.y_pos = 0;
			}
            break;
        }
    }
}
