/*
 * disp_keyboard.c
 *
 *  Created on: Jun 7, 2025
 *      Author: stelhs
 */

#include "ui_keyboard.h"

#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/kref_alloc.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "disp_button.h"
#include "machine.h"

struct disp_keyboard {
    struct disp *disp_info;
    struct disp *disp_touch;
    struct disp_button *keys_num[10];
    struct disp_button *key_point;
    struct disp_button *key_del;
    struct disp_button *key_ok;
    struct disp_button *key_cancel;
    int max_len;
};

static struct disp_keyboard *disp_keyboard = NULL;

static struct text_style key_text_style = {
        .bg_color = BLACK,
        .color = BLUE,
        .font = font_rus,
        .fontsize = 3
};

struct key_num {
    char text[2];
    int val;
};

static void key_num_draw(struct disp_button *db)
{
    struct key_num *kn = db->priv;

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);
    disp_text(db->disp, kn->text,
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

static void show(struct disp_keyboard *dk)
{
    struct key_num *kn;
    struct disp_button *db;
    const int key_width = 90;
    const int key_height = 80;
    const int key_indent = 15;
    const int left_indent = 10;
    const int top_indent = 10;

    int i, j;
    int key_num;

    disp_clear(dk->disp_touch);

    db = disp_button_create("key_del", dk->disp_touch,
                            key_width * 2 + key_indent, key_height, 0);
    disp_button_show(db, left_indent + (key_width + key_indent) * 1,
                       top_indent + (key_height + key_indent) * 0,
                       key_del_draw);
    dk->key_del = db;


    db = disp_button_create("key_esc", dk->disp_touch,
                            key_width, key_height, 0);
    disp_button_show(db, left_indent,
                       top_indent + (key_height + key_indent) * 0,
                       key_esc_draw);
    dk->key_cancel = db;

    key_num = 0;
    for (i = 3; i >= 1; i--)
        for (j = 0; j < 3; j++, key_num++) {
            db = disp_button_create("key_num",
                                    dk->disp_touch,
                                    key_width, key_height,
                                    sizeof *kn);

            kn = (struct key_num *)db->priv;
            kn->text[0] = '1' + key_num;
            kn->val = 1 + key_num;
            disp_button_show(db,
                               left_indent + (key_width + key_indent) * j,
                               top_indent + (key_height + key_indent) * i,
                               key_num_draw);
            dk->keys_num[key_num + 1] = db;
        }

    db = disp_button_create("key_0", dk->disp_touch,
                            key_width, key_height,
                            sizeof *kn);
    kn = (struct key_num *)db->priv;
    kn->text[0] = '0';
    kn->val = 0;
    disp_button_show(db, left_indent,
                       top_indent + (key_height + key_indent) * 4,
                       key_num_draw);
    dk->keys_num[0] = db;

    db = disp_button_create("key_point", dk->disp_touch,
                            key_width, key_height, 0);
    disp_button_show(db, left_indent + (key_width + key_indent) * 1,
                       top_indent + (key_height + key_indent) * 4,
                       key_point_draw);
    dk->key_point = db;

    db = disp_button_create("key_ok", dk->disp_touch,
                            key_width, key_height, 0);
    disp_button_show(db, left_indent + (key_width + key_indent) * 2,
                       top_indent + (key_height + key_indent) * 4,
                       key_ok_draw);
    dk->key_ok = db;
}

static void draw_input_win(struct disp_keyboard *dk, char *text)
{
    struct disp *disp = dk->disp_info;
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

static void draw_input_win_value(struct disp_keyboard *dk, char *val,
                                 struct color color)
{
    struct disp *disp = dk->disp_info;
    static struct text_style ts = {
            .bg_color = DARK_GRAY,
            .font = font_rus,
            .fontsize = 4
    };
    ts.color = color;

    disp_fill(disp, 40, 255,
              disp_text_width(&ts, dk->max_len),
              disp_text_height(&ts, dk->max_len),
              DARK_GRAY);
    disp_text(disp, val, 40, 255, &ts);
}

static void disp_keyboard_destructor(void *mem)
{
    struct disp_keyboard *dk = (struct disp_keyboard *)mem;
    int i;
    kmem_deref(&dk->disp_info);
    kmem_deref(&dk->disp_touch);

    for (i = 0; i < ARRAY_SIZE(dk->keys_num); i++)
        kmem_deref(dk->keys_num + i);
    kmem_deref(&dk->key_cancel);
    kmem_deref(&dk->key_ok);
    kmem_deref(&dk->key_point);
    kmem_deref(&dk->key_del);
}

int ui_keyboard_run(char *field_name, float *val)
{
    struct disp_keyboard *dk = disp_keyboard;
    struct machine *m = &machine;
    char input[10 + 1];
    int input_index = 0;
    memset(input, 0, sizeof input);

    dk = kzref_alloc("disp_keyboard", sizeof *dk, disp_keyboard_destructor);
    dk->disp_touch = kmem_ref(m->disp1);
    dk->disp_info = kmem_ref(m->disp2);
    dk->max_len = 10;

    {
        char *buf;
        draw_input_win(dk, field_name);
        buf = kref_sprintf("%.2f", *val);
        draw_input_win_value(dk, buf, DARK_GREEN);
        kmem_deref(&buf);
    }

    show(dk);

    while (1) {
        int i;

        yield();
        for (i = 0; i < ARRAY_SIZE(dk->keys_num); i++) {
            struct disp_button *db = dk->keys_num[i];
            struct key_num *kn = (struct key_num *)db->priv;
            if (!is_disp_button_touched(db))
                continue;

            if (input_index >= dk->max_len)
                continue;

            input[input_index] = kn->text[0];
            input_index++;

            draw_input_win_value(dk, input, YELLOW);
        }

        if (is_disp_button_touched(dk->key_point)) {
            if (strchr(input, '.'))
                continue;

            if (input_index >= dk->max_len)
                continue;

            input[input_index] = '.';
            input_index++;
            draw_input_win_value(dk, input, YELLOW);
        }

        if (is_disp_button_touched(dk->key_del) && input_index) {
            input_index--;
            input[input_index] = 0;
            draw_input_win_value(dk, input, YELLOW);
        }

        if (is_disp_button_touched(dk->key_ok) && input_index) {
            sscanf(input, "%f", val);
            kmem_deref(&dk);
            return 0;
        }

        if (is_disp_button_touched(dk->key_cancel)) {
            kmem_deref(&dk);
            return -1;
        }
    }
    return -1;
}

