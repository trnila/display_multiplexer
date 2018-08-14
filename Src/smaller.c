#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "ssd1306.h"
#include "bitmatrix.h"
#include "game_of_life.h"


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

#define ZOOM 2
uint8_t gameoflife_mem[2 * GAMEOFLIFE_BUFFER_SIZE(ZOOM)];
struct gameoflife game;

void task_SSD1306(void *argument) {
	ssd1306_Init(&oled, buffer, NULL);

	gameoflife_init(&game, &oled, ZOOM, gameoflife_mem);

	for(;;) {
		game_of_life(&game);
		vTaskDelay(100);
	}
}