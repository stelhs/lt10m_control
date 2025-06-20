/*
 * ui_set_xy.c
 *
 *  Created on: Jun 20, 2025
 *      Author: stelhs
 */

#include "machine.h"
#include "ui_set_xy.h"
#include "stm32_lib/kref_alloc.h"
#include "disp_mipi_dcs.h"
#include "disp_button.h"
#include "images.h"
#include "abs_position.h"
#include "ui_keyboard.h"


struct ui_set_xy_buttons {
    struct disp_button *key_esc;
    struct disp_button *key_ok;
    struct disp_button *key_tools[4];
    struct disp_button *key_up_down;
    struct disp_button *key_left_right;
    struct disp_button *key_cross_pos;
    struct disp_button *key_longitudal_pos;
};

struct ui_set_xy {
    struct ui_set_xy_buttons *buttons;
    struct disp *disp_info;
    struct disp *disp_touch;
    int tool_num;
    int longitudal_pos[4];
    int cross_pos[4];
    bool is_longitudal_inc_right;
    bool is_cross_inc_up;
};

static struct ui_set_xy *ui_set_xy = NULL;

static void show(void);
static void hide(void)
{
    struct ui_set_xy *usx = ui_set_xy;
    kmem_deref(&usx->buttons);
}


void onclick_up_down(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    hide();
    usx->is_cross_inc_up = !usx->is_cross_inc_up;
    show();
}

void onclick_left_right(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    hide();
    usx->is_longitudal_inc_right = !usx->is_longitudal_inc_right;
    show();
}

void onclick_cross_pos(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    float pos = (float)usx->cross_pos[usx->tool_num] / 1000;
    hide();
    if (!ui_keyboard_run("diameter: ", &pos)) {
        usx->cross_pos[usx->tool_num] = (int)(pos * 1000);
    }
    show();
}

void onclick_longitudal_pos(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    float pos = (float)usx->longitudal_pos[usx->tool_num] / 1000;
    hide();
    if (!ui_keyboard_run("diameter: ", &pos)) {
        usx->longitudal_pos[usx->tool_num] = (int)(pos * 1000);
    }
    show();
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

static void key_tool_num_draw(struct disp_button *db)
{
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = EMERALD,
            .font = font_rus,
            .fontsize = 3
    };
    int tool_num = (int)db->priv;
    char str[3];

    sprintf(str, "%d", tool_num);
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, EMERALD);
    disp_text(db->disp, str,
            db->x + db->width / 2 - 8,
            db->y + db->height / 2 - 10,
            &ts);
}

static void key_up_down_draw(struct disp_button *db)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)db->priv;
    struct img *img;
    if (usx->is_cross_inc_up)
        img = img_cross_arrow_up1();
    else
        img = img_cross_arrow_down1();
    disp_fill_img(db->disp, db->x + 73, db->y + 11, img);
    kmem_deref(&img);
}

static void key_left_right_draw(struct disp_button *db)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)db->priv;
    struct img *img;
    if (usx->is_longitudal_inc_right)
        img = img_longitudal_arrow_right1();
    else
        img = img_longitudal_arrow_left1();
    disp_fill_img(db->disp, db->x + 65, db->y + 17, img);
    kmem_deref(&img);
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

static void key_cross_pos_draw(struct disp_button *db)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)db->priv;
    char *string;
    int x, width;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f", (float)usx->cross_pos[usx->tool_num] / 1000);
    width = disp_text_width(&ts, strlen(string));
    x = db->width / 2 - width / 2;
    disp_text(db->disp, string, db->x + x, db->y + 15, &ts);
    kmem_deref(&string);
}

static void key_longitudal_pos_draw(struct disp_button *db)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)db->priv;
    char *string;
    int x, width;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f", (float)usx->longitudal_pos[usx->tool_num] / 1000);
    width = disp_text_width(&ts, strlen(string));
    x = db->width / 2 - width / 2;
    disp_text(db->disp, string, db->x + x, db->y + 15, &ts);
    kmem_deref(&string);
}

