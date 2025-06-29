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
#include "ui_keyboard.h"
#include "abs_position.h"

#define BLINK_INTERVAL 300

struct moveto_ui_items {
    struct ui_button *set_xy;
    struct ui_button *set_step;
    struct ui_button *set_cross_pos;
    struct ui_button *set_longitudal_pos;
    struct ui_button *move_to_cross;
    struct ui_button *move_to_longitudal;
    struct ui_item *up_arrow;
    struct ui_item *down_arrow;
    struct ui_item *left_arrow;
    struct ui_item *right_arrow;
    struct ui_item *up_down_arrow;
    struct ui_item *left_right_arrow;
};

struct ui_move_to {
    struct moveto_ui_items *ui_items;
    struct disp *disp_info;
    struct disp *disp_touch;
    int move_to_cross_pos;
    int move_to_longitudal_pos;
    int move_step;
};

struct ui_move_to *ui_move_to = NULL;

static void show(void);
static void hide(void)
{
    struct ui_move_to *umt = ui_move_to;
    kmem_deref(&umt->ui_items);
}

int ui_move_to_step(void)
{
    struct ui_move_to *umt = ui_move_to;
    return umt->move_step;
}

static void moveto_ui_items_destructor(void *mem)
{
    struct moveto_ui_items *ui_items = (struct moveto_ui_items *)mem;
    kmem_deref(&ui_items->set_xy);
    kmem_deref(&ui_items->set_step);
    kmem_deref(&ui_items->set_cross_pos);
    kmem_deref(&ui_items->set_longitudal_pos);
    kmem_deref(&ui_items->move_to_cross);
    kmem_deref(&ui_items->move_to_longitudal);
    kmem_deref(&ui_items->up_arrow);
    kmem_deref(&ui_items->down_arrow);
    kmem_deref(&ui_items->left_arrow);
    kmem_deref(&ui_items->right_arrow);
    kmem_deref(&ui_items->up_down_arrow);
    kmem_deref(&ui_items->left_right_arrow);
}

static void key_set_xy_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);

    img = img_set_xy();
    disp_fill_img(ut->disp, ut->x + 20, ut->y + 4, img);
    kmem_deref(&img);
}

static void onclick_set_xy(void *priv)
{
    hide();
    ui_set_xy_run();
    show();
}

static void key_set_step_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_move_to *umt = (struct ui_move_to *)ub->priv;
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

static void key_set_cross_pos_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_move_to *umt = (struct ui_move_to *)ub->priv;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f", (float)umt->move_to_cross_pos / 1000);
    disp_text(ut->disp, string, ut->x + 5, ut->y + 17, &ts);
    kmem_deref(&string);
}

static void key_set_longitudal_pos_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_move_to *umt = (struct ui_move_to *)ub->priv;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3
    };

    string = kref_sprintf("%.3f", (float)umt->move_to_longitudal_pos / 1000);
    disp_text(ut->disp, string, ut->x + 5, ut->y + 17, &ts);
    kmem_deref(&string);
}

static void key_move_to_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 2, BLUE);
    img = img_moveto();
    disp_fill_img(ut->disp, ut->x + 8, ut->y + 10, img);
    kmem_deref(&img);
}



void onclick_set_step(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    int pos = umt->move_step;
    hide();
    if (!ui_keyboard_run("move step: ", &pos,
                         5, 1000 * 1000, 5)) {
        umt->move_step = pos;
    }
    show();
}

void onclick_set_cross_pos(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    int pos = umt->move_to_cross_pos;
    hide();
    if (!ui_keyboard_run("cross position: ", &pos,
                         -1000 * 1000, 1000 * 1000,
                         LINEAR_CROSS_RESOLUTION * 2)) {
        umt->move_to_cross_pos = pos;
    }
    show();
}

void onclick_set_longitudal_pos(void *priv)
{
    struct ui_move_to *umt = (struct ui_move_to *)priv;
    int pos = umt->move_to_longitudal_pos;
    hide();
    if (!ui_keyboard_run("longitudal position: ", &pos,
                         -1500 * 1000, 1500 * 1000,
                         LINEAR_LONGITUDAL_RESOLUTION)) {
        umt->move_to_longitudal_pos = pos;
    }
    show();
}

void ui_move_to_lock_moveto(void)
{
    struct ui_move_to *umt = ui_move_to;
    ui_button_hide(umt->ui_items->move_to_cross);
    ui_button_hide(umt->ui_items->move_to_longitudal);
}

void ui_move_to_unlock_moveto(void)
{
    struct ui_move_to *umt = ui_move_to;
    ui_button_show(umt->ui_items->move_to_cross);
    ui_button_show(umt->ui_items->move_to_longitudal);
}

static void onclick_move_to_cross(void *priv)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = (struct ui_move_to *)priv;

    if (m->is_busy)
        return; // TODO
    ui_move_to_lock_moveto();
    thread_send_msg(m->machine_tid, MACHINE_MSG_MOVETO_CROSS,
                    (void *)umt->move_to_cross_pos);
}

