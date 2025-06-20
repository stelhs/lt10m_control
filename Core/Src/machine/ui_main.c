/*
 * disp_main_win.c
 *
 *  Created on: Jun 8, 2025
 *      Author: stelhs
 */

#include "ui_main.h"

#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "images.h"
#include "disp_button.h"
#include "ui_keyboard.h"
#include "ui_sel_prog.h"
#include "ui_set_xy.h"


struct cmsis_thread *ui_tid;

struct ui_main_buttons {
    struct disp_button *sel_prog;
    struct disp_button *set_xy;
    struct disp_button *sel_feed_limit;
    struct disp_button *sel_cross_or_target_diameter;
    struct disp_button *sel_cross_or_target_diameter_value;
    struct disp_button *sel_step;
};

struct ui_main {
    struct disp *disp;
    struct ui_main_buttons *buttons;
    enum progs prog;
    float longitudial_limit;
    float target_diameter;
    float cross_limit;
    float step;
    bool feed_or_diameter;
};
static struct ui_main *ui_main;

static void show(struct ui_main *um);
static void hide(struct ui_main *um)
{
    kmem_deref(&um->buttons);
}


static void key_prog_sel_draw(struct disp_button *db)
{
    struct ui_main *um = (struct ui_main *)db->priv;
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    img = img_prog_by_num(um->prog);
    disp_fill_img(db->disp, db->x + 2, db->y + 3, img);
    kmem_deref(&img);
}

static void key_set_xy_draw(struct disp_button *db)
{
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    img = img_set_xy();
    disp_fill_img(db->disp, db->x + 20, db->y + 4, img);
    kmem_deref(&img);
}

static void key_feed_limit_draw(struct disp_button *db)
{
    struct ui_main *um = (struct ui_main *)db->priv;
    struct img *img;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, DARK_GRAY);

    switch (um->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_DOWN_LEFT:
        img = img_feed_arrow_left(); break;
    case PROG_FEED_RIGHT:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_RIGHT_DOWN:
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_RIGHT:
        img = img_feed_arrow_right(); break;
    default:
        img = NULL;
    }

    if (img) {
        disp_fill_img(db->disp, db->x + 5, db->y + 10, img);
        kmem_deref(&img);
    }

    string = kref_sprintf("%.2f", um->longitudial_limit);
    disp_text(db->disp, string, db->x + 80, db->y + 15, &ts);
    kmem_deref(&string);
}


static void key_cross_or_diameter_draw(struct disp_button *db)
{
    struct ui_main *um = (struct ui_main *)db->priv;
    struct img *img;

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, DARK_GRAY);

    if (um->feed_or_diameter) {
        switch (um->prog) {
        case PROG_FEED_LEFT_UP:
        case PROG_FEED_RIGHT_UP:
        case PROG_FEED_UP_LEFT:
        case PROG_FEED_UP_RIGHT:
        case PROG_FEED_UP:
            img = img_cross_feed_up(); break;
        case PROG_FEED_LEFT_DOWN:
        case PROG_FEED_RIGHT_DOWN:
        case PROG_FEED_DOWN_LEFT:
        case PROG_FEED_DOWN_RIGHT:
        case PROG_FEED_DOWN:
            img = img_cross_feed_down(); break;
        default:
            panic("key_cross_or_diameter_draw incorect prog");
        }
    } else
        img = img_big_circle();

    disp_fill_img(db->disp, db->x + 5, db->y + 10, img);
    kmem_deref(&img);
}


static void key_cross_or_diameter_value_draw(struct disp_button *db)
{
    struct ui_main *um = (struct ui_main *)db->priv;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, DARK_GRAY);

    string = kref_sprintf("%.2f", um->feed_or_diameter ?
                            um->cross_limit : um->target_diameter);
    disp_text(db->disp, string, db->x + 15, db->y + 15, &ts);
    kmem_deref(&string);
}

static void key_step_draw(struct disp_button *db)
{
    struct ui_main *um = (struct ui_main *)db->priv;
    struct img *img;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, DARK_GRAY);

    img = img_step();
    disp_fill_img(db->disp, db->x + 5, db->y + 10, img);
    kmem_deref(&img);

    string = kref_sprintf("%.2f", um->step);
    disp_text(db->disp, string, db->x + 80, db->y + 15, &ts);
    kmem_deref(&string);
}


static void on_click_prog_sel(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    hide(um);
    um->prog = ui_sel_prog_run();
    show(um);
}

static void on_click_set_xy(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    hide(um);
    ui_set_xy_run();
    show(um);
}

static void on_click_feed_limit(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    hide(um);
    ui_keyboard_run("longitudial feed limit:", &um->longitudial_limit);
    show(um);
}

static void on_click_cross_or_diameter(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    hide(um);
    um->feed_or_diameter = !um->feed_or_diameter;
    show(um);
}

static void on_click_cross_or_diameter_val(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    hide(um);
    if (um->feed_or_diameter)
        ui_keyboard_run("cross feed limit:", &um->cross_limit);
    else
        ui_keyboard_run("target diameter:", &um->target_diameter);
    show(um);
}

