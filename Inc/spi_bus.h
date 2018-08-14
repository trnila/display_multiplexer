#pragma once
#include <FreeRTOS.h>
#include <semphr.h>
#include "stm32f1xx.h"

struct spi_dev {
	GPIO_TypeDef *port_cs;
	uint16_t pin_cs;

	GPIO_TypeDef *port_dc;
	uint16_t pin_dc;
};

void SPIBus_init(SPI_HandleTypeDef *spi);
void SPIBus_transmit(struct spi_dev *dev, GPIO_PinState dc_state, uint8_t *data, uint16_t size);