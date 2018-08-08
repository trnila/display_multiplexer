#include <stm32f1xx.h>
#include <FreeRTOS.h>
#include <task.h>
#include "lcd.h"
#include "images/tux.h"
#include "utils.h"
#include "ssd1306.h"
#include "spi_bus.h"

#define COLORS 64

void task_ILI9225(void *param) {
	LCD_Init();
	LCD_Clear(COLOR_WHITE);

	int i = 0;
	for(;;) {
		LCD_Clear(COLOR_WHITE);
		LCD_DrawBitmap(image_tux_data, i, 0, image_tux_width, image_tux_height);
		vTaskDelay(10);

		i = (i + 10) % 100;
	}
}

void app() {
	SPIBus_init(&hspi1);

	if(xTaskCreate(task_ILI9225, "ili9225", 128, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
		HALT();
	}

	vTaskDelay(10);

	ssd1306_Init();

	int dots = 0;
	for(;;) {
		ssd1306_SetCursor(0, 0);
		ssd1306_Fill(Black);
		ssd1306_WriteString("Testing", Font_11x18, White);

		for(int i = 0; i < dots; i++) {
			ssd1306_WriteChar('.', Font_11x18, White);
		}
		dots = (dots + 1) % 4;

		ssd1306_UpdateScreen();
		vTaskDelay(10);
	}
}