#include <utils.h>
#include "ssd1306.h"

void bitmatrix_set(uint8_t *data, uint8_t x, uint8_t y, uint8_t val) {
	if(x < 0 || y < 0 || x >= SSD1306_WIDTH/2 || y >= SSD1306_HEIGHT/2) {
		return;
	}

	int index = x + (y / 8) * SSD1306_WIDTH/2;

	ASSERT(index >= 256);

	if(val) {
		data[index] |= 1 << (y % 8);
	} else {
		data[index] &= ~(1 << (y % 8));
	}
}

uint8_t bitmatrix_get(uint8_t *data, uint8_t x, uint8_t y) {
	if(x < 0 || y < 0 || x >= SSD1306_WIDTH/2 || y >= SSD1306_HEIGHT/2) {
		return 0;
	}

	return (data[x + (y / 8) * SSD1306_WIDTH/2] & (1 << (y % 8))) > 0;
}