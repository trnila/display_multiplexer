#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "ssd1306.h"

void game_of_life(SSD1306_t *oled);


uint8_t buffer[OLED_BUFFER_SIZE], backbuffer[OLED_BUFFER_SIZE];
SSD1306_t oled = {
		.buffer = buffer,
		.backbuffer = backbuffer,
		.dev = {
				.port_cs = SSD1306_CS_GPIO_Port,
				.pin_cs = SSD1306_CS_Pin,

				.port_dc = SSD1306_DC_GPIO_Port,
				.pin_dc = SSD1306_DC_Pin
		}

};

void task_SSD1306(void *argument) {
	ssd1306_Init(&oled, buffer, backbuffer);

	for (int y = 0; y < SSD1306_HEIGHT; y++) {
		for (int x = 0; x < SSD1306_WIDTH; x++) {
			ssd1306_DrawPixel(&oled, x, y, rand() % 2 == 0 ? White : Black);
		}
	}
	ssd1306_Swap(&oled);

	for(;;) {
		game_of_life(&oled);
		vTaskDelay(100);
	}
}