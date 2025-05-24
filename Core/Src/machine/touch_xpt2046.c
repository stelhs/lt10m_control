/*
 * touch_xpt2046.c
 *
 *  Created on: May 24, 2025
 *      Author: stelhs
 */
#include "stm32f407xx.h"
#include "touch_xpt2046.h"
#include "stm32_lib/kref_alloc.h"


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

    touch_init(touch);
    return touch;
}
