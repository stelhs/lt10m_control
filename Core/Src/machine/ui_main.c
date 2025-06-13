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


struct cmsis_thread *ui_tid;

struct ui_main_buttons {
    struct disp_button *sel_prog;
    struct disp_button *sel_feed_limit;
    struct disp_button *sel_feed_or_diameter;
    struct disp_button *sel_cross_feed;
    struct disp_button *sel_big_diameter;
    struct disp_button *sel_small_diameter;
    struct disp_button *sel_step;
};

struct ui_main {
    struct disp *disp;
    struct ui_main_buttons *umb;
    enum progs prog;
    float longitudial_limit;
    float big_diameter;
    float small_diameter;
    float cross_limit;
    float step;
    bool feed_or_diameter;
};
static struct ui_main *ui_main;


static void key_prog_sel_draw(struct disp_button *db)
{
    struct ui_main *um = (struct ui_main *)db->priv;
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, GRAY);

    img = img_prog_by_num(um->prog);
    disp_fill_img(db->disp, db->x + 2, db->y + 3, img);
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


static void key_feed_or_diameter_draw(struct disp_button *db)
{
    struct img *img;
    disp_rect(db->disp, db->x, db->y, db->width, db->height, 1, DARK_GRAY);
    img = img_feed_or_diameter();
    disp_fill_img(db->disp, db->x + 3, db->y + 3, img);
    kmem_deref(&img);
}

static void key_cross_feed_draw(struct disp_button *db)
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
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_UP_RIGHT:
        img = img_cross_feed_up(); break;
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_RIGHT_DOWN:
    case PROG_FEED_DOWN_LEFT:
    case PROG_FEED_DOWN_RIGHT:
        img = img_cross_feed_down(); break;
    default:
        img = NULL;
    }

    if (img) {
        disp_fill_img(db->disp, db->x + 5, db->y + 10, img);
        kmem_deref(&img);
    }

    string = kref_sprintf("%.2f", um->cross_limit);
    disp_text(db->disp, string, db->x + 80, db->y + 15, &ts);
    kmem_deref(&string);
}


static void key_big_diameter_draw(struct disp_button *db)
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

    img = img_big_circle();
    disp_fill_img(db->disp, db->x + 5, db->y + 10, img);
    kmem_deref(&img);

    string = kref_sprintf("%.2f", um->big_diameter);
    disp_text(db->disp, string, db->x + 80, db->y + 15, &ts);
    kmem_deref(&string);
}

static void key_small_diameter_draw(struct disp_button *db)
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

    img = img_small_circle();
    disp_fill_img(db->disp, db->x + 5, db->y + 10, img);
    kmem_deref(&img);

    string = kref_sprintf("%.2f", um->small_diameter);
    disp_text(db->disp, string, db->x + 80, db->y + 15, &ts);
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


static void main_win_items_destructor(void *mem)
{
    struct ui_main_buttons *umb = (struct ui_main_buttons *)mem;
    kmem_deref(&umb->sel_prog);
    kmem_deref(&umb->sel_feed_limit);
    kmem_deref(&umb->sel_feed_or_diameter);
    kmem_deref(&umb->sel_big_diameter);
    kmem_deref(&umb->sel_small_diameter);
    kmem_deref(&umb->sel_step);
}

static void show(struct ui_main *um)
{
    struct ui_main_buttons *umb;
    struct disp_button *db;
    umb = kzref_alloc("main_win_items", sizeof *umb,
                      main_win_items_destructor);
    um->umb = umb;

    disp_clear(um->disp);

    db = disp_button_create("key_prog_sel", um->disp, 100, 55, sizeof (void *));
    db->priv = um;
    disp_button_show(db, 30, 10, key_prog_sel_draw);
    umb->sel_prog = db;

    db = disp_button_create("key_feed_limit", um->disp, 200, 55, sizeof (void *));
    db->priv = um;
    disp_button_show(db, 10, 90, key_feed_limit_draw);
    umb->sel_feed_limit = db;

    db = disp_button_create("key_feed_or_diameter", um->disp, 80, 80, sizeof (void *));
    db->priv = um;
    disp_button_show(db, 230, 90 + (55 + 15) + 24, key_feed_or_diameter_draw);
    umb->sel_feed_or_diameter = db;

    if (um->feed_or_diameter) {
        db = disp_button_create("key_big_diameter", um->disp, 200, 55, sizeof (void *));
        db->priv = um;
        disp_button_show(db, 10, 90 + 55 + 15, key_big_diameter_draw);
        umb->sel_big_diameter = db;

        db = disp_button_create("key_small_diameter", um->disp, 200, 55, sizeof (void *));
        db->priv = um;
        disp_button_show(db, 10, 90 + (55 + 15) * 2, key_small_diameter_draw);
        umb->sel_small_diameter = db;
    } else {
        db = disp_button_create("key_cross_feed", um->disp, 200, 55, sizeof (void *));
        db->priv = um;
        disp_button_show(db, 10, 90 + 55 + 15 + 35, key_cross_feed_draw);
        umb->sel_cross_feed = db;
    }

    db = disp_button_create("key_step", um->disp, 200, 55, sizeof (void *));
    db->priv = um;
    disp_button_show(db, 10, 90 + (55 + 15) * 3, key_step_draw);
    umb->sel_step = db;
}

static void hide(struct ui_main *um)
{
    kmem_deref(&um->umb);
}

static void disp_main_win_destructor(void *mem)
{
    struct ui_main *um = (struct ui_main *)mem;
    kmem_deref(&um->disp);
    kmem_deref(&um->umb);
}


static void ui_main_thread(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct machine *m = &machine;
    struct ui_main_buttons *umb;

    um = kzref_alloc("disp_main_win", sizeof *um, disp_main_win_destructor);
    um->disp = kmem_ref(m->disp1);
    show(um);
    umb = um->umb;
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

        if (is_disp_button_touched(umb->sel_prog)) {
            hide(um);
            um->prog = ui_sel_prog_run();
            show(um);
        }

        if (is_disp_button_touched(umb->sel_feed_limit)) {
            hide(um);
            ui_keyboard_run("longitudial feed limit:", &um->longitudial_limit);
            show(um);
        }

        if (is_disp_button_touched(umb->sel_feed_or_diameter)) {
            hide(um);
            um->feed_or_diameter = !um->feed_or_diameter;
            show(um);
        }

        if (is_disp_button_touched(umb->sel_cross_feed)) {
            hide(um);
            ui_keyboard_run("cross feed limit:", &um->cross_limit);
            show(um);
        }

        if (is_disp_button_touched(umb->sel_big_diameter)) {
            hide(um);
            ui_keyboard_run("big diameter:", &um->big_diameter);
            show(um);
        }

        if (is_disp_button_touched(umb->sel_small_diameter)) {
            hide(um);
            ui_keyboard_run("small diameter:", &um->small_diameter);
            show(um);
        }

        if (is_disp_button_touched(umb->sel_step)) {
            hide(um);
            ui_keyboard_run("feed step:", &um->step);
            show(um);
        }

    }
}

void ui_main_start(void)
{
    ui_tid = thread_register("ui_thread", 1500,
                             ui_main_thread, &ui_main);
}


