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

struct ui_item *
ui_button_register(char *name, struct list *ui_scope,
                   int x, int y, int width, int height,
                   void (*show)(struct ui_item *),
                   void (*on_click)(struct ui_item *), void *priv,
                   size_t data_size);

bool is_ui_button_touched(struct ui_item *);
void ui_button_handler(void);
void *ui_button_data(struct ui_item *ut);

#endif /* UI_BUTTON_H_ */
