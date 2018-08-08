#pragma once

#include "ILI9225.h"

#define COLOR_WHITE COLOR(255, 255, 255)
#define COLOR_BLACK COLOR(0,     0,   0)
#define COLOR_RED   COLOR(255,   0,   0)
#define COLOR_GREEN COLOR(0,   255,   0)
#define COLOR_BLUE  COLOR(0,   0  , 255)

void transmit(uint8_t data);

void LCD_WriteIndex(uint8_t index);
void LCD_WriteData(uint8_t data) ;
void LCD_WriteData_16Bit(uint16_t data);
void LCD_WriteReg(uint8_t index, uint16_t data);

void LCD_Reset(void);

void LCD_Init(void);
void LCD_SetXY(uint16_t Xpos, uint16_t Ypos);
void LCD_SetRegion(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t data);
void LCD_Clear(uint16_t color);

void LCD_DrawBitmap(const uint16_t* data, uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);