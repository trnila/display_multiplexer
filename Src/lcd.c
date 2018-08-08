#include <FreeRTOS.h>
#include <task.h>
#include "stm32f1xx.h"
#include "spi.h"
#include "ILI9225.h"
#include <memory.h>

static uint16_t line[ILI9225_LCD_WIDTH];

void begin_transfer() {
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_SET);
}

void end_transfer() {
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

void transmit(uint8_t data) {
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &data, sizeof(data), HAL_MAX_DELAY);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
}

void LCD_WriteIndex(uint8_t index) {
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_RESET);
	transmit(index);
}

void LCD_WriteData(uint8_t data) {
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, GPIO_PIN_SET);
	transmit(data);
}

void LCD_WriteData_16Bit(uint16_t data) {
	LCD_WriteData(data >> 8);
	LCD_WriteData(data);
}

void LCD_WriteReg(uint8_t index, uint16_t data) {
	LCD_WriteIndex(index);
	LCD_WriteData_16Bit(data);
}

void LCD_Reset(void) {
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET);
	vTaskDelay(100);
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
	vTaskDelay(50);
}

void LCD_Init(void) {
	LCD_Reset();

	LCD_WriteReg(ILI9225_POWER_CTRL1, 0);
	LCD_WriteReg(ILI9225_POWER_CTRL2, 0);
	LCD_WriteReg(ILI9225_POWER_CTRL3, 0);
	LCD_WriteReg(ILI9225_POWER_CTRL4, 0);
	LCD_WriteReg(ILI9225_POWER_CTRL5, 0);
	vTaskDelay(40);

	// power on sequence
	LCD_WriteReg(ILI9225_POWER_CTRL2, 0x0018);
	LCD_WriteReg(ILI9225_POWER_CTRL3, 0x6121); //0x6121
	LCD_WriteReg(ILI9225_POWER_CTRL4, 0x006F); // F
	LCD_WriteReg(ILI9225_POWER_CTRL5, 0x495F); // 495f
	LCD_WriteReg(ILI9225_POWER_CTRL1, 0x0800); // 0x0F00, rychlejsi render?

	vTaskDelay(10);
	LCD_WriteReg(ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
	vTaskDelay(30);


	LCD_WriteReg(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion

	//R01H:SM=0,GS=0,SS=1 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
	//R03H:BGR=1,ID0=1,ID1=1,AM=0 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(ILI9225_ENTRY_MODE, 0x1038); // set GRAM write direction.

	LCD_WriteReg(ILI9225_DISP_CTRL1, 0x0000); // Display off
	LCD_WriteReg(ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
	LCD_WriteReg(ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
	LCD_WriteReg(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
	LCD_WriteReg(ILI9225_OSC_CTRL, 0x0D01); // Set Osc
	LCD_WriteReg(ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
	LCD_WriteReg(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
	LCD_WriteReg(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address

	// Set GRAM area
	LCD_WriteReg(ILI9225_GATE_SCAN_CTRL, 0x0000);
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
	LCD_WriteReg(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
	LCD_WriteReg(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
	LCD_WriteReg(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
	LCD_WriteReg(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
	LCD_WriteReg(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
	LCD_WriteReg(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);


	// Adjust the Gamma 2.2 Curve
	LCD_WriteReg(ILI9225_GAMMA_CTRL1, 0x0000);
	LCD_WriteReg(ILI9225_GAMMA_CTRL2, 0x0808);
	LCD_WriteReg(ILI9225_GAMMA_CTRL3, 0x080A);
	LCD_WriteReg(ILI9225_GAMMA_CTRL4, 0x000A);
	LCD_WriteReg(ILI9225_GAMMA_CTRL5, 0x0A08);
	LCD_WriteReg(ILI9225_GAMMA_CTRL6, 0x0808);
	LCD_WriteReg(ILI9225_GAMMA_CTRL7, 0x0000);
	LCD_WriteReg(ILI9225_GAMMA_CTRL8, 0x0A00);
	LCD_WriteReg(ILI9225_GAMMA_CTRL9, 0x0710);
	LCD_WriteReg(ILI9225_GAMMA_CTRL10, 0x0710);

	LCD_WriteReg(ILI9225_DISP_CTRL1, 0x0012); // Vertical RAM Address Position
	vTaskDelay(50);
	LCD_WriteReg(ILI9225_DISP_CTRL1, 0x1017); // Vertical RAM Address Position
}

void LCD_SetXY(uint16_t Xpos, uint16_t Ypos) {
	LCD_WriteReg(ILI9225_RAM_ADDR_SET1, Xpos);
	LCD_WriteReg(ILI9225_RAM_ADDR_SET2, Ypos);
	LCD_WriteIndex(ILI9225_GRAM_DATA_REG);
}


void LCD_SetRegion(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd) {
	LCD_WriteReg(ILI9225_ENTRY_MODE, 0x18);

	LCD_WriteReg(ILI9225_HORIZONTAL_WINDOW_ADDR1, xEnd);
	LCD_WriteReg(ILI9225_HORIZONTAL_WINDOW_ADDR2, xStart);
	LCD_WriteReg(ILI9225_VERTICAL_WINDOW_ADDR1, yEnd);
	LCD_WriteReg(ILI9225_VERTICAL_WINDOW_ADDR2, yStart);
	LCD_WriteReg(ILI9225_RAM_ADDR_SET1, xStart);
	LCD_WriteReg(ILI9225_RAM_ADDR_SET2, yEnd);

	LCD_WriteIndex(ILI9225_GRAM_DATA_REG);
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t data) {
	LCD_SetXY(x, y);
	LCD_WriteData_16Bit(data);
}

void LCD_Clear(uint16_t color) {
	LCD_SetRegion(0, 0, ILI9225_LCD_WIDTH, ILI9225_LCD_HEIGHT);

	begin_transfer();
	for(int i = 0; i < ILI9225_LCD_WIDTH; i++) {
		line[i] = color;
	}

	for (int i = 0; i < ILI9225_LCD_HEIGHT; i++) {
		HAL_SPI_Transmit(&hspi1, line, sizeof(line), HAL_MAX_DELAY);
	}

	end_transfer();
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	for (; x1 <= x2, y1 <= y2; x1++, y1++) {
		LCD_DrawPoint(x1, y1, color);
	}
}

void LCD_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	for (; x1 <= x2; x1++) {
		for (int y = y1; y <= y2; y++) {
			LCD_DrawPoint(x1, y, color);
		}
	}
}

void LCD_DrawBitmap(const uint16_t* data, uint8_t x0, uint8_t y0, uint8_t width, uint8_t height) {
	LCD_SetRegion(x0, y0, width, height);

	begin_transfer();
	HAL_SPI_Transmit(&hspi1, (uint8_t*) data, sizeof(data[0]) * width * height, HAL_MAX_DELAY);
	end_transfer();
}
