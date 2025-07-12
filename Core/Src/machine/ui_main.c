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
#include "stm32_lib/buf.h"
#include "machine.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "images.h"
#include "ui_button.h"
#include "ui_keyboard.h"
#include "ui_sel_prog.h"
#include "ui_set_xy.h"
#include "ui_move_to.h"
#include "abs_position.h"
#include "mode_thread.h"


struct ui_main_items {
    struct ui_button *sel_prog;
    struct ui_button *set_xy;
    struct ui_button *sel_feed_return_mode;
    struct ui_item *ui_item_longitudal_arrow;
    struct ui_button *sel_feed_limit;
    struct ui_button *sel_feed_repeate;
    struct ui_button *sel_cross_or_target_diameter;
    struct ui_button *sel_cross_or_target_diameter_value;
    struct ui_button *sel_cut_depth;
    struct ui_button *sel_thread_type;
    struct ui_button *sel_thread_size;
    struct ui_button *sel_thread_arrow;
    struct ui_item *ui_item_thread_cut_depth;
};

char *interval_to_str(int time_sec)
{
    int sec;
    int min;
    int hour;

    if (time_sec < 60)
        return kref_sprintf("%ds", time_sec);

    if (time_sec < 60*60) {
        min = time_sec / 60;
        sec = time_sec - min * 60;
        return kref_sprintf("%dm,%ds", min, sec);
    }
    hour = time_sec / 3600;
    min = (time_sec % 3600) / 60;
    sec = time_sec - hour * 3600 - min * 60;
    return kref_sprintf("%dh,%dm,%ds", hour, min, sec);
}

static void show(struct ui_main *um);
static void hide(struct ui_main *um)
{
    kmem_deref(&um->ui_items);
}


static void key_prog_sel_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);

    img = img_prog_by_num(m->prog);
    disp_fill_img(ut->disp, ut->x + 2, ut->y + 3, img);
    kmem_deref(&img);
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


static void key_sel_feed_return_longitudal_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    struct img *img = NULL;

    switch (m->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_LEFT_DOWN: {
        switch (mc_settings->longitudal_ret_mode) {
        case CUT_LONGITUDAL_NO_RETURN:
            img = img_feed_arrow_left(); break;
        case CUT_LONGITUDAL_MOVE_UP:
            img = img_feed_arrow_left_up(); break;
        case CUT_LONGITUDAL_MOVE_DOWN:
            img = img_feed_arrow_left_down(); break;
        case CUT_LONGITUDAL_RETURN_UP:
            img = img_feed_arrow_left_up_return(); break;
        case CUT_LONGITUDAL_RETURN_DOWN:
            img = img_feed_arrow_left_down_return(); break;
        default:
            break;
        }
        break;
    }

    case PROG_FEED_RIGHT:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_RIGHT_DOWN: {
        switch (mc_settings->longitudal_ret_mode) {
        case CUT_LONGITUDAL_NO_RETURN:
            img = img_feed_arrow_right(); break;
        case CUT_LONGITUDAL_MOVE_UP:
            img = img_feed_arrow_right_up(); break;
        case CUT_LONGITUDAL_MOVE_DOWN:
            img = img_feed_arrow_right_down(); break;
        case CUT_LONGITUDAL_RETURN_UP:
            img = img_feed_arrow_right_up_return(); break;
        case CUT_LONGITUDAL_RETURN_DOWN:
            img = img_feed_arrow_right_down_return(); break;
        default:
            break;
        }
        break;
    }

    default:
        break;
    }

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 5, ut->y + 10, img);
        kmem_deref(&img);
    }
}

