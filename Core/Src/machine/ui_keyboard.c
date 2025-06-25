/*
 * disp_keyboard.c
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */

#include "ui_keyboard.h"

#include "stm32_lib/kref_alloc.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "machine.h"
#include "ui_button.h"

struct ui_keyboard {
    struct disp *disp_info;
    struct disp *disp_touch;
    struct ui_button *keys_num[10];
    struct ui_button *key_point;
    struct ui_button *key_del;
    struct ui_button *key_minus;
    struct ui_button *key_ok;
    struct ui_button *key_cancel;
    int min;
    int max;
    int step;
    int max_len;
};

static struct ui_keyboard *ui_keyboard = NULL;

static struct text_style key_text_style = {
        .bg_color = BLACK,
        .color = BLUE,
        .font = font_rus,
        .fontsize = 3
};

static void key_num_show(struct ui_item *ut)
{
    struct ui_button *ub = (struct ui_button *)ut->priv;
    int key_num = (int)ub->priv;
    char str[3];

    sprintf(str, "%d", key_num);
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);
    disp_text(ut->disp, str,
            ut->x + ut->width / 2 - 10,
            ut->y + ut->height / 2 - 10,
            &key_text_style);
}

static void key_point_show(struct ui_item *ut)
{
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);
    disp_text(ut->disp, ".",
            ut->x + ut->width / 2 - 10,
            ut->y + ut->height / 2 - 10,
            &key_text_style);
}

static void key_del_show(struct ui_item *ut)
{
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);
    disp_text(ut->disp, "<-",
            ut->x + ut->width / 2 - 15,
            ut->y + ut->height / 2 - 10,
            &key_text_style);
}

static void key_ok_show(struct ui_item *ut)
{
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GREEN);
    disp_text(ut->disp, "OK",
            ut->x + ut->width / 2 - 15,
            ut->y + ut->height / 2 - 10,
            &ts);
}

static void key_esc_show(struct ui_item *ut)
{
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = RED,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, RED);
    disp_text(ut->disp, "Esc",
            ut->x + ut->width / 2 - 25,
            ut->y + ut->height / 2 - 10,
            &ts);
}

static void key_minus_show(struct ui_item *ut)
{
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);
    disp_text(ut->disp, "-",
            ut->x + ut->width / 2 - 7,
            ut->y + ut->height / 2 - 10,
            &key_text_style);
}


static void show(struct ui_keyboard *uk)
{
    const int key_width = 90;
    const int key_height = 80;
    const int key_indent = 15;
    const int left_indent = 10;
    const int top_indent = 10;

    int i, j;
    int key_num;

    disp_clear(uk->disp_touch);

    uk->key_cancel =
            ui_button_register("key_esc", uk->disp_touch,
                                 left_indent,
                                 top_indent + (key_height + key_indent) * 0,
                                 key_width, key_height,
                                 key_esc_show, NULL, NULL);

    uk->key_minus =
            ui_button_register("key_minus", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 1,
                                 top_indent + (key_height + key_indent) * 0,
                                 key_width, key_height,
                                 key_minus_show, NULL, NULL);

    uk->key_del =
            ui_button_register("key_del", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 2,
                                 top_indent + (key_height + key_indent) * 0,
                                 key_width, key_height,
                                 key_del_show, NULL, NULL);

    key_num = 0;
    for (i = 3; i >= 1; i--)
        for (j = 0; j < 3; j++, key_num++) {

            uk->keys_num[key_num + 1] =
                    ui_button_register("key_num", uk->disp_touch,
                                       left_indent + (key_width + key_indent) * j,
                                       top_indent + (key_height + key_indent) * i,
                                       key_width, key_height,
                                       key_num_show, NULL, (void *)(1 + key_num));
        }

    uk->keys_num[0] =
            ui_button_register("key_0", uk->disp_touch,
                                 left_indent,
                                 top_indent + (key_height + key_indent) * 4,
                                 key_width, key_height,
                                 key_num_show, NULL, (void *)0);

    uk->key_point =
            ui_button_register("key_point", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 1,
                                 top_indent + (key_height + key_indent) * 4,
                                 key_width, key_height,
                                 key_point_show, NULL, NULL);

    uk->key_ok =
            ui_button_register("key_ok", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 2,
                                 top_indent + (key_height + key_indent) * 4,
                                 key_width, key_height,
                                 key_ok_show, NULL, NULL);

}

