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

static struct list areas = LIST_INIT;


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

bool is_touched(struct touch_xpt2046 *touch, int *x, int *y)
{
    int i;
    int sum_x, sum_y, round_x, round_y;
    const int number = 32;
    bool is_touched;

    is_touched = touch->is_touched;
    touch->is_touched = FALSE;

    if (!is_touched)
        return FALSE;

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

    round_x = sum_x / number;
    round_y = sum_y / number;

    switch(touch->orient) {
    case DISP_ORIENT_PORTRAIT:
        touch->width = 320;
        touch->height = 480;
        *x = touch->width * round_y / 4096;
        *y = touch->height - touch->height * round_x / 4096;
        if (*x < 15 || *x > 305 || *y < 15 || *y > 465)
            return FALSE;
        break;
    case DISP_ORIENT_PORTRAIT_MIRROR:
        touch->width = 320;
        touch->height = 480;
        *x = touch->width - touch->width * round_y / 4096;
        *y = touch->height * round_x / 4096;
        if (*x < 15 || *x > 305 || *y < 15 || *y > 465)
            return FALSE;
        break;
    case DISP_ORIENT_LANDSCAPE:
        touch->width = 480;
        touch->height = 320;
        *x = touch->width * round_x / 4096;
        *y = touch->height * round_y / 4096;
        if (*x < 15 || *x > 465 || *y < 15 || *y > 305)
            return FALSE;
        break;
    case DISP_ORIENT_LANDSCAPE_MIRROR:
        touch->width = 480;
        touch->height = 320;
        *x = touch->width - touch->width * round_x / 4096;
        *y = touch->height - touch->height * round_y / 4096;
        if (*x < 15 || *x > 465 || *y < 15 || *y > 305)
            return FALSE;
        break;
    }
    return TRUE;
}

static void touch_area_destructor(void *mem)
{
    struct touch_area *ta = (struct touch_area *)mem;
    list_unlink(&ta->le);
}

struct touch_area *
touch_area_register(struct touch_xpt2046 *dev,
                    char *name,
                    int x1, int y1,
                    int x2, int y2)
{
    struct touch_area *ta;

    ta = kzref_alloc(name, sizeof *ta, touch_area_destructor);
    ta->dev = dev;
    ta->x1 = x1;
    ta->y1 = y1;
    ta->x2 = x2;
    ta->y2 = y2;
    list_append(&areas, &ta->le, ta);
    return ta;
}

bool is_area_touched(struct touch_area *ta)
{
    bool is_touched = ta->is_touched;
    ta->is_touched = FALSE;
    return is_touched;
}

void touch_handler(struct touch_xpt2046 *dev)
{
    int x, y;
    struct le *le;

    if(!is_touched(dev, &x, &y))
        return;

    LIST_FOREACH(&areas, le) {
        struct touch_area *ta = (struct touch_area *)list_ledata(le);
        if (ta->dev != dev)
            continue;
        if (x >= ta->x1 && x <= ta->x2 &&
                y >= ta->y1 && y <= ta->y2 &&
                (!ta->is_lock)) {
            ta->is_touched = TRUE;
            return;
        }
    }
}


static void touch_destructor(void *mem)
{
    struct touch_xpt2046 *touch = (struct touch_xpt2046 *)mem;
    kmem_deref(&touch->spi);
}

struct touch_xpt2046 *
touch_xpt2046_register(char *name, struct gpio *cs,
                       SPI_HandleTypeDef *hspi,
                       enum disp_orientation orient)
{
    struct touch_xpt2046 *dev;
    dev = kzref_alloc(name, sizeof *dev, touch_destructor);

    dev->spi = spi_dev_register(name, hspi, cs);
    dev->orient = orient;

    switch(orient) {
    case DISP_ORIENT_PORTRAIT:
        dev->width = 320;
        dev->height = 480;
        break;
    case DISP_ORIENT_PORTRAIT_MIRROR:
        dev->width = 320;
        dev->height = 480;
        break;
    case DISP_ORIENT_LANDSCAPE:
        dev->width = 480;
        dev->height = 320;
        break;
    case DISP_ORIENT_LANDSCAPE_MIRROR:
        dev->width = 480;
        dev->height = 320;
        break;
    }

    return dev;
}

void touch_enable(struct touch_xpt2046 *touch)
{
    irq_disable();
    touch->is_enabled = TRUE;
    irq_enable();
}

void touch_disable(struct touch_xpt2046 *touch)
{
    irq_disable();
    touch->is_enabled = FALSE;
    irq_enable();
}
