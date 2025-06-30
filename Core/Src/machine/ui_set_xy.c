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
#include "images.h"
#include "abs_position.h"
#include "ui_button.h"
#include "ui_keyboard.h"


struct set_xy_ui_items {
    struct ui_button *key_esc;
    struct ui_button *key_tools[4];
    struct ui_button *key_up_down;
    struct ui_button *key_left_right;
    struct ui_button *key_cross_pos;
    struct ui_button *key_longitudal_pos;
    struct ui_button *key_reset_cross;
    struct ui_button *key_reset_longitudal;
};

struct ui_set_xy {
    struct set_xy_ui_items *ui_items;
    struct disp *disp_info;
    struct disp *disp_touch;
    int tool_num;
    int longitudal_pos[4];
    int cross_pos[4];
    bool is_longitudal_inc_left;
    bool is_cross_inc_down;
    bool is_finished;
};

static struct ui_set_xy *ui_set_xy = NULL;

static void show(void);
static void hide(void)
{
    struct ui_set_xy *usx = ui_set_xy;
    kmem_deref(&usx->ui_items);
}

void onclick_tool_num(void *priv)
{
    int tool_num = (int)priv;
    struct ui_set_xy *usx = ui_set_xy;
    if (usx->tool_num == tool_num)
        return;
    hide();
    usx->tool_num = tool_num;
    show();
}

void onclick_up_down(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    hide();
    usx->is_cross_inc_down = !usx->is_cross_inc_down;
    show();
}

void onclick_left_right(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    hide();
    usx->is_longitudal_inc_left = !usx->is_longitudal_inc_left;
    show();
}

void onclick_cross_pos(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    int pos = usx->cross_pos[usx->tool_num];
    hide();
    if (!ui_keyboard_run("diameter: ", &pos,
                         -1000 * 1000, 1000 * 1000,
                         LINEAR_CROSS_RESOLUTION * 2)) {
        usx->cross_pos[usx->tool_num] = pos / 2;
    }
    usx->is_finished = TRUE;
}

void onclick_longitudal_pos(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    int pos = usx->longitudal_pos[usx->tool_num];
    hide();
    if (!ui_keyboard_run("Longitudal: ", &pos,
                         -1500 * 1000, 1500 * 1000,
                         LINEAR_LONGITUDAL_RESOLUTION)) {
        usx->longitudal_pos[usx->tool_num] = pos;
    }
    usx->is_finished = TRUE;
}

void onclick_reset_cross(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    hide();
    usx->cross_pos[usx->tool_num] = 0;
    usx->is_finished = TRUE;
}

void onclick_reset_longitudal(void *priv)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)priv;
    hide();
    usx->longitudal_pos[usx->tool_num] = 0;
    usx->is_finished = TRUE;
}


static void key_esc_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = RED,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, RED);
    disp_text(ut->disp, "Esc",
            ut->x + ut->width / 2 - 25,
            ut->y + ut->height / 2 - 9,
            &ts);
}

static void key_tool_num_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_set_xy *usx = ui_set_xy;
    static struct text_style non_selected_ts = {
            .bg_color = BLACK,
            .color = EMERALD,
            .font = font_rus,
            .fontsize = 3
    };
    static struct text_style selected_ts = {
            .bg_color = DARK_BLUE,
            .color = YELLOW,
            .font = font_rus,
            .fontsize = 3
    };
    struct text_style *ts = &non_selected_ts;

    int tool_num = (int)ub->priv;
    char str[3];

    sprintf(str, "%d", tool_num + 1);
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, EMERALD);

    if (usx->tool_num == tool_num) {
        disp_fill(ut->disp, ut->x + 1, ut->y + 1,
                  ut->width - 2, ut->height - 2, DARK_BLUE);
        ts = &selected_ts;
    }
    disp_text(ut->disp, str,
              ut->x + ut->width / 2 - 8,
              ut->y + ut->height / 2 - 10, ts);
}

static void key_up_down_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_set_xy *usx = (struct ui_set_xy *)ub->priv;
    struct img *img;
    if (usx->is_cross_inc_down)
        img = img_cross_arrow_down1();
    else
        img = img_cross_arrow_up1();
    disp_fill_img(ut->disp, ut->x + 73, ut->y + 11, img);
    kmem_deref(&img);
}

static void key_left_right_show(struct ui_button *ub)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)ub->priv;
    struct ui_item *ut = ub->ut;
    struct img *img;
    if (usx->is_longitudal_inc_left)
        img = img_longitudal_arrow_left1();
    else
        img = img_longitudal_arrow_right1();
    disp_fill_img(ut->disp, ut->x + 65, ut->y + 17, img);
    kmem_deref(&img);
}

static void key_cross_pos_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_set_xy *usx = (struct ui_set_xy *)ub->priv;
    char *string;
    int x, width;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f", (float)usx->cross_pos[usx->tool_num] / 1000 * 2);
    width = disp_text_width(&ts, strlen(string));
    x = ut->width / 2 - width / 2;
    disp_text(ut->disp, string, ut->x + x, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void key_longitudal_pos_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_set_xy *usx = (struct ui_set_xy *)ub->priv;
    char *string;
    int x, width;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f",
                          (float)usx->longitudal_pos[usx->tool_num] / 1000);
    width = disp_text_width(&ts, strlen(string));
    x = ut->width / 2 - width / 2;
    disp_text(ut->disp, string, ut->x + x, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void key_reset_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = EMERALD,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, EMERALD);
    disp_text(ut->disp, "0",
              ut->x + ut->width / 2 - 8,
              ut->y + ut->height / 2 - 10, &ts);
}


