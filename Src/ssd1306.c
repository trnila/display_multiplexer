#include <memory.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ssd1306.h>
#include "ssd1306.h"
#include "spi_bus.h"

#define DC_COMMAND GPIO_PIN_RESET
#define DC_DATA GPIO_PIN_SET

void ssd1306_Reset(void) {
	// CS = High (not selected)
	HAL_GPIO_WritePin(SSD1306_DC_GPIO_Port, SSD1306_CS_Pin, GPIO_PIN_SET);

	// Reset the OLED
	HAL_GPIO_WritePin(SSD1306_Reset_GPIO_Port, SSD1306_Reset_Pin, GPIO_PIN_RESET);
	vTaskDelay(10);
	HAL_GPIO_WritePin(SSD1306_Reset_GPIO_Port, SSD1306_Reset_Pin, GPIO_PIN_SET);
	vTaskDelay(10);
}

// Send a byte to the command register
void ssd1306_WriteCommand(SSD1306_t *oled, uint8_t byte) {
    SPIBus_transmit(&oled->dev, DC_COMMAND, &byte, 1);
}

// Send data
void ssd1306_WriteData(SSD1306_t *oled, uint8_t* buffer, size_t buff_size) {
    SPIBus_transmit(&oled->dev, DC_DATA, buffer, buff_size);
}