static void key_sel_feed_return_cross_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    struct img *img = NULL;

    switch (m->prog) {
    case PROG_FEED_UP:
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_UP_RIGHT: {
        switch (mc_settings->cross_ret_mode) {
        case CUT_CROSS_NO_RETURN:
            img = img_feed_arrow_up(); break;
        case CUT_CROSS_MOVE_LEFT:
            img = img_feed_arrow_up_left(); break;
        case CUT_CROSS_MOVE_RIGHT:
            img = img_feed_arrow_up_right(); break;
        case CUT_CROSS_RETURN_LEFT:
            img = img_feed_arrow_up_left_return(); break;
        case CUT_CROSS_RETURN_RIGHT:
            img = img_feed_arrow_up_right_return(); break;
        default:
            break;
        }
        break;
    }

    case PROG_FEED_DOWN:
    case PROG_FEED_DOWN_LEFT:
    case PROG_FEED_DOWN_RIGHT: {
        switch (mc_settings->cross_ret_mode) {
        case CUT_CROSS_NO_RETURN:
            img = img_feed_arrow_down(); break;
        case CUT_CROSS_MOVE_LEFT:
            img = img_feed_arrow_down_left(); break;;
        case CUT_CROSS_MOVE_RIGHT:
            img = img_feed_arrow_down_right(); break;
        case CUT_CROSS_RETURN_LEFT:
            img = img_feed_arrow_down_left_return(); break;
        case CUT_CROSS_RETURN_RIGHT:
            img = img_feed_arrow_down_right_return(); break;
        default:
            break;
        }
        break;
    }

    default:
        break;
    }

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 20, ut->y + 5, img);
        kmem_deref(&img);
    }
}

void ui_item_longitudal_arrow_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct img *img = NULL;

    switch (m->prog) {
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_DOWN_LEFT:
        img = img_feed_arrow_left(); break;
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_RIGHT:
        img = img_feed_arrow_right(); break;
    default:
        break;
    }

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 5, ut->y + 10, img);
        kmem_deref(&img);
    }
}

void key_thread_arrow_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    struct img *img = NULL;

    switch (m->prog) {
    case PROG_THREAD_LEFT:
        if (mt_settings->is_internal)
            img = img_feed_arrow_left_down2();
        else
            img = img_feed_arrow_left_up2();
        break;

    case PROG_THREAD_RIGHT:
        if (mt_settings->is_internal)
            img = img_feed_arrow_right_down2();
        else
            img = img_feed_arrow_right_up2();
        break;
    default:
        break;
    }

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 5, ut->y + 10, img);
        kmem_deref(&img);
    }
}

static void key_thread_length_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    string = kref_sprintf("%.3f", (float)mt_settings->length / 1000);
    string[8] = 0;
    disp_text(ut->disp, string, ut->x + 5, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void onclick_thread_length(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    hide(um);
    ui_keyboard_run("thread length:", &mt_settings->length,
                    LINEAR_LONGITUDAL_RESOLUTION, 1500 * 1000,
                    LINEAR_LONGITUDAL_RESOLUTION);
    show(um);
}

static void key_thread_size_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    string = kref_sprintf("%.3f", (float)mt_settings->thread_size / 1000);
    string[8] = 0;
    disp_text(ut->disp, string, ut->x + 5, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void onclick_key_thread_size(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    hide(um);
    ui_keyboard_run("thread size:", &mt_settings->thread_size,
                    LINEAR_LONGITUDAL_RESOLUTION, 1500 * 1000,
                    LINEAR_LONGITUDAL_RESOLUTION);
    show(um);
}

void ui_item_thread_cut_depth_show(struct ui_item *ut)
{
    struct img *img = NULL;
    img = img_thread_cut_depth();
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 5, ut->y + 10, img);
        kmem_deref(&img);
    }
}

