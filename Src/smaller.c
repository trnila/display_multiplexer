#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "ssd1306.h"
#include "bitmatrix.h"
#include "game_of_life.h"
#include "assets/patterns.h"


uint8_t buffer[OLED_BUFFER_SIZE];

SSD1306_t oled1 = {
		.buffer = buffer,
		.dev = {
				.port_cs = SSD1306_CS_GPIO_Port,
				.pin_cs = SSD1306_CS_Pin,

				.port_dc = SSD1306_DC_GPIO_Port,
				.pin_dc = SSD1306_DC_Pin
		}

};

SSD1306_t oled2 = {
		.buffer = buffer,
		.dev = {
				.port_cs = SSD1306_CS2_GPIO_Port,
				.pin_cs = SSD1306_CS2_Pin,

				.port_dc = SSD1306_DC_GPIO_Port,
				.pin_dc = SSD1306_DC_Pin
		}

};

#define ZOOM 1
struct gameoflife game;

void draw(SSD1306_t *oled, int from) {
	ssd1306_Fill(oled, Black);
	for(int x = 0; x < 128 / ZOOM; x++) {
		for(int y = 0; y < 64 / ZOOM; y++) {
			for(int i = 0; i < ZOOM; i++) {
				for (int j = 0; j < ZOOM; j++) {
					int state = bitmatrix_get(&game.prev, from + x, y);
					ssd1306_DrawPixel(oled, ZOOM * x + i, ZOOM * y + j, state);
				}
			}
		}
	}
	ssd1306_UpdateScreen(oled);
}

void task_SSD1306(void *argument) {
	ssd1306_Init(&oled1);
	ssd1306_Init(&oled2);

	ssd1306_WriteChar(&oled2, 'a', Font_16x26, White);
	ssd1306_UpdateScreen(&oled2);

	gameoflife_init(&game, 2 * SSD1306_WIDTH, SSD1306_HEIGHT);

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

		for(int i = 0; i < 10; i++) {
			game_of_life(&game);

			draw(&oled1, 0);
			draw(&oled2, 128);

			vTaskDelay(100);
		}
	}
}