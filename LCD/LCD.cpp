#include "LCD.h"
#include "LCD_font.h"

LCD::LCD(uint16_t x, uint16_t y, void (*HalSpiWrite)(uint8_t*, uint8_t), void (*HalDCpinWrite)(uint8_t), void (*HalRstpinWrite)(uint8_t), void (*HalDelay)(uint32_t)) {
    ScreenMaxX = x;
    ScreenMaxY = y;
    SpiWrite = HalSpiWrite;
    DCpinWrite = HalDCpinWrite;
    RstpinWrite = HalRstpinWrite;
    Delay = HalDelay;

    DCpinWrite(1);
    RstpinWrite(0);
    Delay(5000);
    RstpinWrite(1);
    Delay(20);
    //************* Start Initial Sequence **********//
    SPI_WR_REG(0x11);
    Delay(200);
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

void LCD::SPI_WR_DAT(uint8_t dat)
{
    SpiWrite(&dat, 1);
}

void LCD::SPI_WR_2DAT(uint16_t dat)
{
    uint8_t DAT_group[2] = {(uint8_t)(dat >> 8), (uint8_t)dat};
    SpiWrite(DAT_group, 2);
}

void LCD::SPI_WR_REG(uint8_t dat)
{
    DCpinWrite(0);
    SPI_WR_DAT(dat);
    DCpinWrite(1);
}

void LCD::SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    SPI_WR_REG(0x2a); // 列地址设置
    SPI_WR_2DAT(x1);
    SPI_WR_2DAT(x2);
    SPI_WR_REG(0x2b); // 行地址设置
    SPI_WR_2DAT(y1);
    SPI_WR_2DAT(y2);
    SPI_WR_REG(0x2c);
}

void LCD::DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    SetWindow(x, y, x, y); // 设置窗口位置
    SPI_WR_2DAT(color);
}

void LCD::Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    SetWindow(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            SPI_WR_2DAT(color);
        }
    }
}

void LCD::SetCursor(uint16_t x, uint16_t y, uint16_t F, uint16_t B)
{
    x_pos = x;
	y_pos = y;
	FColor = F;
	BColor = B;
}

void LCD::ShowChar(uint16_t x, uint16_t y, uint16_t F, uint16_t B, char letter)
{
    uint8_t i, j, c;
    c = letter - ' ';
    SetWindow(x, y, x + 7, y + 13);
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

void LCD::ShowString(char *chr)
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
            ShowChar(x_pos + i * 8, y_pos + 14 * j, FColor, BColor, chr[k++]);
            i++;
            break;
        }
    }
}

void LCD::ShowString2(char *chr)
{
    uint8_t k = 0;
    while (chr[k] != '\0')
    {
        switch (chr[k])
        {
        case '\r':
            k++;
            x_pos = 0;
            break;
        case '\n':
            k++;
            y_pos += 14;
            break;
        default:
            ShowChar(x_pos, y_pos, FColor, BColor, chr[k++]);
            x_pos += 8;
			if(x_pos > (ScreenMaxX - 8))
			{
				x_pos = 0;
				y_pos += 14;
			}
			if(y_pos > (ScreenMaxY-14))
			{
				Fill(0, 0, ScreenMaxX, ScreenMaxY, BColor);
				y_pos = 0;
			}
            break;
        }
    }
}
