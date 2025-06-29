/*
 * abs_position.c
 *
 *  Created on: Jun 17, 2025
 *      Author: stelhs
 */
#include "abs_position.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/crc32.h"

struct abs_position *
abs_position_dev_register(char *name, SPI_HandleTypeDef *hspi,
                          struct gpio *cs)
{
    struct abs_position *dev;
    dev = kzref_alloc("abs_position", sizeof *dev, NULL);
    dev->dev = spi_dev_register(name, hspi, cs);
    dev->raw.cross = 0; // TODO
    dev->raw.longitudal = 0;
    dev->is_cross_inc_down = TRUE;
    dev->is_longitudal_inc_left = TRUE;
    return dev;
}


void abs_position_update(struct abs_position *ap)
{
//    return; // TODO
    u8 tx[12];
    u8 rx[12];
    memset(tx, 0, 12);
    u32 *raw_longitudal = (u32 *)rx;
    u32 *raw_cross = (u32 *)(rx + 4);
    u32 *crc = (u32 *)(rx + 8);
    u32 crc_calc;
    spi_send_cs_activate(ap->dev);
    //sleep(1);
    delay_us(500);
    spi_send_recv_sync(ap->dev, tx, rx, 12);

//    printf("raw_longitudal = %lu\r\n", *raw_longitudal);
//    printf("raw_cross = %lu\r\n", *raw_cross);
//    printf("crc = %lu\r\n", *crc);
    crc_calc = crc32(rx, 8);
//    printf("crc_calc = %lu\r\n", crc_calc);
    if (*crc != crc_calc) {
        printf("CRC ERR\r\n");
        return;
    }
    ap->raw.longitudal = (int)*raw_longitudal * LINEAR_LONGITUDAL_RESOLUTION;
    ap->raw.cross = (int)*raw_cross * LINEAR_CROSS_RESOLUTION;
}

int abs_longitudal(struct abs_position *ap, int tool_num)
{
    int offset = ap->offset_tools[tool_num].longitudal;
    if (ap->is_longitudal_inc_left)
        return (int)(0xFFFFFFFF - ap->raw.longitudal) + 1 + offset;
    return ap->raw.longitudal - offset;
}

int abs_longitudal_curr_tool(struct abs_position *ap)
{
    return abs_longitudal(ap, ap->curr_tool_num);
}

int abs_cross(struct abs_position *ap, int tool_num)
{
    int val;
    int offset = ap->offset_tools[tool_num].cross;
    if (ap->is_cross_inc_down)
        val = (int)(0xFFFFFFFF - ap->raw.cross) + 1 + offset;
    else
        val = ap->raw.cross - offset;

    return val * 2; // to diameter
}

int abs_cross_curr_tool(struct abs_position *ap)
{
    return abs_cross(ap, ap->curr_tool_num);
}

void abs_cross_set(struct abs_position *ap, int tool_num, int val)
{
    val /= 2; // to radius
    if (ap->is_cross_inc_down) {
        ap->offset_tools[tool_num].cross =
            val - (int)(0xFFFFFFFF - ap->raw.cross + 1);
        return;
    }
    ap->offset_tools[tool_num].cross = ap->raw.cross - val;
}

void abs_longitudal_set(struct abs_position *ap, int tool_num, int val)
{
    if (ap->is_longitudal_inc_left) {
        ap->offset_tools[tool_num].longitudal =
            val - (int)(0xFFFFFFFF - ap->raw.longitudal + 1);
        return;
    }
    ap->offset_tools[tool_num].longitudal =
        ap->raw.longitudal - val;
}

void abs_pos_set_tool(struct abs_position *ap, int tool_num)
{
    ap->curr_tool_num = tool_num;
}

int abs_pos_tool(struct abs_position *ap)
{
    return ap->curr_tool_num;
}
