/*
 * disp_ili9488.h
 *
 *  Created on: May 22, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_DISP_MIPI_DCS_H_
#define SRC_MACHINE_DISP_MIPI_DCS_H_

#include "stm32_lib/gpio.h"
#include "spi_dev.h"


struct disp {
    struct gpio *reset;
    struct gpio *dc_rs;
    struct spi_dev *spi;
};

struct __attribute__((packed)) color {
    u8 b;
    u8 g;
    u8 r;
};

#define BLACK (struct color){.r=0, .g=0, .b=0}
#define YELLOW (struct color){.r=255, .g=255, .b=0}
#define GREEN (struct color){.r=0, .g=255, .b=0}
#define RED (struct color){.r=255, .g=0, .b=0}
#define BLUE (struct color){.r=0, .g=0, .b=255}

extern u8 font_rus[];

struct text_style {
    u8 *font;
    int fontsize;
    struct color color;
    struct color bg_color;
};

struct img {
    struct buf *buf;
    int width;
    int height;
};

struct disp *disp_register(char *name, struct gpio *cs,
                           struct gpio *reset,
                           struct gpio *dc_rs,
                           SPI_HandleTypeDef *hspi);

void disp_init(struct disp *disp);
void disp_fill_img(struct disp *disp, int x, int y, struct img *img);

void disp_fill(struct disp *disp,
               int x, int y,
               int width, int height,
               struct color color);

void disp_clear(struct disp *disp);

void disp_rect(struct disp *disp,
               int x, int y,
               int width, int height,
               int thickness,
               struct color color);

void disp_text(struct disp *disp, char *text,
               int x, int y,
               struct text_style *style);

void disp_line(struct disp *disp,
               int x0, int y0, int x1, int y1,
               int thickness, struct color color);

struct img *img_alloc(char *name, int width, int height);

struct img *font_symbol_img(char ch, u8 *font, int fontsize,
                            struct color color, struct color bg_color);



#endif /* SRC_MACHINE_DISP_MIPI_DCS_H_ */
