#pragma once
#include "ssd1306.h"

void bitmatrix_set(uint8_t *data, uint8_t x, uint8_t y, uint8_t val);
uint8_t bitmatrix_get(uint8_t *data, uint8_t x, uint8_t y);