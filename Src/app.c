#include <stm32f1xx.h>
#include <FreeRTOS.h>
#include <stdlib.h>
#include <task.h>
#include <memory.h>
#include "lcd.h"
#include "utils.h"
#include "ssd1306.h"
#include "spi_bus.h"

void task_ILI9225(void *param);
void task_SSD1306(void *param);

void app() {
	SPIBus_init(&hspi1);
	LCD_Reset();

	ASSERT(xTaskCreate(task_ILI9225, "ili9225", 128, NULL, configMAX_PRIORITIES - 1, NULL) == pdPASS);
	task_SSD1306(NULL);
}