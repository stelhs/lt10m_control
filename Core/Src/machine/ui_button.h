/*
 * disp_button.h
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */

#ifndef UI_BUTTON_H_
#define UI_BUTTON_H_

#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "ui_item.h"

struct ui_button {
    char *name;
    struct le le;
    struct ui_item *ut;
    struct disp *disp;
    struct touch_area *ta;
    void (*on_click)(void *);
    void *priv;
};

struct ui_button *
ui_button_register(char *name,
                   struct disp *disp,
                   int x, int y, int width, int height,
                   void (*show)(struct ui_item *),
                   void (*on_click)(void *), void *priv);

bool is_ui_button_touched(struct ui_button *db);
void ui_button_hide(struct ui_button *ub);
void ui_button_show(struct ui_button *ub);
void ui_button_handler(void);

#endif /* UI_BUTTON_H_ */