static void key_thread_cut_depth_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    string = kref_sprintf("%.3f", (float)mt_settings->cut_depth / 1000);
    string[8] = 0;
    disp_text(ut->disp, string, ut->x + 5, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void onclick_key_thread_cut_depth(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    hide(um);
    ui_keyboard_run("thread size:", &mt_settings->cut_depth,
                    LINEAR_LONGITUDAL_RESOLUTION, 5000,
                    LINEAR_LONGITUDAL_RESOLUTION);
    show(um);
}


static void key_thread_repeate_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    struct img *img = NULL;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    switch (m->prog) {
    case PROG_THREAD_LEFT:
        img = img_feed_many_arrows_left(); break;
    case PROG_THREAD_RIGHT:
        img = img_feed_many_arrows_right(); break;
    default:
        break;
    }

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 7, ut->y + 10, img);
        kmem_deref(&img);
    }

    string = kref_sprintf("%d", mt_settings->last_repeate_number);
    disp_text(ut->disp, string, ut->x + 45, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void onclick_thread_repeate(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    mt_settings->last_repeate_number ++;
    if (mt_settings->last_repeate_number >= 5)
        mt_settings->last_repeate_number = 0;
    ui_button_show(um->ui_items->sel_feed_repeate);
}

static void key_thread_type_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    struct img *img = NULL;

    if (mt_settings->is_type_inch)
        img = img_thread_inch();
    else
        img = img_thread_mm();

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 7, ut->y + 10, img);
        kmem_deref(&img);
    }
}

static void onclick_key_thread_type(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    if (mt_settings->is_type_inch)
        mt_settings->is_type_inch = FALSE;
    else
        mt_settings->is_type_inch = TRUE;
    ui_button_show(um->ui_items->sel_thread_type);
}

static void key_longitudal_feed_limit_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    string = kref_sprintf("%.3f", (float)mc_settings->longitudal_distance / 1000);
    string[8] = 0;
    disp_text(ut->disp, string, ut->x + 5, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void key_feed_repeate_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    struct img *img = NULL;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    switch (m->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_DOWN_LEFT:
        img = img_feed_many_arrows_left(); break;
    case PROG_FEED_RIGHT:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_RIGHT_DOWN:
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_RIGHT:
        img = img_feed_many_arrows_right(); break;
    default:
        break;
    }

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 7, ut->y + 10, img);
        kmem_deref(&img);
    }

    string = kref_sprintf("%d", mc_settings->last_repeate_number);
    disp_text(ut->disp, string, ut->x + 45, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void key_cross_or_diameter_show(struct ui_button *ub)
{
    struct machine *m = &machine;
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    struct img *img;

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);

    if (mc_settings->cross_distance) {
        switch (m->prog) {
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
    } else {
        img = img_big_circle();
    }

    disp_fill_img(ut->disp, ut->x + 17, ut->y + 9, img);
    kmem_deref(&img);
}




static void key_cross_or_diameter_value_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);

    string = kref_sprintf("%.3f", mc_settings->cross_distance ?
                            (float)mc_settings->cross_distance / 1000 :
                            (float)mc_settings->target_diameter / 1000);
    string[8] = 0;
    disp_text(ut->disp, string, ut->x + 5, ut->y + 15, &ts);
    kmem_deref(&string);
}

static void key_cut_depth_show(struct ui_button *ub)
{
    struct ui_item *ut = ub->ut;
    struct ui_main *um = (struct ui_main *)ub->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    struct img *img;
    char *string;
    static struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3
    };

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);

    img = img_step();
    disp_fill_img(ut->disp, ut->x + 5, ut->y + 10, img);
    kmem_deref(&img);

    string = kref_sprintf("%.3f", (float)mc_settings->cut_depth / 1000);
    disp_text(ut->disp, string, ut->x + 80, ut->y + 15, &ts);
    kmem_deref(&string);
}


static void on_click_prog_sel(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    if (is_switch_on(m->switch_run))
        return;

    hide(um);
    m->prog = ui_sel_prog_run();
    mode_cut_settings_validate();
    show(um);
}

static void on_click_set_xy(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    if (is_switch_on(m->switch_run))
        return;

    hide(um);
    ui_set_xy_run();
    show(um);
}

