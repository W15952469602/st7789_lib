#include "stdint.h"

#ifndef INC_LCD_H_
#define INC_LCD_H_

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12

typedef struct{
    void (*SpiWrite)(uint8_t*, uint8_t);
    void (*DCpinWrite)(uint8_t);
    void (*RstpinWrite)(uint8_t);
    void (*Delay)(uint32_t);
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t FColor;
    uint16_t BColor;
	uint16_t ScreenMaxX;
    uint16_t ScreenMaxY;
}LCD_dev;

void LCD_Init(uint16_t, uint16_t, void (*HalSpiWrite)(uint8_t*, uint8_t), void (*HalDCpinWrite)(uint8_t), void (*HalRstpinWrite)(uint8_t), void (*HalDelay)(uint32_t));
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_Set(uint16_t x, uint16_t y, uint16_t F, uint16_t B);
void LCD_Show_Char(uint16_t x, uint16_t y, uint16_t F, uint16_t B, char letter);
void LCD_ShowString(char *chr);
void LCD_ShowString2(char *chr);

#endif
