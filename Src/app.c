#include <stm32f1xx.h>
#include <FreeRTOS.h>
#include <stdlib.h>
#include <task.h>
#include <memory.h>
#include "lcd.h"
#include "images/tux.h"
#include "utils.h"
#include "ssd1306.h"
#include "spi_bus.h"

void game_of_life();

#define COLORS 64

void task_ILI9225(void *param) {
	LCD_Init();
	LCD_Clear(COLOR_WHITE);

	int i = 0;
	for(;;) {
		LCD_Clear(COLOR_WHITE);
		LCD_DrawBitmap(image_tux_data, i, 0, image_tux_width, image_tux_height);
		vTaskDelay(1000);

		i = (i + 10) % 100;
	}
}

void app() {
	SPIBus_init(&hspi1);
	ASSERT(xTaskCreate(task_ILI9225, "ili9225", 128, NULL, tskIDLE_PRIORITY, NULL) == pdPASS);

	ssd1306_Init();

	for (int y = 0; y < SSD1306_HEIGHT; y++) {
		for (int x = 0; x < SSD1306_WIDTH; x++) {
			ssd1306_DrawPixel(x, y, rand() % 2 == 0 ? White : Black);
		}
	}
	for(;;) {
		game_of_life();
		vTaskDelay(100);
	}
}