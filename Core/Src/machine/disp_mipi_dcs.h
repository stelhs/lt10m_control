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

enum disp_orientation {
    DISP_ORIENT_PORTRAIT,
    DISP_ORIENT_PORTRAIT_MIRROR,
    DISP_ORIENT_LANDSCAPE,
    DISP_ORIENT_LANDSCAPE_MIRROR,
};

struct disp {
    struct gpio *dc_rs;
    struct spi_dev *spi;
    enum disp_orientation orient;
    int width;
    int height;
    struct touch_xpt2046 *touch;
};

struct __attribute__((packed)) color {
    u8 r;
    u8 g;
    u8 b;
};

#define BLACK (struct color){0, 0, 0}
#define YELLOW (struct color){255, 255, 0}
#define GREEN (struct color){0, 255, 0}
#define DARK_GREEN (struct color){0, 127, 0}
#define EMERALD (struct color){0, 255, 255}
#define LIGHT_GRAY (struct color){200, 200, 200}
#define GRAY (struct color){127, 127, 127}
#define DARK_GRAY (struct color){50, 50, 50}
#define RED (struct color){255, 0, 0}
#define BLUE (struct color){0, 0, 255}
#define DARK_BLUE (struct color){0, 0, 64}

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
                           struct gpio *dc_rs,
                           SPI_HandleTypeDef *hspi,
                           enum disp_orientation orient);

void disp_init(struct disp *disp);
void disp_set_orientation(struct disp *disp, enum disp_orientation orient);
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

int disp_text_width(struct text_style *ts, int text_len);
int disp_text_height(struct text_style *ts);


#endif /* SRC_MACHINE_DISP_MIPI_DCS_H_ */
