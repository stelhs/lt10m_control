/*
 * touch_xpt2046.c
 *
 *  Created on: May 24, 2025
 *      Author: stelhs
 */
#include "stm32f407xx.h"
#include "touch_xpt2046.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/timers.h"


static void touch_destructor(void *mem)
{
    struct touch_xpt2046 *touch = (struct touch_xpt2046 *)mem;
    kmem_deref(&touch->spi);
}

struct touch_xpt2046 *
touch_xpt2046_register(char *name, struct gpio *cs, SPI_HandleTypeDef *hspi)
{
    struct touch_xpt2046 *touch;
    touch = kzref_alloc(name, sizeof *touch, touch_destructor);

    touch->spi = spi_dev_register(name, hspi, cs);
    timeout_start(&touch->t, 300);
    return touch;
}

static u16 read_x(struct touch_xpt2046 *touch)
{
    u8 tx_data[3];
    u8 rx_data[3];

    tx_data[0] = 0x90;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;
    spi_send_recv_sync(touch->spi, tx_data, rx_data, 3);
    return ((rx_data[1] << 8) | rx_data[2]) >> 3;
}

static u16 read_y(struct touch_xpt2046 *touch)
{
    u8 tx_data[3];
    u8 rx_data[3];

    tx_data[0] = 0xD0;
    tx_data[1] = 0x00;
    tx_data[2] = 0x00;
    spi_send_recv_sync(touch->spi, tx_data, rx_data, 3);
    return ((rx_data[1] << 8) | rx_data[2]) >> 3;
}

void touch_read(struct touch_xpt2046 *touch)
{
    int i;
    int sum_x;
    int sum_y;
    const int number = 32;
    struct item {
        u16 x;
        u16 y;
    }queue[number];



    for (i = 0; i < number; i++) {
        queue[i].x = read_x(touch);
        queue[i].y = read_y(touch);
    }

    sum_x = 0;
    sum_y = 0;
    for (i = 0; i < number; i++) {
        sum_x += queue[i].x;
        sum_y += queue[i].y;
    }

    touch->x = sum_x / number;
    touch->y = sum_y / number;
    touch->is_touched = TRUE;

}