static void on_click_step_draw(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    hide(um);
    ui_keyboard_run("feed step:", &um->step);
    show(um);
}

static void main_win_buttons_destructor(void *mem)
{
    struct ui_main_buttons *umb = (struct ui_main_buttons *)mem;
    kmem_deref(&umb->sel_prog);
    kmem_deref(&umb->set_xy);
    if (umb->sel_feed_limit)
        kmem_deref(&umb->sel_feed_limit);
    if (umb->sel_cross_or_target_diameter)
        kmem_deref(&umb->sel_cross_or_target_diameter);
    if (umb->sel_cross_or_target_diameter_value)
        kmem_deref(&umb->sel_cross_or_target_diameter_value);
    if (umb->sel_step)
        kmem_deref(&umb->sel_step);
}

static void show(struct ui_main *um)
{
    struct ui_main_buttons *buttons;
    buttons = kzref_alloc("main_win_buttons", sizeof *buttons,
                      main_win_buttons_destructor);
    um->buttons = buttons;

    disp_clear(um->disp);

    buttons->sel_prog =
            disp_button_register("key_prog_sel", um->disp,
                                 30, 10, 100, 55, um,
                                 key_prog_sel_draw, on_click_prog_sel);

    buttons->set_xy =
            disp_button_register("key_set_xy", um->disp,
                                 190, 10, 100, 55, um,
                                 key_set_xy_draw, on_click_set_xy);

    if (um->prog == PROG_FEED_LEFT ||
            um->prog == PROG_FEED_RIGHT ||
            um->prog == PROG_FEED_LEFT_UP ||
            um->prog == PROG_FEED_RIGHT_UP ||
            um->prog == PROG_FEED_LEFT_DOWN ||
            um->prog == PROG_FEED_RIGHT_DOWN ||
            um->prog == PROG_FEED_UP_LEFT ||
            um->prog == PROG_FEED_UP_RIGHT ||
            um->prog == PROG_FEED_DOWN_LEFT ||
            um->prog ==  PROG_FEED_DOWN_RIGHT) {

        buttons->sel_feed_limit =
                disp_button_register("key_feed_limit", um->disp,
                                     10, 90, 200, 55, um,
                                     key_feed_limit_draw,
                                     on_click_feed_limit);
    }

    if (um->prog == PROG_FEED_LEFT_UP ||
            um->prog == PROG_FEED_RIGHT_UP ||
            um->prog == PROG_FEED_LEFT_DOWN ||
            um->prog == PROG_FEED_RIGHT_DOWN ||
            um->prog == PROG_FEED_UP_LEFT ||
            um->prog == PROG_FEED_UP_RIGHT ||
            um->prog == PROG_FEED_DOWN_LEFT ||
            um->prog ==  PROG_FEED_DOWN_RIGHT) {

        buttons->sel_cross_or_target_diameter =
                disp_button_register("key_cross_or_diameter", um->disp,
                                     10, 90 + (55 + 15) * 1,
                                     70, 55, um,
                                     key_cross_or_diameter_draw,
                                     on_click_cross_or_diameter);

        buttons->sel_cross_or_target_diameter_value =
                disp_button_register("key_cross_or_diameter_val", um->disp,
                                     100, 90 + (55 + 15) * 1,
                                     160, 55, um,
                                     key_cross_or_diameter_value_draw,
                                     on_click_cross_or_diameter_val);

        buttons->sel_step =
                disp_button_register("key_step", um->disp,
                                     10, 90 + (55 + 15) * 2,
                                     200, 55, um,
                                     key_step_draw,
                                     on_click_step_draw);
    }

    if (um->prog == PROG_FEED_UP ||
            um->prog == PROG_FEED_DOWN) {

        buttons->sel_cross_or_target_diameter =
                disp_button_register("key_cross_or_diameter", um->disp,
                                     10, 90, 70, 55, um,
                                     key_cross_or_diameter_draw,
                                     on_click_cross_or_diameter);

        buttons->sel_cross_or_target_diameter_value =
                disp_button_register("key_cross_or_diameter_val", um->disp,
                                     100, 90, 160, 55, um,
                                     key_cross_or_diameter_value_draw,
                                     on_click_cross_or_diameter_val);
    }
}

static void disp_main_win_destructor(void *mem)
{
    struct ui_main *um = (struct ui_main *)mem;
    kmem_deref(&um->disp);
    kmem_deref(&um->buttons);
}


static void ui_main_thread(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct machine *m = &machine;

    um = kzref_alloc("disp_main_win", sizeof *um, disp_main_win_destructor);
    um->disp = kmem_ref(m->disp1);
    show(um);

    if (is_button_held_down(m->switch_touch_lock))
        touch_enable(m->disp1->touch);
    else
        touch_disable(m->disp1->touch);

    while (1) {
        yield();

        if (is_button_changed(m->switch_touch_lock)) {
            if (is_button_held_down(m->switch_touch_lock))
                touch_enable(m->disp1->touch);
            else
                touch_disable(m->disp1->touch);
        }

        disp_button_handler();
    }
}

void ui_main_start(void)
{
    ui_tid = thread_register("ui_thread", 1500,
                             ui_main_thread, &ui_main);
}


