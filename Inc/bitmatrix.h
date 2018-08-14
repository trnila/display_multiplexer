#pragma once
#include "ssd1306.h"

struct bitmatrix {
	uint8_t width;
	uint8_t height;
	uint8_t *data;
};

void bitmatrix_set(struct bitmatrix *data, uint8_t x, uint8_t y, uint8_t val);
uint8_t bitmatrix_get(struct bitmatrix *data, uint8_t x, uint8_t y);
void bitmatrix_clear(struct bitmatrix *bits);
