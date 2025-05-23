/*
 * disp_ili9488.h
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_DISP_ILI9488_H_
#define SRC_MACHINE_DISP_ILI9488_H_

#include "stm32_lib/gpio.h"
#include "spi_dev.h"


struct disp_ili9488 {
	struct gpio *reset;
	struct gpio *dc_rs;
	struct spi_dev *spi;
};

struct __attribute__((packed)) color {
	u8 b;
	u8 g;
	u8 r;
};

struct disp_ili9488 *
disp_ili9488_register(char *name, struct gpio *cs, struct gpio *reset,
				      struct gpio *dc_rs, SPI_HandleTypeDef *hspi);

void disp_ili9488_init(struct disp_ili9488 *disp);
void disp_ili9488_update_buf(struct disp_ili9488 *disp, int x, int y,
						 int width, int height, struct buf *data);

void disp_ili9488_fill(struct disp_ili9488 *disp,
               int x, int y,
               int width, int height,
               struct color color);

void disp_ili9488_clear(struct disp_ili9488 *disp);

#endif /* SRC_MACHINE_DISP_ILI9488_H_ */
