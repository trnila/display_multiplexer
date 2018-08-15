#include <memory.h>
#include "utils.h"
#include "bitmatrix.h"
#include "ssd1306.h"

void bitmatrix_set(struct bitmatrix *bits, uint8_t x, uint8_t y, uint8_t val) {
	if(x < 0 || y < 0 || x >= bits->width || y >= bits->height) {
		return;
	}

	int index = x + (y / 8) * bits->width;

	if(val) {
		bits->data[index] |= 1 << (y % 8);
	} else {
		bits->data[index] &= ~(1 << (y % 8));
	}
}

uint8_t bitmatrix_get(struct bitmatrix *bits, uint8_t x, uint8_t y) {
	if(x < 0 || y < 0 || x >= bits->width || y >= bits->height) {
		return 0;
	}

	return (bits->data[x + (y / 8) * bits->width] & (1 << (y % 8))) > 0;
}

void bitmatrix_clear(struct bitmatrix *bits) {
	memset(bits->data, 0, bits->height * bits->width / 8);
}