// Initialize the oled screen
void ssd1306_Init(SSD1306_t *oled, uint8_t* buffer, uint8_t* backbuffer) {
    oled->buffer = buffer;
    oled->backbuffer = backbuffer;
    oled->CurrentX = 0;
    oled->CurrentY = 0;


	// Reset OLED
	ssd1306_Reset();

    // Wait for the screen to boot
    vTaskDelay(100);
    
    // Init OLED
    ssd1306_WriteCommand(oled, 0xAE); //display off

    ssd1306_WriteCommand(oled, 0x20); //Set Memory Addressing Mode   
    ssd1306_WriteCommand(oled, 0x10); // 00,Horizontal Addressing Mode; 01,Vertical Addressing Mode;
                                // 10,Page Addressing Mode (RESET); 11,Invalid

    ssd1306_WriteCommand(oled, 0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd1306_WriteCommand(oled, 0xC0); // Mirror vertically
#else
    ssd1306_WriteCommand(oled, 0xC8); //Set COM Output Scan Direction
#endif

    ssd1306_WriteCommand(oled, 0x00); //---set low column address
    ssd1306_WriteCommand(oled, 0x10); //---set high column address

    ssd1306_WriteCommand(oled, 0x40); //--set start line address - CHECK

    ssd1306_WriteCommand(oled, 0x81); //--set contrast control register - CHECK
    ssd1306_WriteCommand(oled, 0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_WriteCommand(oled, 0xA0); // Mirror horizontally
#else
    ssd1306_WriteCommand(oled, 0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
    ssd1306_WriteCommand(oled, 0xA7); //--set inverse color
#else
    ssd1306_WriteCommand(oled, 0xA6); //--set normal color
#endif

    ssd1306_WriteCommand(oled, 0xA8); //--set multiplex ratio(1 to 64) - CHECK
    ssd1306_WriteCommand(oled, 0x3F); //

    ssd1306_WriteCommand(oled, 0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    ssd1306_WriteCommand(oled, 0xD3); //-set display offset - CHECK
    ssd1306_WriteCommand(oled, 0x00); //-not offset

    ssd1306_WriteCommand(oled, 0xD5); //--set display clock divide ratio/oscillator frequency
    ssd1306_WriteCommand(oled, 0xF0); //--set divide ratio

    ssd1306_WriteCommand(oled, 0xD9); //--set pre-charge period
    ssd1306_WriteCommand(oled, 0x22); //

    ssd1306_WriteCommand(oled, 0xDA); //--set com pins hardware configuration - CHECK
    ssd1306_WriteCommand(oled, 0x12);

    ssd1306_WriteCommand(oled, 0xDB); //--set vcomh
    ssd1306_WriteCommand(oled, 0x20); //0x20,0.77xVcc

    ssd1306_WriteCommand(oled, 0x8D); //--set DC-DC enable
    ssd1306_WriteCommand(oled, 0x14); //
    ssd1306_WriteCommand(oled, 0xAF); //--turn on SSD1306 panel

/*
    // For SH1106
    // see http://we.easyelectronics.ru/lcd_gfx/oled-sh1106.html
    ssd1306_WriteCommand(oled, 0xAE); //display off

    ssd1306_WriteCommand(oled, 0xA8); // Set Multiplex Ratio (0x3F) - CHECK
    ssd1306_WriteCommand(oled, 0x3F);

    ssd1306_WriteCommand(oled, 0x81); // Set Contrast Control (0x7f) - CHECK
    ssd1306_WriteCommand(oled, 0x7F);

    ssd1306_WriteCommand(oled, 0xA1); // Set Segment Re-map - CHECK
    ssd1306_WriteCommand(oled, 0xC0); // Set Common Output Scan Direction (i.e. reverset)
    
    ssd1306_WriteCommand(oled, 0xDA); // Common Pads Hardware Configuration (Alternative Mode Set) - CHECK
    ssd1306_WriteCommand(oled, 0x12);
    
    ssd1306_WriteCommand(oled, 0xD3); // Set Display Offset (0x00) - CHECK
    ssd1306_WriteCommand(oled, 0x00);

    ssd1306_WriteCommand(oled, 0x40); // Set Display Start Line 0 - CHECK

    ssd1306_WriteCommand(oled, 0xAF); //--turn on SSD1306 panel
*/
    // Clear screen
    ssd1306_Fill(oled, Black);
    
    // Flush buffer to screen
    ssd1306_UpdateScreen(oled);
}

// Fill the whole screen with the given color
void ssd1306_Fill(SSD1306_t* oled, SSD1306_COLOR color) {
    /* Set memory */
    uint32_t i;

    for(i = 0; i < OLED_BUFFER_SIZE; i++) {
        oled->buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

void ssd1306_Swap(SSD1306_t *oled) {
    uint8_t *tmp = oled->buffer;
    oled->buffer = oled->backbuffer;
    oled->backbuffer = tmp;

}

// Write the screenbuffer with changed to the screen
void ssd1306_UpdateScreen(SSD1306_t *oled) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
        ssd1306_WriteCommand(oled, 0xB0 + i);
        ssd1306_WriteCommand(oled, 0x00);
        ssd1306_WriteCommand(oled, 0x10);
        ssd1306_WriteData(oled, &oled->buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

//    Draw one pixel in the screenbuffer
//    X => X Coordinate
//    Y => Y Coordinate
//    color => Pixel color
void ssd1306_DrawPixel(SSD1306_t *oled, uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        // Don't write outside the buffer
        return;
    }
    
    // Check if pixel should be inverted
    if(oled->Inverted) {
        color = (SSD1306_COLOR)!color;
    }
    
    // Draw in the right color
    if(color == White) {
        oled->buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else { 
        oled->buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

SSD1306_COLOR ssd1306_GetPixel(SSD1306_t *oled, uint8_t x, uint8_t y) {
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		// Don't write outside the buffer
		return Black;
	}

	if(oled->backbuffer[x + (y / 8) * SSD1306_WIDTH] & (1 << (y % 8))) {
	    return White;
	}
	return Black;
}
// Draw 1 char to the screen buffer
// ch         => char om weg te schrijven
// Font     => Font waarmee we gaan schrijven
// color     => Black or White
char ssd1306_WriteChar(SSD1306_t *oled, char ch, FontDef Font, SSD1306_COLOR color) {
    uint32_t i, b, j;
    
    // Check remaining space on current line
    if (SSD1306_WIDTH <= (oled->CurrentX + Font.FontWidth) ||
        SSD1306_HEIGHT <= (oled->CurrentY + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }
    
    // Use the font to write
    for(i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for(j = 0; j < Font.FontWidth; j++) {
            if((b << j) & 0x8000)  {
                ssd1306_DrawPixel(oled, oled->CurrentX + j, (oled->CurrentY + i), (SSD1306_COLOR) color);
            } else {
                ssd1306_DrawPixel(oled, oled->CurrentX + j, (oled->CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }
    
    // The current space is now taken
    oled->CurrentX += Font.FontWidth;
    
    // Return written char for validation
    return ch;
}

// Write full string to screenbuffer
char ssd1306_WriteString(SSD1306_t *oled, char* str, FontDef Font, SSD1306_COLOR color) {
    // Write until null-byte
    while (*str) {
        if (ssd1306_WriteChar(oled, *str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        
        // Next char
        str++;
    }
    
    // Everything ok
    return *str;
}

// Position the cursor
void ssd1306_SetCursor(SSD1306_t *oled, uint8_t x, uint8_t y) {
    oled->CurrentX = x;
    oled->CurrentY = y;
}
