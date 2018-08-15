#include <memory.h>
#include "game_of_life.h"
#include "ssd1306.h"
#include "bitmatrix.h"
#include <stdlib.h>

void gameoflife_init(struct gameoflife *game, uint8_t zoom, uint8_t* mem) {
	game->zoom = zoom;

	game->cur.data = mem;
	game->cur.width = SSD1306_WIDTH / zoom;
	game->cur.height = SSD1306_HEIGHT / zoom;

	game->prev.data = mem + GAMEOFLIFE_BUFFER_SIZE(zoom);
	game->prev.width = SSD1306_WIDTH / zoom;
	game->prev.height = SSD1306_HEIGHT / zoom;
}

void gameoflife_random(struct gameoflife *game) {
	for (int y = 0; y < SSD1306_HEIGHT / game->zoom; y++) {
		for (int x = 0; x < SSD1306_WIDTH / game->zoom; x++) {
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
	for (int y = 0; y < SSD1306_HEIGHT / game->zoom; y++) {
		for (int x = 0; x < SSD1306_WIDTH / game->zoom; x++) {
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