static void draw_input_win(struct ui_keyboard *uk, char *text)
{
    struct disp *disp = uk->disp_info;
    static struct text_style ts = {
            .bg_color = DARK_GRAY,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_fill(disp, 20, 200, 440, 100, DARK_GRAY);
    disp_rect(disp, 20, 200, 440, 100, 3, BLUE);
    disp_text(disp, text, 40, 220, &ts);
}

static void draw_input_win_value(struct ui_keyboard *uk, char *val,
                                 struct color color)
{
    struct disp *disp = uk->disp_info;
    static struct text_style ts = {
            .bg_color = DARK_GRAY,
            .font = font_rus,
            .fontsize = 4
    };
    ts.color = color;

    disp_fill(disp, 40, 255,
              disp_text_width(&ts, uk->max_len),
              disp_text_height(&ts, uk->max_len),
              DARK_GRAY);
    disp_text(disp, val, 40, 255, &ts);
}

static void ui_keyboard_destructor(void *mem)
{
    struct ui_keyboard *uk = (struct ui_keyboard *)mem;
    int i;
    kmem_deref(&uk->disp_info);
    kmem_deref(&uk->disp_touch);

    for (i = 0; i < ARRAY_SIZE(uk->keys_num); i++)
        kmem_deref(uk->keys_num + i);
    kmem_deref(&uk->key_minus);
    kmem_deref(&uk->key_cancel);
    kmem_deref(&uk->key_ok);
    kmem_deref(&uk->key_point);
    kmem_deref(&uk->key_del);
}

int ui_keyboard_run(char *field_name, int *val,
                    int min, int max, int step)
{
    struct ui_keyboard *uk = ui_keyboard;
    struct machine *m = &machine;
    char input[10 + 1];
    int input_index = 0;
    memset(input, 0, sizeof input);

    uk = kzref_alloc("ui_keyboard", sizeof *uk, ui_keyboard_destructor);
    uk->disp_touch = kmem_ref(m->disp1);
    uk->disp_info = kmem_ref(m->disp2);
    uk->min = min;
    uk->max = max;
    uk->step = step;
    uk->max_len = 10;

    if (*val < min)
        *val = min;
    if (*val > max)
        *val = max;
    if (*val % step)
        *val = (*val / step) * step;

    {
        char *buf;
        draw_input_win(uk, field_name);
        buf = kref_sprintf("%.3f", (float)*val / 1000);
        draw_input_win_value(uk, buf, DARK_GREEN);
        kmem_deref(&buf);
    }

    show(uk);

    while (1) {
        int i;

        yield();
        for (i = 0; i < ARRAY_SIZE(uk->keys_num); i++) {
            char str[3];
            struct ui_button *db = uk->keys_num[i];
            int key_num = (int)db->priv;
            if (!is_ui_button_touched(db))
                continue;

            if (input_index >= uk->max_len)
                continue;

            sprintf(str, "%d", key_num);

            input[input_index] = str[0];
            input_index++;

            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_ui_button_touched(uk->key_point)) {
            if (strchr(input, '.'))
                continue;

            if (input_index >= uk->max_len)
                continue;

            input[input_index] = '.';
            input_index++;
            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_ui_button_touched(uk->key_minus)) {
            int i;
            if (input[0] == '-') {
                for (i = 0; i < input_index - 1; i++)
                    input[i] = input[i + 1];
                input_index --;
                input[input_index] = 0;
            } else {
                for (i = input_index - 1; i >= 0; i--)
                    input[i + 1] = input[i];
                input[0] = '-';
                input_index ++;
            }
            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_ui_button_touched(uk->key_del) && input_index) {
            input_index--;
            input[input_index] = 0;
            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_ui_button_touched(uk->key_ok) && input_index) {
            float f_val;
            sscanf(input, "%f", &f_val);
            *val = (int)(f_val * 1000);
            if (*val < min)
                *val = min;
            if (*val > max)
                *val = max;
            if (*val % step)
                *val = (*val / step) * step;
            kmem_deref(&uk);
            return 0;
        }

        if (is_ui_button_touched(uk->key_cancel)) {
            kmem_deref(&uk);
            return -1;
        }
    }
    return -1;
}

