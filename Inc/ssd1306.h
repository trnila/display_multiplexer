/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "main.h"
#include "spi_bus.h"

#if defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#else
#error "SSD1306 library was tested only on STM32F1 and STM32F4 MCU families. Please modify ssd1306.h if you know what you are doing. Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif

#include "ssd1306_fonts.h"

#ifndef SSD1306_SPI_PORT
#define SSD1306_SPI_PORT        hspi1
#endif

/* ^^^ SPI config ^^^ */

#if defined(SSD1306_USE_I2C)
extern I2C_HandleTypeDef SSD1306_I2C_PORT;
#elif defined(SSD1306_USE_SPI)
extern SPI_HandleTypeDef SSD1306_SPI_PORT;
#else
#error "You should define SSD1306_USE_SPI or SSD1306_USE_I2C macro!"
#endif

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif

// some LEDs don't display anything in first two columns
// #define SSD1306_WIDTH           130

#define OLED_BUFFER_SIZE SSD1306_WIDTH * SSD1306_HEIGHT / 8

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;

    uint8_t *buffer, *backbuffer;

    struct spi_dev dev;
} SSD1306_t;

// Procedure definitions
void ssd1306_Init(SSD1306_t *oled, uint8_t* buffer, uint8_t* backbuffer);
void ssd1306_Fill(SSD1306_t *oled, SSD1306_COLOR color);
void ssd1306_UpdateScreen(SSD1306_t *oled);
void ssd1306_DrawPixel(SSD1306_t *oled, uint8_t x, uint8_t y, SSD1306_COLOR color);
SSD1306_COLOR ssd1306_GetPixel(SSD1306_t *oled, uint8_t x, uint8_t y);
char ssd1306_WriteChar(SSD1306_t *oled, char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(SSD1306_t *oled, char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(SSD1306_t *oled, uint8_t x, uint8_t y);
void ssd1306_Swap(SSD1306_t *oled);

// Low-level procedures
void ssd1306_Reset(void);
void ssd1306_WriteCommand(SSD1306_t* oled, uint8_t byte);
void ssd1306_WriteData(SSD1306_t* oled, uint8_t* buffer, size_t buff_size);

#endif // __SSD1306_H__
