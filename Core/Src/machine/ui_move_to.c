/*
 * ui_move_to.c
 *
 *  Created on: Jun 21, 2025
 *      Author: stelhs
 */
#include "machine.h"
#include "ui_move_to.h"
#include "ui_set_xy.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/buttons.h"
#include "disp_mipi_dcs.h"
#include "images.h"
#include "ui_button.h"
#include "ui_input.h"
#include "ui_keyboard.h"
#include "abs_position.h"

static void ui_scope_init(void);

int ui_move_to_step(void)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = &m->ui_move_to;
    return umt->move_step;
}

static void key_set_xy_show(struct ui_item *ut)
{
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);

    img = img_set_xy();
    disp_fill_img(ut->disp, ut->x + 20, ut->y + 4, img);
    kmem_deref(&img);
}

static int onclick_set_xy(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    kmem_deref(&umt->ui_scope);
    ui_set_xy_run();
    ui_scope_init();
    return TRUE;
}

static void key_set_step_show(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    char *string;
    int x, width;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = YELLOW,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f", (float)umt->move_step / 1000);
    width = disp_text_width(&ts, strlen(string));
    x = ut->width / 2 - width / 2;
    disp_text(ut->disp, string, ut->x + x, ut->y + 17, &ts);
    kmem_deref(&string);
}

static void key_set_cross_pos_show(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    char *string;
    string = kref_sprintf("%.3f", (float)umt->move_to_cross_pos / 1000);
    disp_text(ut->disp, string, ut->x + 5, ut->y + 17, &input_ts);
    kmem_deref(&string);
}

static void key_set_longitudal_pos_show(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    char *string;
    string = kref_sprintf("%.3f", (float)umt->move_to_longitudal_pos / 1000);
    disp_text(ut->disp, string, ut->x + 5, ut->y + 17, &input_ts);
    kmem_deref(&string);
}

static void key_move_to_show(struct ui_item *ut)
{
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 2, BLUE);
    img = img_moveto();
    disp_fill_img(ut->disp, ut->x + 8, ut->y + 10, img);
    kmem_deref(&img);
}



static int onclick_set_step(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    int pos = umt->move_step;

    kmem_deref(&umt->ui_scope);
    if (!ui_keyboard_run("move step: ", &pos,
                         4, 1000 * 1000, 4, FALSE)) {
        umt->move_step = pos;
    }
    ui_scope_init();
    return TRUE;
}

static int onclick_set_cross_pos(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    int pos = umt->move_to_cross_pos;
    kmem_deref(&umt->ui_scope);
    if (!ui_keyboard_run("cross position: ", &pos,
                         -1000 * 1000, 1000 * 1000,
                         LINEAR_CROSS_RESOLUTION * 2, FALSE)) {
        umt->move_to_cross_pos = pos;
    }
    ui_scope_init();
    return TRUE;
}

static int onclick_set_longitudal_pos(struct ui_item *ut)
{
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;
    int pos = umt->move_to_longitudal_pos;

    kmem_deref(&umt->ui_scope);
    if (!ui_keyboard_run("longitudal position: ", &pos,
                         -1500 * 1000, 1500 * 1000,
                         LINEAR_LONGITUDAL_RESOLUTION, FALSE)) {
        umt->move_to_longitudal_pos = pos;
    }
    ui_scope_init();
    return TRUE;
}


static void onclick_move_to_cross(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;

    if (m->is_busy)
        return; // TODO

    ui_item_hide(umt->move_to_cross);
    ui_item_hide(umt->move_to_longitudal);

    ui_item_blink(umt->up_down_arrow, 300);
    cross_move_to(umt->move_to_cross_pos / 2, TRUE, NULL, NULL);
    buttons_reset();
    ui_item_blink_stop(umt->up_down_arrow);

    ui_item_show(umt->move_to_cross);
    ui_item_show(umt->move_to_longitudal);
}


static void onclick_move_to_longitudal(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = (struct ui_move_to *)ut->priv;

    if (m->is_busy)
        return; // TODO

    ui_item_hide(umt->move_to_cross);
    ui_item_hide(umt->move_to_longitudal);

    ui_item_blink(umt->left_right_arrow, 300);
    longitudal_move_to(umt->move_to_longitudal_pos, TRUE, 0, NULL, NULL);
    buttons_reset();
    ui_item_blink_stop(umt->left_right_arrow);

    ui_item_show(umt->move_to_cross);
    ui_item_show(umt->move_to_longitudal);
}

