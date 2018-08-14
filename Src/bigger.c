#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <math.h>
#include "lcd.h"
#include "images/logo_white.h"

#define MAX_OFFSET 0xDB

void falling_logo() {
	const uint8_t pos[][2] = {
			{30, 10},
			{100, 40},
			{180, 110},
			{70, 20}
	};


	LCD_Clear(COLOR_WHITE);
	for(int i = 0; i < sizeof(pos) / sizeof(*pos); i++) {
		LCD_DrawBitmap(image_logo_white_data, pos[i][0], pos[i][1], image_logo_white_width, image_logo_white_height);
	}

	// scroll whole screen
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);

	int i = MAX_OFFSET;
	for(;;) {
		LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL3, i);

		i -= 10;
		if(i < 0) {
			i = MAX_OFFSET;
		}

		vTaskDelay(100);
	}
}

// https://rosettacode.org/wiki/Chaos_game#C
void sierpinski_chaos(float offset, int side, int iters) {
	LCD_Clear(COLOR_BLACK);

	int vertices[3][3];
	for(int i = 0; i < 3; i++) {
		vertices[i][0] = ILI9225_LCD_WIDTH / 2 + side * cos(i * 2 * 3.14 / 3 + 3.14 / 2);
		vertices[i][1] = ILI9225_LCD_HEIGHT / 2 + side * sin(i * 2 * 3.14 / 3 + 3.14 / 2);
		LCD_DrawPoint(vertices[i][0], vertices[i][1], COLOR_WHITE);
	}


	int seedX = rand() % (vertices[0][0] / 2 + (vertices[1][0] + vertices[2][0]) / 4);
	int seedY = rand() % (vertices[0][1] / 2 + (vertices[1][1] + vertices[2][1]) / 4);
	LCD_DrawPoint(seedX, seedY, COLOR_WHITE);

	uint16_t color = COLOR_WHITE;
	for(;;) {
		for (int i = 0; i < iters; i++) {
			int choice = rand() % 3;

			seedX = (seedX + vertices[choice][0]) / 2;
			seedY = (seedY + vertices[choice][1]) / 2;

			LCD_DrawPoint(seedX, seedY, color);

			vTaskDelay(1);
		}

		color = ~color;
	}
}


void task_ILI9225(void *param) {
	LCD_Init();

	falling_logo();
	sierpinski_chaos(3.14 / 2, 78, 10000);

	falling_logo();
}