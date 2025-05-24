/*
 * spi_dev.h
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */

#ifndef SRC_SPI_DEV_H_
#define SRC_SPI_DEV_H_

#include "stm32_lib/klist.h"
#include "stm32f4xx_hal.h"

struct spi_dev {
	struct le le;
	SPI_HandleTypeDef *hspi;
	struct gpio *cs;
	bool is_finished;
};

struct spi_dev *spi_dev_register(char *name, SPI_HandleTypeDef *hspi,
								 struct gpio *cs);

void spi_send_sleep(struct spi_dev *dev, u8 *data, size_t len);
void spi_send_sync(struct spi_dev *dev, u8 *data, size_t len);

#endif /* SRC_SPI_DEV_H_ */