static void ui_set_xy_buttons_destructor(void *mem)
{
    struct ui_set_xy_buttons *buttons = (struct ui_set_xy_buttons *)mem;
    int i;
    kmem_deref(&buttons->key_esc);
    kmem_deref(&buttons->key_ok);
    for (i = 0; i < ARRAY_SIZE(buttons->key_tools); i++)
        kmem_deref(buttons->key_tools + i);
    kmem_deref(&buttons->key_up_down);
    kmem_deref(&buttons->key_left_right);
    kmem_deref(&buttons->key_cross_pos);
    kmem_deref(&buttons->key_longitudal_pos);
}

static void show(void)
{
    struct ui_set_xy *usx = ui_set_xy;
    int i;

    struct ui_set_xy_buttons *buttons;
    buttons = kzref_alloc("ui_set_xy_buttons", sizeof *buttons,
                          ui_set_xy_buttons_destructor);
    usx->buttons = buttons;

    disp_clear(usx->disp_touch);

    buttons->key_esc =
            disp_button_register("key_esc", usx->disp_touch,
                                 16, 16,
                                 90, 90, 0,
                                 key_esc_draw, NULL);

    for (i = 0; i < 4; i++) {
        buttons->key_tools[i] =
                disp_button_register("key_tool_num", usx->disp_touch,
                                     12 + (65 + 11) * i, 138,
                                     65, 65, (void *)(i + 1),
                                     key_tool_num_draw, NULL);
    }

    buttons->key_up_down =
            disp_button_register("key_up_down", usx->disp_touch,
                                 0, 234,
                                 160, 50, usx,
                                 key_up_down_draw, onclick_up_down);

    buttons->key_left_right =
            disp_button_register("key_left_right", usx->disp_touch,
                                 160, 234,
                                 160, 50, usx,
                                 key_left_right_draw, onclick_left_right);

    buttons->key_cross_pos =
            disp_button_register("key_cross_pos", usx->disp_touch,
                                 0, 285,
                                 160, 50, usx,
                                 key_cross_pos_draw, onclick_cross_pos);

    buttons->key_longitudal_pos =
            disp_button_register("key_longitudal_pos", usx->disp_touch,
                                 160, 285,
                                 160, 50, usx,
                                 key_longitudal_pos_draw, onclick_longitudal_pos);

    buttons->key_ok =
            disp_button_register("key_longitudal_pos", usx->disp_touch,
                                 212, 374,
                                 90, 90, 0,
                                 key_ok_draw, NULL);

    disp_fill(usx->disp_touch, 159, 234, 2, 100, GREEN); // vertical line
    disp_fill(usx->disp_touch, 0, 234 + 50 - 1, 320, 2, GREEN); // horizontal line
}

static void ui_set_xy_destructor(void *mem)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)mem;
    kmem_deref(&usx->disp_info);
    kmem_deref(&usx->disp_touch);
    kmem_deref(&usx->buttons);
}

int ui_set_xy_run(void)
{
    struct machine *m = &machine;
    struct ui_set_xy *usx;
    int i;

    usx = kzref_alloc("ui_set_xy", sizeof *usx, ui_set_xy_destructor);
    usx->disp_touch = kmem_ref(m->disp1);
    usx->disp_info = kmem_ref(m->disp2);
    usx->tool_num = m->curr_tool_num;
    for (i = 0; i < 4; i++) {
        usx->longitudal_pos[i] = abs_longitudal(m->ap, i);
        usx->cross_pos[i] = abs_cross(m->ap, i);
    }
    usx->is_cross_inc_up = m->ap->is_cross_inc_up;
    usx->is_longitudal_inc_right = m->ap->is_longitudal_inc_right;

    usx->tool_num = 0;
    ui_set_xy = usx;

    show();

    while (1) {
        yield();

        if (is_disp_button_touched(usx->buttons->key_ok)) {
            for (i = 0; i < 4; i++) {
                abs_cross_set(m->ap, i, usx->cross_pos[i]);
                abs_longitudal_set(m->ap, i, usx->longitudal_pos[i]);
            }
            m->ap->is_cross_inc_up = usx->is_cross_inc_up;
            m->ap->is_longitudal_inc_right = usx->is_longitudal_inc_right;
            m->curr_tool_num = usx->tool_num;
            kmem_deref(&usx);
            return 0;
        }

        if (is_disp_button_touched(usx->buttons->key_esc)) {
            kmem_deref(&usx);
            return -1;
        }

        disp_button_handler();
    }

    return -1;
}
