#include <stm32f1xx.h>
#include <FreeRTOS.h>
#include <task.h>
#include "lcd.h"
#include "images/logo.h"

void app() {
	LCD_Init();
	LCD_Clear(COLOR_WHITE);
	LCD_DrawBitmap(image_logo_data, 0, 0, image_logo_width, image_logo_height);
	for(;;);
}