static void onclick_feed_return_longitudal(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (m->prog != PROG_FEED_LEFT &&
            m->prog != PROG_FEED_RIGHT) {
        return;
    }

    mc_settings->longitudal_ret_mode++;
    if (mc_settings->longitudal_ret_mode >= 5)
        mc_settings->longitudal_ret_mode = 0;

    if (mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_UP &&
            mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_DOWN) {
        mc_settings->last_repeate_number = 0;
    }

    ui_button_show(um->ui_items->sel_feed_return_mode);
    ui_button_show(um->ui_items->sel_feed_repeate);
}

static void onclick_key_thread_arrow(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    if (m->prog != PROG_THREAD_LEFT &&
            m->prog != PROG_THREAD_RIGHT) {
        return;
    }

    if (mt_settings->is_internal)
        mt_settings->is_internal = FALSE;
    else
        mt_settings->is_internal = TRUE;

    ui_button_show(um->ui_items->sel_thread_arrow);
}


static void onclick_feed_return_cross(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (m->prog != PROG_FEED_UP &&
            m->prog != PROG_FEED_DOWN) {
        return;
    }

    mc_settings->cross_ret_mode++;
    if (mc_settings->cross_ret_mode >= 5)
        mc_settings->cross_ret_mode = 0;

    ui_button_show(um->ui_items->sel_feed_return_mode);
}


static void onclick_longitudal_feed_limit(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    hide(um);
    ui_keyboard_run("longitudial feed limit:", &mc_settings->longitudal_distance,
                    LINEAR_LONGITUDAL_RESOLUTION, 1500 * 1000,
                    LINEAR_LONGITUDAL_RESOLUTION);
    show(um);
}

static void onclick_feed_repeate(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (m->prog == PROG_FEED_LEFT ||
            m->prog == PROG_FEED_RIGHT) {
        if (mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_UP &&
                mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_DOWN) {
            mc_settings->last_repeate_number = 0;
            ui_button_show(um->ui_items->sel_feed_repeate);
            return;
        }
    }

    mc_settings->last_repeate_number ++;
    if (mc_settings->last_repeate_number >= 5)
        mc_settings->last_repeate_number = 0;
    ui_button_show(um->ui_items->sel_feed_repeate);
}

static void onclick_cross_or_diameter(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    hide(um);
    if (mc_settings->cross_distance) {
        mc_settings->target_diameter = mc_settings->cross_distance * 2;
        mc_settings->cross_distance = 0;
    } else {
        mc_settings->cross_distance = mc_settings->target_diameter / 2;
    }
    mode_cut_settings_validate();
    show(um);
}

static void onclick_cross_or_diameter_val(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    hide(um);
    if (mc_settings->cross_distance)
        ui_keyboard_run("cross feed limit:", &mc_settings->cross_distance,
                        LINEAR_CROSS_RESOLUTION * 2, 1000 * 1000,
                        LINEAR_CROSS_RESOLUTION * 2);
    else
        ui_keyboard_run("target diameter:", &mc_settings->target_diameter,
                        LINEAR_CROSS_RESOLUTION * 2, 1000 * 1000,
                        LINEAR_CROSS_RESOLUTION * 2);
    show(um);
}

static void onclick_cut_depth(void *priv)
{
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    hide(um);
    ui_keyboard_run("feed step:", &mc_settings->cut_depth, 5, 10 * 1000, 5);
    show(um);
}