static void up_arrow_show(struct ui_item *ut)
{
    struct img *img = img_cross_arrow_up1();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void down_arrow_show(struct ui_item *ut)
{
    struct img *img = img_cross_arrow_down1();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void left_arrow_show(struct ui_item *ut)
{
    struct img *img = img_longitudal_arrow_left1();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void right_arrow_show(struct ui_item *ut)
{
    struct img *img = img_longitudal_arrow_right1();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void up_down_arrow_show(struct ui_item *ut)
{
    struct img *img = img_cross_arrow_up_down();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void left_right_arrow_show(struct ui_item *ut)
{
    struct img *img = img_longitudal_arrow_left_right();
    disp_fill_img(ut->disp, ut->x, ut->y, img);
    kmem_deref(&img);
}

static void ui_scope_init(void)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = &m->ui_move_to;

    umt->ui_scope = ui_scope_create("ui_scope_moveto");

    disp_clear(umt->disp_touch);

    ui_button_register("key2_set_xy", umt->ui_scope,
                         190, 10, 100, 55,
                         key_set_xy_show,
                         onclick_set_xy, umt, 0);

    ui_button_register("key_set_step", umt->ui_scope,
                         87, 125, 145, 55,
                         key_set_step_show,
                         onclick_set_step, umt, 0);

    ui_button_register("key_set_cross_pos", umt->ui_scope,
                         160, 280, 160, 55,
                         key_set_cross_pos_show,
                         onclick_set_cross_pos, umt, 0);

    ui_button_register("key_set_longitudal_pos", umt->ui_scope,
                         160, 390, 160, 55,
                         key_set_longitudal_pos_show,
                         onclick_set_longitudal_pos, umt, 0);

    umt->move_to_cross =
            ui_button_confirmation_register("key_move_to_cross",
                                            umt->ui_scope,
                                            5, 260, 90, 90,
                                            key_move_to_show,
                                            onclick_move_to_cross, umt);

    umt->move_to_longitudal =
            ui_button_confirmation_register("key_move_to_longitudal",
                                            umt->ui_scope,
                                            5, 370, 90, 90,
                                            key_move_to_show,
                                            onclick_move_to_longitudal, umt);

    umt->up_arrow =
            ui_item_register("ui_item_arrow_up", umt->ui_scope,
                             umt->disp_touch, 150, 90, 16, 28,
                             up_arrow_show, NULL, NULL, 0);

    umt->down_arrow =
            ui_item_register("ui_item_arrow_down", umt->ui_scope,
                             umt->disp_touch, 150, 190, 16, 28,
                             down_arrow_show, NULL, NULL, 0);

    umt->left_arrow =
            ui_item_register("ui_item_arrow_left", umt->ui_scope,
                             umt->disp_touch, 50, 144, 31, 18,
                             left_arrow_show, NULL, NULL, 0);


    umt->right_arrow =
            ui_item_register("ui_item_arrow_right", umt->ui_scope,
                             umt->disp_touch, 235, 144, 31, 18,
                             right_arrow_show, NULL, NULL, 0);

    umt->up_down_arrow =
            ui_item_register("ui_item_arrow_up_down", umt->ui_scope,
                             umt->disp_touch, 120, 280, 18, 41,
                             up_down_arrow_show, NULL, NULL, 0);

    umt->left_right_arrow =
            ui_item_register("ui_item_arrow_left_right", umt->ui_scope,
                             umt->disp_touch, 105, 410, 43, 16,
                             left_right_arrow_show, NULL, NULL, 0);
}

void ui_move_to_run(void)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = &m->ui_move_to;

    umt->disp_touch = m->disp1;
    umt->disp_info = m->disp2;

    ui_scope_init();

    while (1) {
        yield();

        ui_button_handler();
        if (!is_switch_on(m->switch_move_to)) {
            kmem_deref(&umt->ui_scope);
            return;
        }

        if (is_switch_on(m->switch_run)) {
            kmem_deref(&umt->ui_scope);
            return;
        }
    }
}


