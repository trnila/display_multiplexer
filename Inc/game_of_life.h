#pragma once
#include <stdint.h>
#include "bitmatrix.h"

#define GAMEOFLIFE_BUFFER_SIZE(ZOOM) (OLED_BUFFER_SIZE / (ZOOM) / (ZOOM))

struct gameoflife {
	struct bitmatrix cur;
	struct bitmatrix prev;
	uint8_t zoom;
	SSD1306_t *oled;
};

void gameoflife_init(struct gameoflife *game, SSD1306_t *oled, uint8_t zoom, uint8_t* mem);
void game_of_life(struct gameoflife* game);