static void main_win_buttons_destructor(void *mem)
{
    struct ui_main_items *umi = (struct ui_main_items *)mem;
    kmem_deref(&umi->sel_prog);
    kmem_deref(&umi->set_xy);
    if (umi->sel_feed_return_mode)
        kmem_deref(&umi->sel_feed_return_mode);

    if (umi->ui_item_longitudal_arrow)
        kmem_deref(&umi->ui_item_longitudal_arrow);

    if (umi->sel_feed_limit)
        kmem_deref(&umi->sel_feed_limit);

    if (umi->sel_feed_repeate)
        kmem_deref(&umi->sel_feed_repeate);

    if (umi->sel_cross_or_target_diameter)
        kmem_deref(&umi->sel_cross_or_target_diameter);

    if (umi->sel_cross_or_target_diameter_value)
        kmem_deref(&umi->sel_cross_or_target_diameter_value);

    if (umi->sel_cut_depth)
        kmem_deref(&umi->sel_cut_depth);

    if (umi->sel_thread_type)
        kmem_deref(&umi->sel_thread_type);

    if (umi->sel_thread_size)
        kmem_deref(&umi->sel_thread_size);

    if (umi->ui_item_thread_cut_depth)
        kmem_deref(&umi->ui_item_thread_cut_depth);

    if (umi->sel_thread_arrow)
        kmem_deref(&umi->sel_thread_arrow);
}

void show_longitudal(struct ui_main *um)
{
    struct machine *m = &machine;
    struct ui_main_items *ui_items = um->ui_items;

    ui_items->sel_feed_return_mode =
            ui_button_register("key_sel_feed_return_longitudal", um->disp,
                                 0, 90, 70, 55,
                                 key_sel_feed_return_longitudal_show,
                                 onclick_feed_return_longitudal, um);

    ui_items->sel_feed_limit =
            ui_button_register("key_feed_limit", um->disp,
                                 77, 90, 155, 55,
                                 key_longitudal_feed_limit_show,
                                 onclick_longitudal_feed_limit, um);

    ui_items->sel_feed_repeate =
            ui_button_register("key_feed_repeate", um->disp,
                                 240, 90, 75, 55,
                                 key_feed_repeate_show,
                                 onclick_feed_repeate, um);

    if (m->prog == PROG_FEED_LEFT_UP ||
            m->prog == PROG_FEED_RIGHT_UP ||
            m->prog == PROG_FEED_LEFT_DOWN ||
            m->prog == PROG_FEED_RIGHT_DOWN) {

        ui_items->sel_cross_or_target_diameter =
                ui_button_register("key_cross_or_diameter", um->disp,
                                     0, 90 + (55 + 15) * 1, 70, 55,
                                     key_cross_or_diameter_show,
                                     onclick_cross_or_diameter, um);

        ui_items->sel_cross_or_target_diameter_value =
                ui_button_register("key_cross_or_diameter_val", um->disp,
                                     77, 90 + (55 + 15) * 1, 155, 55,
                                     key_cross_or_diameter_value_show,
                                     onclick_cross_or_diameter_val, um);

        ui_items->sel_cut_depth =
                ui_button_register("key_cut_depth", um->disp,
                                     0, 90 + (55 + 15) * 2, 200, 55,
                                     key_cut_depth_show,
                                     onclick_cut_depth, um);
    }
}

void show_cross(struct ui_main *um)
{
    struct machine *m = &machine;
    struct ui_main_items *ui_items = um->ui_items;

    ui_items->sel_cross_or_target_diameter =
            ui_button_register("key_cross_or_diameter", um->disp,
                                 0, 90, 70, 55,
                                 key_cross_or_diameter_show,
                                 onclick_cross_or_diameter, um);

    ui_items->sel_cross_or_target_diameter_value =
            ui_button_register("key_cross_or_diameter_val", um->disp,
                                 77, 90, 155, 55,
                                 key_cross_or_diameter_value_show,
                                 onclick_cross_or_diameter_val, um);

    ui_items->sel_feed_return_mode =
            ui_button_register("key_feed_return_mode", um->disp,
                                 240, 90, 75, 55,
                                 key_sel_feed_return_cross_show,
                                 onclick_feed_return_cross, um);

    if (m->prog == PROG_FEED_UP_LEFT ||
            m->prog == PROG_FEED_UP_RIGHT ||
            m->prog == PROG_FEED_DOWN_LEFT ||
            m->prog == PROG_FEED_DOWN_RIGHT) {

        ui_items->ui_item_longitudal_arrow =
                ui_item_register("ui_item_longitudal_arrow", um->disp,
                                     0, 90 + (55 + 15) * 1, 70, 55,
                                     ui_item_longitudal_arrow_show,
                                     NULL, um, 0);

        ui_items->sel_feed_limit =
                ui_button_register("key_feed_limit", um->disp,
                                     77, 90 + (55 + 15) * 1, 155, 55,
                                     key_longitudal_feed_limit_show,
                                     onclick_longitudal_feed_limit, um);

        ui_items->sel_cut_depth =
                ui_button_register("key_cut_depth", um->disp,
                                     0, 90 + (55 + 15) * 2, 200, 55,
                                     key_cut_depth_show,
                                     onclick_cut_depth, um);
    }
}


