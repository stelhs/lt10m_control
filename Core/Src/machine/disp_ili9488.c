/*
 * disp_ili9488.c
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */
#include "disp_ili9488.h"

#include "stm32f407xx.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/buf.h"
#include "spi_dev.h"


static void disp_destructor(void *mem)
{
    struct disp_ili9488 *disp = (struct disp_ili9488 *)mem;
    UNUSED(disp);
}

struct disp_ili9488 *
disp_ili9488_register(char *name, struct gpio *cs, struct gpio *reset,
                      struct gpio *dc_rs, SPI_HandleTypeDef *hspi)
{
    struct disp_ili9488 *disp;
    disp = kzref_alloc(name, sizeof *disp, disp_destructor);

    disp->spi = spi_dev_register(name, hspi, cs);
    disp->reset = reset;
    disp->dc_rs = dc_rs;

    disp_ili9488_init(disp);
    return disp;
}

static void cmd_send(struct disp_ili9488 *disp, u8 cmd)
{
    gpio_down(disp->dc_rs);
    spi_send_sleep(disp->spi, &cmd, 1);
}

static void data_send(struct disp_ili9488 *disp, u8 *data, size_t len)
{
    gpio_up(disp->dc_rs);
    spi_send_sleep(disp->spi, data, len);
}

static void data_send_buf(struct disp_ili9488 *disp, struct buf *data)
{
    data_send(disp, data->d, buf_len(data));
}

static void set_window(struct disp_ili9488 *disp, int x, int y,
                       int width, int height)
{
    u8 buf[4];
    cmd_send(disp, 0x2A); // CASET
    buf[0] = (x >> 8) & 0xFF;
    buf[1] = x & 0xFF;
    buf[2] = ((x + width - 1) >> 8) & 0xFF;
    buf[3] = (x + width - 1) & 0xFF;
    data_send(disp, buf, 4);

    cmd_send(disp, 0x2B); // RASET
    buf[0] = (y >> 8) & 0xFF;
    buf[1] = y & 0xFF;
    buf[2] = ((y + height - 1) >> 8) & 0xFF;
    buf[3] = (y + height - 1) & 0xFF;
    data_send(disp, buf, 4);

    cmd_send(disp, 0x2c);
}

void disp_ili9488_update_buf(struct disp_ili9488 *disp, int x, int y,
                         int width, int height, struct buf *data)
{
    set_window(disp, x, y, width, height);


}

void disp_ili9488_fill(struct disp_ili9488 *disp,
               int x, int y,
               int width, int height,
               struct color color)
{
    int i;
    struct color *c;
    struct buf *row;

    row = buf_alloc("row", width * sizeof (struct color));
    c = (struct color *)row->d;
    for (i = 0; i < width; i++, c++)
        *c = color;
    buf_put(row, width * sizeof (struct color));

    set_window(disp, x, y, width, height);
    for (i = 0; i < height; i++)
        data_send_buf(disp, row);
    kmem_deref(&row);
}

void disp_ili9488_clear(struct disp_ili9488 *disp)
{
    disp_ili9488_fill(disp, 0, 0, 480, 320, (struct color){0, 0, 0});
}


void disp_ili9488_init(struct disp_ili9488 *disp)
{
    gpio_up(disp->reset);
    sleep(50);
    gpio_down(disp->reset);
    sleep(50);
    gpio_up(disp->reset);
    sleep(50);

    // Сброс дисплея
    cmd_send(disp, 0x01); // Software Reset
    sleep(50);

    cmd_send(disp, 0x11); // SLEEP OUT
    sleep(50);

    {
        cmd_send(disp, 0x3A); // COLMOD: pixel format
        u8 data = 0x66; // 18 bit per pixel (RGB666)
        data_send(disp, &data, 1);
    }

 /*   {
        cmd_send(disp, 0xC2); // Power Control 3
        u8 data = 0x44;
        data_send(disp, &data, 1);
    }

    {
        u8 buf[4];
        buf[0] = 0x00;
        buf[1] = 0x00;
        buf[2] = 0x00;
        buf[2] = 0x00;
        cmd_send(disp, 0XC5);      //Power Control
        data_send(disp, buf, 4);
    }

    {
        uint8_t datas[] = {
            0x0F, 0x1F, 0x1C, 0x0C,
            0x0F, 0x08, 0x48, 0x98,
            0x37, 0x0A, 0x13, 0x04,
            0x11, 0x0D, 0x00};
        cmd_send(disp, 0XE0);
        data_send(disp, datas, sizeof(datas));
    }

    {
        uint8_t datas[] = {
            0x0F, 0x32, 0x2E, 0x0B,
            0x0D, 0x05, 0x47, 0x75,
            0x37, 0x06, 0x10, 0x03,
            0x24, 0x20, 0x00};

        cmd_send(disp, 0XE1);
        data_send(disp, datas, sizeof(datas));
    }*/

    {
        cmd_send(disp, 0x36); // MADCTL: setup orientation
        u8 data = 0x20;
        data_send(disp, &data, 1);
    }

    cmd_send(disp, 0x20);
    cmd_send(disp, 0x29); // DISPON: display enable

    disp_ili9488_clear(disp);
}
