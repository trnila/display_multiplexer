#include <FreeRTOS.h>
#include <task.h>
#include <stm32f1xx.h>
#include "spi_bus.h"
#include "utils.h"

static SemaphoreHandle_t spibus_mutex;
static SPI_HandleTypeDef* spi;
static xTaskHandle *task;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	if(hspi == spi) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(task, &xHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void SPIBus_init(SPI_HandleTypeDef *spi_) {
	if((spibus_mutex = xSemaphoreCreateMutex()) == NULL) {
		HALT();
	}

	spi = spi_;
}

void SPIBus_transmit(struct spi_dev *dev, GPIO_PinState dc_state, uint8_t *data, uint16_t size) {
	if(xSemaphoreTake(spibus_mutex, portMAX_DELAY) != pdTRUE) {
		HALT();
	}

	task = xTaskGetCurrentTaskHandle();

	// set command pin
	HAL_GPIO_WritePin(dev->port_dc, dev->pin_dc, dc_state);

	// chip select
	HAL_GPIO_WritePin(dev->port_cs, dev->pin_cs, GPIO_PIN_RESET);

	if(HAL_SPI_Transmit_DMA(spi, data, size) != HAL_OK) {
		HALT();
	}

	// sleep until transfer complete
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	// disable chip select
	HAL_GPIO_WritePin(dev->port_cs, dev->pin_cs, GPIO_PIN_SET);

	if(xSemaphoreGive(spibus_mutex) != pdTRUE) {
		HALT();
	}
}