void show_thread(struct ui_main *um)
{
    struct ui_main_items *ui_items = um->ui_items;

    ui_items->sel_thread_arrow =
            ui_button_register("key_thread_arrow", um->disp,
                                 0, 90 + (55 + 15) * 0, 70, 55,
                                 key_thread_arrow_show,
                                 onclick_key_thread_arrow, um);

    ui_items->sel_feed_limit =
            ui_button_register("key_thread_length", um->disp,
                                 77, 90 + (55 + 15) * 0, 155, 55,
                                 key_thread_length_show,
                                 onclick_thread_length, um);

    ui_items->sel_feed_repeate =
            ui_button_register("key_thread_repeate", um->disp,
                                 240, 90 + (55 + 15) * 0, 75, 55,
                                 key_thread_repeate_show,
                                 onclick_thread_repeate, um);

    ui_items->sel_thread_type =
            ui_button_register("key_thread_type", um->disp,
                                 0, 90 + (55 + 15) * 1, 70, 55,
                                 key_thread_type_show,
                                 onclick_key_thread_type, um);

    ui_items->sel_thread_size =
            ui_button_register("key_thread_size", um->disp,
                                 77, 90 + (55 + 15) * 1, 155, 55,
                                 key_thread_size_show,
                                 onclick_key_thread_size, um);

    ui_items->ui_item_thread_cut_depth =
            ui_item_register("ui_item_thread_cut_depth", um->disp,
                                 0, 90 + (55 + 15) * 2, 70, 55,
                                 ui_item_thread_cut_depth_show,
                                 NULL, um, 0);

    ui_items->sel_cut_depth =
            ui_button_register("key_thread_cut_depth", um->disp,
                                 77, 90 + (55 + 15) * 2, 155, 55,
                                 key_thread_cut_depth_show,
                                 onclick_key_thread_cut_depth, um);
}



static void show(struct ui_main *um)
{
    struct machine *m = &machine;
    struct ui_main_items *ui_items;
    ui_items = kzref_alloc("main_win_buttons", sizeof *ui_items,
                      main_win_buttons_destructor);
    um->ui_items = ui_items;

    disp_clear(um->disp);

    ui_items->sel_prog =
            ui_button_register("key_prog_sel", um->disp,
                                 30, 10, 100, 55,
                                 key_prog_sel_show, on_click_prog_sel, um);

    ui_items->set_xy =
            ui_button_register("key_set_xy", um->disp,
                                 190, 10, 100, 55,
                                 key_set_xy_show, on_click_set_xy, um);


    if (m->prog == PROG_FEED_LEFT ||
            m->prog == PROG_FEED_RIGHT ||
            m->prog == PROG_FEED_LEFT_UP ||
            m->prog == PROG_FEED_RIGHT_UP ||
            m->prog == PROG_FEED_LEFT_DOWN ||
            m->prog == PROG_FEED_RIGHT_DOWN) {
        show_longitudal(um);
        return;
    }

    if (m->prog == PROG_FEED_UP ||
            m->prog == PROG_FEED_DOWN ||
            m->prog == PROG_FEED_UP_LEFT ||
            m->prog == PROG_FEED_UP_RIGHT ||
            m->prog == PROG_FEED_DOWN_LEFT ||
            m->prog == PROG_FEED_DOWN_RIGHT) {
        show_cross(um);
        return;
    }

    if (m->prog == PROG_THREAD_LEFT ||
            m->prog == PROG_THREAD_RIGHT) {
        show_thread(um);
        return;
    }
    if (m->prog == PROG_FEED_UP ||
            m->prog == PROG_FEED_DOWN) {

        ui_items->sel_cross_or_target_diameter =
                ui_button_register("key_cross_or_diameter", um->disp,
                                     10, 90, 70, 55,
                                     key_cross_or_diameter_show,
                                     onclick_cross_or_diameter, um);

        ui_items->sel_cross_or_target_diameter_value =
                ui_button_register("key_cross_or_diameter_val", um->disp,
                                     100, 90, 160, 55,
                                     key_cross_or_diameter_value_show,
                                     onclick_cross_or_diameter_val, um);
    }

}