static void ui_set_xy_ui_items_destructor(void *mem)
{
    struct set_xy_ui_items *ui_items = (struct set_xy_ui_items *)mem;
    int i;
    kmem_deref(&ui_items->key_esc);
    for (i = 0; i < ARRAY_SIZE(ui_items->key_tools); i++)
        kmem_deref(ui_items->key_tools + i);
    kmem_deref(&ui_items->key_up_down);
    kmem_deref(&ui_items->key_left_right);
    kmem_deref(&ui_items->key_cross_pos);
    kmem_deref(&ui_items->key_longitudal_pos);
    kmem_deref(&ui_items->key_reset_cross);
    kmem_deref(&ui_items->key_reset_longitudal);
}

static void show(void)
{
    struct ui_set_xy *usx = ui_set_xy;
    int i;

    struct set_xy_ui_items *ui_items;
    ui_items = kzref_alloc("ui_set_xy_ui_items", sizeof *ui_items,
                          ui_set_xy_ui_items_destructor);
    usx->ui_items = ui_items;

    disp_clear(usx->disp_touch);

    ui_items->key_esc =
            ui_button_register("ui_set_xy_key_esc", usx->disp_touch,
                                 16, 16, 90, 90,
                                 key_esc_show, NULL, NULL);

    for (i = 0; i < 4; i++) {
        ui_items->key_tools[i] =
                ui_button_register("key_tool_num", usx->disp_touch,
                                     12 + (65 + 11) * i, 138, 65, 65,
                                     key_tool_num_show, onclick_tool_num,
                                     (void *)i);
    }

    ui_items->key_up_down =
            ui_button_register("key_up_down", usx->disp_touch,
                                 0, 234, 160, 50,
                                 key_up_down_show, onclick_up_down, usx);

    ui_items->key_left_right =
            ui_button_register("key_left_right", usx->disp_touch,
                                 160, 234, 160, 50,
                                 key_left_right_show,
                                 onclick_left_right, usx);

    ui_items->key_cross_pos =
            ui_button_register("key_cross_pos", usx->disp_touch,
                                 0, 285, 160, 50,
                                 key_cross_pos_show,
                                 onclick_cross_pos, usx);

    ui_items->key_longitudal_pos =
            ui_button_register("key_longitudal_pos", usx->disp_touch,
                                 160, 285, 160, 50,
                                 key_longitudal_pos_show,
                                 onclick_longitudal_pos, usx);

    disp_fill(usx->disp_touch, 159, 234, 2, 100, GREEN); // vertical line
    disp_fill(usx->disp_touch, 0, 234 + 50 - 1, 320, 2, GREEN); // horizontal line

    ui_items->key_reset_cross =
            ui_button_register("key_reset_cross", usx->disp_touch,
                                 43, 366, 65, 65,
                                 key_reset_show,
                                 onclick_reset_cross, usx);

    ui_items->key_reset_longitudal =
            ui_button_register("key_reset_longitudal", usx->disp_touch,
                                 208, 366, 65, 65,
                                 key_reset_show,
                                 onclick_reset_longitudal, usx);

}

static void ui_set_xy_destructor(void *mem)
{
    struct ui_set_xy *usx = (struct ui_set_xy *)mem;
    kmem_deref(&usx->disp_info);
    kmem_deref(&usx->disp_touch);
    if (usx->ui_items)
        kmem_deref(&usx->ui_items);
}

int ui_set_xy_run(void)
{
    struct machine *m = &machine;
    struct ui_set_xy *usx;
    int i;

    usx = kzref_alloc("ui_set_xy", sizeof *usx, ui_set_xy_destructor);
    ui_set_xy = usx;
    usx->disp_touch = kmem_ref(m->disp1);
    usx->disp_info = kmem_ref(m->disp2);
    usx->tool_num = abs_pos_tool(m->ap);
    for (i = 0; i < 4; i++) {
        usx->longitudal_pos[i] = abs_longitudal(m->ap, i);
        usx->cross_pos[i] = abs_cross(m->ap, i);
    }
    usx->is_cross_inc_down = m->ap->is_cross_inc_down;
    usx->is_longitudal_inc_left = m->ap->is_longitudal_inc_left;

    show();

    while (1) {
        ui_button_handler();
        yield();

        if (usx->is_finished) {
            usx->is_finished = FALSE;
            m->ap->is_cross_inc_down = usx->is_cross_inc_down;
            m->ap->is_longitudal_inc_left = usx->is_longitudal_inc_left;
            for (i = 0; i < 4; i++) {
                int cross_pos = abs_cross(m->ap, i);
                int longitudal_pos = abs_longitudal(m->ap, i);
                if (i == usx->tool_num) {
                    cross_pos = usx->cross_pos[i];
                    longitudal_pos = usx->longitudal_pos[i];
                }
                abs_cross_set(m->ap, i, cross_pos);
                abs_longitudal_set(m->ap, i, longitudal_pos);
            }
            abs_pos_set_tool(m->ap, usx->tool_num);
            kmem_deref(&usx);
            mode_cut_settings_validate();
            return 0;
        }

        if (is_ui_button_touched(usx->ui_items->key_esc)) {
            usx->is_finished = TRUE;
        }
    }

    mode_cut_settings_validate();
    return -1;
}
