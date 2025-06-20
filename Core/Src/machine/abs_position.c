/*
 * abs_position.c
 *
 *  Created on: Jun 17, 2025
 *      Author: stelhs
 */

#include "abs_position.h"
#include "stm32_lib/kref_alloc.h"

struct abs_position *
abs_position_dev_register(char *name, SPI_HandleTypeDef *hspi,
                          struct gpio *cs)
{
    struct abs_position *dev;
    dev = kzref_alloc("abs_position", sizeof *dev, NULL);
    dev->dev = spi_dev_register(name, hspi, cs);
    dev->raw.cross = 0; // TODO
    dev->raw.longitudal = 0;
    return dev;
}


void abs_position_update(struct abs_position *ap)
{
    return; // TODO
    u8 tx[8];
    u8 rx[8];
    u32 *raw_longitudal = (u32 *)rx;
    u32 *raw_cross = (u32 *)(rx + 4);
    spi_send_recv_sync(ap->dev, tx, rx, 8);
    ap->raw.longitudal = (int)*raw_longitudal * LINEAR_LONGITUDAL_RESOLUTION;
    ap->raw.cross = (int)*raw_cross * LINEAR_CROSS_RESOLUTION;
}


int abs_longitudal(struct abs_position *ap, int tool_num)
{
    int offset = ap->offset_tools[tool_num].longitudal;

    if (ap->is_longitudal_inc_right) {
        int pos = (int)(0xFFFFFFFF - (u32)ap->raw.longitudal);
        offset = (int)(0xFFFFFFFF - (u32)offset);
        return pos - offset;
    }

    return ap->raw.longitudal - offset;
}

int abs_cross(struct abs_position *ap, int tool_num)
{
    int offset = ap->offset_tools[tool_num].cross;

    if (ap->is_cross_inc_up) {
        int pos = (int)(0xFFFFFFFF - (u32)ap->raw.cross);
        offset = (int)(0xFFFFFFFF - (u32)offset);
        return pos - offset;
    }

    return ap->raw.cross - offset;
}

void abs_cross_set(struct abs_position *ap, int tool_num, int val)
{
    ap->offset_tools[tool_num].cross = (int)ap->raw.cross - val;
}

void abs_longitudal_set(struct abs_position *ap, int tool_num, int val)
{
    ap->offset_tools[tool_num].longitudal = (int)ap->raw.longitudal - val;
}

