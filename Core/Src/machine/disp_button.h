/*
 * disp_button.h
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */

#ifndef DISP_BUTTON_H_
#define DISP_BUTTON_H_

#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"

struct disp_button {
    struct disp *disp;
    struct touch_area *ta;
    int x;
    int y;
    char *name;
    int width;
    int height;
    void (*draw)(struct disp_button *db);
    void *priv;
};

struct disp_button *
disp_button_create(char *name,
                   struct disp *disp,
                   int width, int height,
                   size_t priv_size);
void disp_button_show(struct disp_button *db, int x, int y,
                        void (*draw)(struct disp_button *db));
bool is_disp_button_touched(struct disp_button *db);

#endif /* DISP_BUTTON_H_ */