void ui_message_hide(void)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    disp_fill(disp, 0, 115, 480, 205, BLACK);
    m->is_disp2_needs_redraw = TRUE;
}

void ui_message_show(char *msg, enum msg_type mt)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct list *rows;
    struct le *le;
    struct buf *msg_buf = buf_strdub(msg);
    struct text_style *ts;
    const int max_width = 480 - 4;
    const int max_height = 205 - 4;
    int row_num;
    struct text_style ts_err = {
        .font = font_rus,
        .fontsize = 3,
        .color = RED,
        .bg_color = DARK_GRAY
    };

    struct text_style ts_norm = {
        .font = font_rus,
        .fontsize = 3,
        .color = GREEN,
        .bg_color = DARK_GRAY
    };

    struct text_style ts_warn = {
        .font = font_rus,
        .fontsize = 3,
        .color = YELLOW,
        .bg_color = DARK_GRAY
    };

    switch(mt) {
    case MSG_ERR:
        ts = &ts_err; break;
    case MSG_WARN:
        ts = &ts_warn; break;
    case MSG_NORM:
        ts = &ts_norm; break;
    }

    rows = buf_split(msg_buf, '\n');

    disp_rect(disp, 0, 115, 480, 205, 2, ts->color);
    disp_fill(disp, 2, 117, max_width, max_height, DARK_GRAY);

    row_num = 0;
    LIST_FOREACH(rows, le) {
        struct buf *msg = (struct buf *)list_ledata(le);
        int row_width = disp_text_width(ts, buf_len(msg));
        int row_height = disp_text_height(ts);
        int x = (max_width / 2) - (row_width / 2);
        int y = 127 + (row_height + 2) * row_num;
        disp_text(disp, (char *)msg->d, x, y, ts);
        row_num ++;
    }

    kmem_deref(&rows);
    kmem_deref(&msg_buf);
}

static void disp_main_win_destructor(void *mem)
{
    struct ui_main *um = (struct ui_main *)mem;
    kmem_deref(&um->ui_items);
}


static void ui_main_thread(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um;
    um = kzref_alloc("ui_main", sizeof *um, disp_main_win_destructor);
    m->ui_main = um;
    um->disp = m->disp1;
    um->mc = &m->mc;
    um->mt = &m->mt;

    show(um);

    if (is_button_held_down(m->switch_touch_lock))
        touch_enable(m->disp1->touch);
    else
        touch_disable(m->disp1->touch);

    while (1) {
        yield();

        ui_button_handler();

        if (is_switch_on(m->switch_run))
            continue;

        if (is_switch_on(m->switch_move_to)) {
            buttons_reset();
            hide(um);
            ui_move_to_run();
            show(um);
        }
    }
}

void ui_main_start(void)
{
    struct machine *m = &machine;
    m->ui_tid = thread_register("ui_thread", 1500,
                                ui_main_thread, NULL);
}


