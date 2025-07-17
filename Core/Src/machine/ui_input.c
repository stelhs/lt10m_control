/*
 * ui_input.c
 *
 *  Created on: Jul 12, 2025
 *      Author: stelhs
 */
#include <string.h>
#include "ui_input.h"
#include "ui_keyboard.h"
#include "stm32_lib/kref_alloc.h"

struct ui_input {
    char *text;
    struct ui_scope *ui_scope;
    int *val;
    int min;
    int max;
    bool is_integer;
    int step;
    int digit_num;
    char format[8];
    struct img *(*img)(void);
    char *prefix;
    void (*onchanged)(void *);
    void *priv;
};

struct text_style input_ts = {
        .bg_color = BLACK,
        .color = GREEN,
        .font = font_rus,
        .fontsize = 3
};


static void input_show(struct ui_item *ut)
{
    struct ui_input *input = (struct ui_input *)ui_button_data(ut);
    int text_left_padding = 3;
    char string[32];
    int prefix_len = 0;

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (input->img) {
        struct img *img = input->img();
        disp_fill_img(ut->disp, ut->x + 5, ut->y + 10, img);
        kmem_deref(&img);
        text_left_padding += 70;
    }

    if (input->prefix) {
        prefix_len = strlen(input->prefix);
        strcpy(string, input->prefix);
    }

    if (input->is_integer)
        snprintf(string + prefix_len, input->digit_num,
                 input->format, *input->val);
    else
        snprintf(string + prefix_len, input->digit_num,
                 input->format, (float)(*input->val) / 1000);

    disp_text(ut->disp, string, ut->x + text_left_padding,
              ut->y + 15, &input_ts);
}

static int input_onclick(struct ui_item *ut)
{
    struct ui_input *input = (struct ui_input *)ut->priv;

    ui_scope_hide(input->ui_scope);
    ui_keyboard_run(input->text, input->val,
                    input->min, input->max,
                    input->step, input->is_integer);

    if (input->onchanged)
        input->onchanged(input->priv);
    ui_scope_show(input->ui_scope);
    return FALSE;
}



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
                  struct img*(*img)(void))
{
    struct ui_input *input;
    struct ui_item *ut;
    int str_len = max_str_len;
    int width;

    width = disp_text_width(&input_ts, str_len) + 2 + 2;
    if (img)
        width += 70;

    ut = ui_button_register(name, ui_scope,
                            x, y, width, 55,
                            input_show,
                            input_onclick, NULL, sizeof *input);
    input = (struct ui_input *)ui_button_data(ut);
    ut->priv = input;
    input->ui_scope = ui_scope;
    input->text = input_text;
    if (*val < min || *val > max)
        *val = min;
    if (*val % step)
        *val -= (*val % step);
    input->val = val;
    input->min = min;
    input->max = max;
    input->is_integer = is_integer;
    input->step = step;
    input->digit_num = max_str_len;
    strcpy(input->format, view_format);
    input->img = img;
    input->prefix = prefix;
    ui_item_show(ut);
    return ut;
}

void ui_input_set_onchanged_cb(struct ui_item *ut,
                               void (*onchanged)(void *),
                               void *priv)
{
    struct ui_input *input = (struct ui_input *)ui_button_data(ut);
    input->priv = priv;
    input->onchanged = onchanged;
}

