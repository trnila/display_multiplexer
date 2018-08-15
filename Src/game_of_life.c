#include <memory.h>
#include "game_of_life.h"
#include "ssd1306.h"
#include "bitmatrix.h"
#include <stdlib.h>

uint8_t mem1[3000];
uint8_t mem2[3000];

void gameoflife_init(struct gameoflife *game, int width, int height) {
	game->width = width;
	game->height = height;

	game->cur.data = mem1;
	game->cur.width = width;
	game->cur.height = height;

	game->prev.data = mem2;
	game->prev.width = width;
	game->prev.height = height;
}

void gameoflife_random(struct gameoflife *game) {
	for (int x = 0; x < game->width; x++) {
		for (int y = 0; y < game->height; y++) {
			bitmatrix_set(&game->prev, x, y, rand() % 2 == 0);
		}
	}
}

void gameoflife_clear(struct gameoflife *game) {
	bitmatrix_clear(&game->cur);
	bitmatrix_clear(&game->prev);
}

int get_neighbors(struct gameoflife *game, int x, int y) {
	int total = 0;
	for(int i = -1; i < 2; i++) {
		for(int j = -1; j < 2; j++) {
			if(bitmatrix_get(&game->prev, x + i, y + j)) {
				total++;
			}
		}
	}

	if(bitmatrix_get(&game->prev, x, y)) {
		total--;
	}

	return total;
}

void game_of_life(struct gameoflife* game) {
	for (int x = 0; x < game->width; x++) {
		for (int y = 0; y < game->height; y++) {
			int neighbors = get_neighbors(game, x, y);
			int state;
			if (bitmatrix_get(&game->prev, x, y)) {
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
			bitmatrix_set(&game->cur, x, y, state);
		}
	}

	uint8_t *tmp = game->prev.data;
	game->prev.data = game->cur.data;
	game->cur.data = tmp;
}