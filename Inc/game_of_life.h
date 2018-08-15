#pragma once
#include <stdint.h>
#include "bitmatrix.h"

// TODO: fix calculation
#define GAMEOFLIFE_BUFFER_SIZE(ZOOM) (2400)

struct gameoflife {
	struct bitmatrix cur;
	struct bitmatrix prev;
	int width;
	int height;
};

void gameoflife_init(struct gameoflife *game, int width, int height);
void game_of_life(struct gameoflife* game);
void gameoflife_clear(struct gameoflife *game);
void gameoflife_random(struct gameoflife *game);
