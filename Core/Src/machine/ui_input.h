/*
 * ui_num_input.h
 *
 *  Created on: Jul 12, 2025
 *      Author: stelhs
 */

#ifndef UI_NUM_INPUT_H_
#define UI_NUM_INPUT_H_
#include "ui_button.h"

extern struct text_style input_ts;

struct ui_item *
ui_input_register(char *name,
                  struct ui_scope *ui_scope,
                  char *input_text,
                  int x, int y,
                  int *val,
                  int min, int max, int step,
                  bool is_integer,
                  int max_str_len,
                  char *view_format,
                  char *prefix,
                  struct img*(*img)(void));

void ui_input_set_onchanged_cb(struct ui_item *ut,
                               void (*onchanged)(void *),
                               void *priv);

#endif /* UI_NUM_INPUT_H_ */
