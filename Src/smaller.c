#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "ssd1306.h"
#include "bitmatrix.h"

void game_of_life(SSD1306_t *oled);
void init();


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
	ssd1306_Init(&oled, buffer, NULL);

	init();

	for(;;) {
		game_of_life(&oled);
		vTaskDelay(100);
	}
}