static void onclick_move_to_longitudal(void *priv)
{
    struct machine *m = &machine;
    struct ui_move_to *umt = (struct ui_move_to *)priv;

    if (m->is_busy)
        return; // TODO
    ui_move_to_lock_moveto();
    thread_send_msg(m->machine_tid, MACHINE_MSG_MOVETO_LONGITUDAL,
                    (void *)umt->move_to_longitudal_pos);
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

static void show(void)
{
    struct ui_move_to *umt = ui_move_to;
    struct moveto_ui_items *ui_items;
    ui_items = kzref_alloc("moveto_ui_items", sizeof *ui_items,
                          moveto_ui_items_destructor);
    umt->ui_items = ui_items;

    disp_clear(umt->disp_touch);

    ui_items->set_xy =
            ui_button_register("key2_set_xy", umt->disp_touch,
                                 190, 10, 100, 55,
                                 key_set_xy_show,
                                 onclick_set_xy, umt);

    ui_items->set_step =
            ui_button_register("key_set_step", umt->disp_touch,
                                 87, 125, 145, 55,
                                 key_set_step_show,
                                 onclick_set_step, umt);

    ui_items->set_cross_pos =
            ui_button_register("key_set_cross_pos", umt->disp_touch,
                                 160, 280, 160, 55,
                                 key_set_cross_pos_show,
                                 onclick_set_cross_pos, umt);

    ui_items->set_longitudal_pos =
            ui_button_register("key_set_longitudal_pos", umt->disp_touch,
                                 160, 390, 160, 55,
                                 key_set_longitudal_pos_show,
                                 onclick_set_longitudal_pos, umt);

    ui_items->move_to_cross =
            ui_button_register("key_move_to_cross", umt->disp_touch,
                                 5, 260, 90, 90,
                                 key_move_to_show,
                                 onclick_move_to_cross, umt);

    ui_items->move_to_longitudal =
            ui_button_register("key_move_to_longitudal", umt->disp_touch,
                                 5, 370, 90, 90,
                                 key_move_to_show,
                                 onclick_move_to_longitudal, umt);


    ui_items->up_arrow =
            ui_item_register("ui_item_arrow_up", umt->disp_touch,
                             150, 90, 16, 28,
                             up_arrow_show, NULL, NULL, 0);

    ui_items->down_arrow =
            ui_item_register("ui_item_arrow_down", umt->disp_touch,
                             150, 190, 16, 28,
                             down_arrow_show, NULL, NULL, 0);

    ui_items->left_arrow =
            ui_item_register("ui_item_arrow_left", umt->disp_touch,
                             50, 144, 31, 18,
                             left_arrow_show, NULL, NULL, 0);


    ui_items->right_arrow =
            ui_item_register("ui_item_arrow_right", umt->disp_touch,
                             235, 144, 31, 18,
                             right_arrow_show, NULL, NULL, 0);

    ui_items->up_down_arrow =
            ui_item_register("ui_item_arrow_up_down", umt->disp_touch,
                             120, 280, 18, 41,
                             up_down_arrow_show, NULL, NULL, 0);

    ui_items->left_right_arrow =
            ui_item_register("ui_item_arrow_left_right", umt->disp_touch,
                             105, 410, 43, 16,
                             left_right_arrow_show, NULL, NULL, 0);
}

void ui_moveto_blink_left_arrow(void)
{
    struct ui_move_to *umt = ui_move_to;
    ui_item_blink(umt->ui_items->left_arrow, BLINK_INTERVAL);
}

void ui_moveto_blink_right_arrow(void)
{
    struct ui_move_to *umt = ui_move_to;
    ui_item_blink(umt->ui_items->right_arrow, BLINK_INTERVAL);
}

void ui_moveto_blink_up_arrow(void)
{
    struct ui_move_to *umt = ui_move_to;
    if(!umt->ui_items)
        return;
    ui_item_blink(umt->ui_items->up_arrow, BLINK_INTERVAL);
}

void ui_moveto_blink_down_arrow(void)
{
    struct ui_move_to *umt = ui_move_to;
    if(!umt->ui_items)
        return;
    ui_item_blink(umt->ui_items->down_arrow, BLINK_INTERVAL);
}

void ui_moveto_blink_up_down_arrow(void)
{
    struct ui_move_to *umt = ui_move_to;
    if(!umt->ui_items)
        return;
    ui_item_blink(umt->ui_items->up_down_arrow, BLINK_INTERVAL);
}

void ui_moveto_blink_left_right_arrow(void)
{
    struct ui_move_to *umt = ui_move_to;
    if(!umt->ui_items)
        return;
    ui_item_blink(umt->ui_items->left_right_arrow, BLINK_INTERVAL);
}

void ui_moveto_blink_stop(void)
{
    struct ui_move_to *umt = ui_move_to;
    ui_item_blink_stop(umt->ui_items->up_arrow);
    ui_item_blink_stop(umt->ui_items->down_arrow);
    ui_item_blink_stop(umt->ui_items->left_arrow);
    ui_item_blink_stop(umt->ui_items->right_arrow);
    ui_item_blink_stop(umt->ui_items->up_down_arrow);
    ui_item_blink_stop(umt->ui_items->left_right_arrow);
}


static void ui_move_to_destructor(void *mem)
{
    struct ui_move_to *umt = (struct ui_move_to *)mem;
    kmem_deref(&umt->ui_items);
}


void ui_move_to_run(void)
{
    struct machine *m = &machine;
    struct ui_move_to *umt;

    umt = kzref_alloc("ui_move_to", sizeof *umt, ui_move_to_destructor);
    ui_move_to = umt;
    umt->disp_touch = m->disp1;
    umt->disp_info = m->disp2;
    umt->move_step = 10;

    show();

    while (1) {
        yield();
        ui_button_handler();

        if (!is_switch_on(m->switch_move_to)) {
            kmem_deref(&umt);
            return;
        }
    }
}


