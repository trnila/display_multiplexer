#include <memory.h>
#include "ssd1306.h"


int get_neighbors(SSD1306_t* oled, int x, int y) {
	int total = 0;
	for(int i = -1; i < 2; i++) {
		for(int j = -1; j < 2; j++) {
			if(ssd1306_GetPixel(oled, x + i, y + j) == White) {
				total++;
			}
		}
	}

	if(ssd1306_GetPixel(oled, x, y) == White) {
		total--;
	}

	return total;
}

void game_of_life(SSD1306_t *oled) {
	for (int y = 0; y < SSD1306_HEIGHT; y++) {
		for (int x = 0; x < SSD1306_WIDTH; x++) {
			int neighbors = get_neighbors(oled, x, y);
			int state;
			if (ssd1306_GetPixel(oled, x, y) == White) {
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
			ssd1306_DrawPixel(oled, x, y, state);
		}
	}

	ssd1306_Swap(oled);
	ssd1306_UpdateScreen(oled);
}