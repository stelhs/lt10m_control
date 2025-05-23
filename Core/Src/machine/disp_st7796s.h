/*
 * st7796s.h
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_DISP_ST7796S_H_
#define SRC_MACHINE_DISP_ST7796S_H_

#include "stm32_lib/gpio.h"
#include "spi_dev.h"

struct disp_st7796s {
	struct gpio *reset;
	struct gpio *dc_rs;
	struct spi_dev *spi;
};

struct disp_st7796s *
disp_st7796s_register(char *name, struct gpio *cs, struct gpio *reset,
				      struct gpio *dc_rs, SPI_HandleTypeDef *hspi);

void disp_st7796s_init(struct disp_st7796s *disp);
void disp_st7796s_update(struct disp_st7796s *disp, u8 x, u8 y, u8 width, u8 height, struct buf *data);


#endif /* SRC_MACHINE_DISP_ST7796S_H_ */
