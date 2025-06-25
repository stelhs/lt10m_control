/*
 * ui_item_blink.h
 *
 *  Created on: Jun 24, 2025
 *      Author: stelhs
 */

#ifndef UI_ITEM_H_
#define UI_ITEM_H_

#include "stm32_lib/timers.h"

struct ui_item {
    struct disp *disp;
    int x;
    int y;
    int width;
    int height;
    void (*show)(struct ui_item *);
    void (*hide)(struct ui_item *);
    void *priv;
    int interval;
    bool is_blink_hide;
    bool is_show;
    struct timer_worker *tw;
};

struct ui_item *
ui_item_register(char *name, struct disp *disp,
                 int x, int y, int width, int height,
                 void (*show)(struct ui_item *),
                 void (*hide)(struct ui_item *), void *priv);

void ui_item_blink(struct ui_item *ut, int interval);
void ui_item_blink_stop(struct ui_item *ut);
void ui_item_hide(struct ui_item *ut);
void ui_item_show(struct ui_item *ut);
bool ui_item_is_show(struct ui_item *ut);

#endif /* UI_ITEM_H_ */
