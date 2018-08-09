#include <memory.h>
#include "ssd1306.h"

static uint8_t next_generation[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

int get_neighbors(int x, int y) {
	int total = 0;
	for(int i = -1; i < 2; i++) {
		for(int j = -1; j < 2; j++) {
			if(ssd1306_GetPixel(x + i, y + j) == White) {
				total++;
			}
		}
	}

	if(ssd1306_GetPixel(x, y) == White) {
		total--;
	}

	return total;
}

void game_of_life() {
	memset(next_generation, 0, sizeof(next_generation));
	for (int y = 0; y < SSD1306_HEIGHT; y++) {
		for (int x = 0; x < SSD1306_WIDTH; x++) {
			int neighbors = get_neighbors(x, y);
			int state;
			if (ssd1306_GetPixel(x, y) == White) {
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

			if (state) {
				next_generation[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
			}
		}
	}

	ssd1306_Swap(next_generation);
	ssd1306_UpdateScreen();
}