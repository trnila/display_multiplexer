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
				.port_cs = OLED0_CS_GPIO_Port,
				.pin_cs = OLED0_CS_Pin,

				.port_dc = DISPLAY_DATA_GPIO_Port,
				.pin_dc = DISPLAY_DATA_Pin
		}

};

SSD1306_t oled2 = {
		.buffer = buffer,
		.dev = {
				.port_cs = OLED1_CS_GPIO_Port,
				.pin_cs = OLED1_CS_Pin,

				.port_dc = DISPLAY_DATA_GPIO_Port,
				.pin_dc = DISPLAY_DATA_Pin
		}

};

#define ZOOM 3
struct gameoflife game;

void draw(SSD1306_t *oled, int offset) {
	int from = offset * 128 / ZOOM;

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

static void show_text() {
	ssd1306_Fill(&oled1, Black);
	ssd1306_SetCursor(&oled1, 0, 15);
	ssd1306_WriteString(&oled1, "   OLED", Font_16x26, White);
	ssd1306_UpdateScreen(&oled1);

	ssd1306_Fill(&oled2, Black);
	ssd1306_SetCursor(&oled2, 0, 15);
	ssd1306_WriteString(&oled2, "DISPLAY", Font_16x26, White);
	ssd1306_UpdateScreen(&oled2);
}

void task_SSD1306(void *argument) {
	ssd1306_Init(&oled1);
	ssd1306_Init(&oled2);

	gameoflife_init(&game, 2 * SSD1306_WIDTH, SSD1306_HEIGHT);

	int pattern = -1;
	for(;;) {
		if(pattern == -1) {
			show_text();
			pattern = 0;
			vTaskDelay(3000);
		}


		gameoflife_clear(&game);
		for(int i = gameoflife_offsets[pattern]; i < gameoflife_offsets[pattern + 1]; i += 2) {
			bitmatrix_set(&game.prev, gameoflife_patterns[i], gameoflife_patterns[i + 1], 1);
		}

		pattern++;
		if(pattern + 1 >= sizeof(gameoflife_offsets) / sizeof(*gameoflife_offsets)) {
			pattern = -1;
		}

		for(int i = 0; i < 10; i++) {
			game_of_life(&game);

			draw(&oled1, 0);
			draw(&oled2, 1);

			vTaskDelay(20);
		}
	}
}