#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "ssd1306.h"

void game_of_life();


void task_SSD1306(void *argument) {
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