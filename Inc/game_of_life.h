#pragma once
#include <stdint.h>
#include "bitmatrix.h"

// TODO: fix calculation
#define GAMEOFLIFE_BUFFER_SIZE(ZOOM) (200)

struct gameoflife {
	struct bitmatrix cur;
	struct bitmatrix prev;
	uint8_t zoom;
	SSD1306_t *oled;
};

void gameoflife_init(struct gameoflife *game, SSD1306_t *oled, uint8_t zoom, uint8_t* mem);
void game_of_life(struct gameoflife* game);
void gameoflife_clear(struct gameoflife *game);
void gameoflife_random(struct gameoflife *game);
