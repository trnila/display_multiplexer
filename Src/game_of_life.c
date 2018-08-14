#include <memory.h>
#include "ssd1306.h"
#include "bitmatrix.h"
#include <stdlib.h>

uint8_t cur[OLED_BUFFER_SIZE / 4], prev[OLED_BUFFER_SIZE / 4];


void init() {
	for (int y = 0; y < SSD1306_HEIGHT / 2; y++) {
		for (int x = 0; x < SSD1306_WIDTH / 2; x++) {
			bitmatrix_set(prev, x, y, rand() % 2 == 0);
		}
	}
}

int get_neighbors(int x, int y) {
	int total = 0;
	for(int i = -1; i < 2; i++) {
		for(int j = -1; j < 2; j++) {
			if(bitmatrix_get(prev, x + i, y + j)) {
				total++;
			}
		}
	}

	if(bitmatrix_get(prev, x, y)) {
		total--;
	}

	return total;
}

void game_of_life(SSD1306_t *oled) {
	for (int y = 0; y < SSD1306_HEIGHT / 2; y++) {
		for (int x = 0; x < SSD1306_WIDTH / 2; x++) {
			int neighbors = get_neighbors(x, y);
			int state;
			if (bitmatrix_get(prev, x, y)) {
				if (neighbors == 2 || neighbors == 3) {
					state = 1;
				} else {
					state = 0;
				}
			} else {
				if (neighbors == 3) {
					state = 1;
				} else {
					state = 0;
				}
			}
			bitmatrix_set(cur, x, y, state);

			ssd1306_DrawPixel(oled, 2 * x, 2 * y, state);
			ssd1306_DrawPixel(oled, 2 * x + 1, 2 * y, state);
			ssd1306_DrawPixel(oled, 2 * x, 2 * y + 1, state);
			ssd1306_DrawPixel(oled, 2 * x + 1, 2 * y + 1, state);
		}
	}

	memcpy(prev, cur, OLED_BUFFER_SIZE / 4);

	//ssd1306_Swap(oled);
	ssd1306_UpdateScreen(oled);
}