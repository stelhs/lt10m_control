/*
 * st7796s.c
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */
#include "disp_st7796s.h"

#include "stm32f407xx.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/buf.h"
#include "spi_dev.h"


static void disp_destructor(void *mem)
{
    struct disp_st7796s *disp = (struct disp_st7796s *)mem;
    UNUSED(disp);
}

struct disp_st7796s *
disp_st7796s_register(char *name, struct gpio *cs, struct gpio *reset,
				      struct gpio *dc_rs, SPI_HandleTypeDef *hspi)
{
	struct disp_st7796s *disp;
	disp = kzref_alloc(name, sizeof *disp, disp_destructor);
    if (!disp)
        return NULL;

    disp->spi = spi_dev_register(name, hspi, cs);
    disp->reset = reset;
    disp->dc_rs = dc_rs;

    disp_st7796s_init(disp);
    return disp;
}

static void cmd_send(struct disp_st7796s *disp, u8 cmd)
{
	gpio_down(disp->dc_rs);
	spi_send_sleep(disp->spi, &cmd, 1);
}

static void data_send(struct disp_st7796s *disp, struct buf *data)
{
	gpio_up(disp->dc_rs);
	spi_send_sleep(disp->spi, data->d, buf_len(data));
}

void disp_st7796s_update(struct disp_st7796s *disp, u8 x, u8 y,
		            u8 width, u8 height, struct buf *data)
{
	cmd_send(disp, 0x21); // Set Column Address
	cmd_send(disp, x); // Start column
	cmd_send(disp, x + width - 1); // End column

	cmd_send(disp, 0x22); // Set Page Address
	cmd_send(disp, y); // Start page
	cmd_send(disp, y + height / 8 - 1); // End page

    data_send(disp, data);
}

void disp_st7796s_init(struct disp_st7796s *disp)
{
    gpio_up(disp->reset);
    sleep(50);
    gpio_down(disp->reset);
    sleep(50);
    gpio_up(disp->reset);

    cmd_send(disp, 0xAE); // Display OFF
    cmd_send(disp, 0x20); // Set Memory Addressing Mode
    cmd_send(disp, 0x00); // Horizontal Addressing Mode
    cmd_send(disp, 0xAF); // Display ON
}


