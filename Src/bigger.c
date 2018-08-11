#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "lcd.h"
#include "images/logo_small.h"

void falling_logo() {
	const uint8_t pos[][2] = {
			{30, 10},
			{100, 40},
			{180, 110},
			{70, 20}
	};


	LCD_Clear(COLOR_BLACK);
	for(int i = 0; i < sizeof(pos) / sizeof(*pos); i++) {
		LCD_DrawBitmap(image_logo_small_data, pos[i][0], pos[i][1], image_logo_small_width, image_logo_small_height);
	}

	// scroll whole screen
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
	LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);

	int i = 0;
	for(;;) {
		LCD_WriteReg(ILI9225_VERTICAL_SCROLL_CTRL3, i);

		i += 10;
		if(i > 0xDB) {
			i = 0;
		}

		vTaskDelay(100);
	}
}

void task_ILI9225(void *param) {
	LCD_Init();

	falling_logo();
}