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
#include "disp_button.h"
#include "machine.h"

struct ui_keyboard {
    struct disp *disp_info;
    struct disp *disp_touch;
    struct disp_button *keys_num[10];
    struct disp_button *key_point;
    struct disp_button *key_del;
    struct disp_button *key_ok;
    struct disp_button *key_cancel;
    int max_len;
};

static struct ui_keyboard *ui_keyboard = NULL;

static struct text_style key_text_style = {
        .bg_color = BLACK,
        .color = BLUE,
        .font = font_rus,
        .fontsize = 3
};

static void key_num_draw(struct disp_button *db)
{
    int key_num = (int)db->priv;
    char str[3];

    sprintf(str, "%d", key_num);
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);
    disp_text(db->disp, str,
            db->x + db->width / 2 - 10,
            db->y + db->height / 2 - 10,
            &key_text_style);
}

static void key_point_draw(struct disp_button *db)
{
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);
    disp_text(db->disp, ".",
            db->x + db->width / 2 - 10,
            db->y + db->height / 2 - 10,
            &key_text_style);
}

static void key_del_draw(struct disp_button *db)
{
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);
    disp_text(db->disp, "<-",
            db->x + db->width / 2 - 15,
            db->y + db->height / 2 - 10,
            &key_text_style);
}

static void key_ok_draw(struct disp_button *db)
{
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GREEN);
    disp_text(db->disp, "OK",
            db->x + db->width / 2 - 15,
            db->y + db->height / 2 - 10,
            &ts);
}

static void key_esc_draw(struct disp_button *db)
{
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = RED,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, RED);
    disp_text(db->disp, "Esc",
            db->x + db->width / 2 - 25,
            db->y + db->height / 2 - 10,
            &ts);
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

    uk->key_del =
            disp_button_register("key_del", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 1,
                                 top_indent + (key_height + key_indent) * 0,
                                 key_width * 2 + key_indent, key_height, 0,
                                 key_del_draw, NULL);

    uk->key_cancel =
            disp_button_register("key_esc", uk->disp_touch,
                                 left_indent,
                                 top_indent + (key_height + key_indent) * 0,
                                 key_width, key_height, 0,
                                 key_esc_draw, NULL);

    key_num = 0;
    for (i = 3; i >= 1; i--)
        for (j = 0; j < 3; j++, key_num++) {

            uk->keys_num[key_num + 1] =
                    disp_button_register("key_num", uk->disp_touch,
                                         left_indent + (key_width + key_indent) * j,
                                         top_indent + (key_height + key_indent) * i,
                                         key_width, key_height, (void *)(1 + key_num),
                                         key_num_draw, NULL);
        }

    uk->keys_num[0] =
            disp_button_register("key_0", uk->disp_touch,
                                 left_indent,
                                 top_indent + (key_height + key_indent) * 4,
                                 key_width, key_height, (void *)0,
                                 key_num_draw, NULL);

    uk->key_point =
            disp_button_register("key_point", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 1,
                                 top_indent + (key_height + key_indent) * 4,
                                 key_width, key_height, (void *)0,
                                 key_point_draw, NULL);

    uk->key_ok =
            disp_button_register("key_ok", uk->disp_touch,
                                 left_indent + (key_width + key_indent) * 2,
                                 top_indent + (key_height + key_indent) * 4,
                                 key_width, key_height, 0,
                                 key_ok_draw, NULL);

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
    kmem_deref(&uk->key_cancel);
    kmem_deref(&uk->key_ok);
    kmem_deref(&uk->key_point);
    kmem_deref(&uk->key_del);
}

int ui_keyboard_run(char *field_name, float *val)
{
    struct ui_keyboard *uk = ui_keyboard;
    struct machine *m = &machine;
    char input[10 + 1];
    int input_index = 0;
    memset(input, 0, sizeof input);

    uk = kzref_alloc("ui_keyboard", sizeof *uk, ui_keyboard_destructor);
    uk->disp_touch = kmem_ref(m->disp1);
    uk->disp_info = kmem_ref(m->disp2);
    uk->max_len = 10;

    {
        char *buf;
        draw_input_win(uk, field_name);
        buf = kref_sprintf("%.3f", *val);
        draw_input_win_value(uk, buf, DARK_GREEN);
        kmem_deref(&buf);
    }

    show(uk);

    while (1) {
        int i;

        yield();
        for (i = 0; i < ARRAY_SIZE(uk->keys_num); i++) {
            char str[3];
            struct disp_button *db = uk->keys_num[i];
            int key_num = (int)db->priv;
            if (!is_disp_button_touched(db))
                continue;

            if (input_index >= uk->max_len)
                continue;

            sprintf(str, "%d", key_num);

            input[input_index] = str[0];
            input_index++;

            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_disp_button_touched(uk->key_point)) {
            if (strchr(input, '.'))
                continue;

            if (input_index >= uk->max_len)
                continue;

            input[input_index] = '.';
            input_index++;
            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_disp_button_touched(uk->key_del) && input_index) {
            input_index--;
            input[input_index] = 0;
            draw_input_win_value(uk, input, YELLOW);
        }

        if (is_disp_button_touched(uk->key_ok) && input_index) {
            sscanf(input, "%f", val);
            kmem_deref(&uk);
            return 0;
        }

        if (is_disp_button_touched(uk->key_cancel)) {
            kmem_deref(&uk);
            return -1;
        }
    }
    return -1;
}

