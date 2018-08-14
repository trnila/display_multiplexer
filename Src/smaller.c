#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "ssd1306.h"
#include "bitmatrix.h"
#include "game_of_life.h"
#include "images/gameoflife.h"


uint8_t buffer[OLED_BUFFER_SIZE];

SSD1306_t oled = {
		.buffer = buffer,
		.dev = {
				.port_cs = SSD1306_CS_GPIO_Port,
				.pin_cs = SSD1306_CS_Pin,

				.port_dc = SSD1306_DC_GPIO_Port,
				.pin_dc = SSD1306_DC_Pin
		}

};

#define ZOOM 4
uint8_t gameoflife_mem[2 * GAMEOFLIFE_BUFFER_SIZE(ZOOM)];
struct gameoflife game;

void task_SSD1306(void *argument) {
	ssd1306_Init(&oled, buffer, NULL);

	gameoflife_init(&game, &oled, ZOOM, gameoflife_mem);

	int pattern = 0;
	for(;;) {
		gameoflife_clear(&game);
		for(int i = gameoflife_offsets[pattern]; i < gameoflife_offsets[pattern + 1]; i += 2) {
			bitmatrix_set(&game.prev, gameoflife_patterns[i], gameoflife_patterns[i + 1], 1);
		}

		pattern++;
		if(pattern + 1 >= sizeof(gameoflife_offsets) / sizeof(*gameoflife_offsets)) {
			pattern = 0;
		}


		for(int i = 0; i < 50; i++) {
			game_of_life(&game);
			vTaskDelay(100);
		}
	}
}