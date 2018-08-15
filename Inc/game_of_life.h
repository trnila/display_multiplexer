#pragma once
#include <stdint.h>
#include "bitmatrix.h"

// TODO: fix calculation
#define GAMEOFLIFE_BUFFER_SIZE(ZOOM) (1300)

struct gameoflife {
	struct bitmatrix cur;
	struct bitmatrix prev;
	uint8_t zoom;
};

void gameoflife_init(struct gameoflife *game, uint8_t zoom, uint8_t* mem);
void game_of_life(struct gameoflife* game);
void gameoflife_clear(struct gameoflife *game);
void gameoflife_random(struct gameoflife *game);
