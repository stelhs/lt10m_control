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
#include "periphery.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "images.h"
#include "ui_button.h"
#include "ui_input.h"
#include "ui_keyboard.h"
#include "ui_sel_prog.h"
#include "ui_set_xy.h"
#include "ui_move_to.h"
#include "abs_position.h"
#include "mode_thread.h"
#include "thread_table.h"
#include "msg_rus.h"


static void ui_scope_init(struct ui_main *um);

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

void ui_main_lock(void)
{
    struct machine *m = &machine;
    struct ui_main *um = m->ui_main;

    ui_buttons_lock(um->ui_scope);

    switch (m->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_RIGHT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_RIGHT_DOWN:
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_LEFT:
    case PROG_FEED_DOWN_RIGHT:
    case PROG_FEED_UP:
    case PROG_FEED_DOWN:
        ui_button_unlock(um->feed_return_mode);
        ui_button_unlock(um->feed_repeate);
        ui_button_unlock(um->feed_limit);
        ui_button_unlock(um->cross_or_diameter_val);
        ui_button_unlock(um->feed_step);
        break;

    case PROG_THREAD_LEFT:
    case PROG_THREAD_RIGHT:
        ui_button_unlock(um->thread_repeate);
        ui_button_unlock(um->thread_depth);
        ui_button_unlock(um->thread_offset);
        ui_button_unlock(um->feed_step);
        break;
    }
}

void ui_main_unlock(void)
{
    struct machine *m = &machine;
    struct ui_main *um = m->ui_main;
    ui_buttons_unlock(um->ui_scope);
}


static void key_prog_sel_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);

    img = img_prog_by_num(m->prog);
    disp_fill_img(ut->disp, ut->x + 2, ut->y + 3, img);
    kmem_deref(&img);
}

static void key_set_xy_show(struct ui_item *ut)
{
    struct img *img;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, GRAY);

    img = img_set_xy();
    disp_fill_img(ut->disp, ut->x + 20, ut->y + 4, img);
    kmem_deref(&img);
}


static void key_sel_feed_return_longitudal_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
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

static void key_sel_feed_return_cross_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
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

void key_thread_arrow_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
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


static void key_thread_repeate_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    struct img *img = NULL;
    char *string;

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
    disp_text(ut->disp, string, ut->x + 45, ut->y + 15, &input_ts);
    kmem_deref(&string);
}

static int onclick_thread_repeate(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    mt_settings->last_repeate_number ++;
    if (mt_settings->last_repeate_number >= 5)
        mt_settings->last_repeate_number = 0;
    ui_item_show(um->thread_repeate);
    return FALSE;
}

static void key_thread_moveto_start_show(struct ui_item *ut)
{
    struct img *img = img_thread_moveto_start();

    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 3, ut->y + 10, img);
        kmem_deref(&img);
    }
}

static void onclick_key_thread_moveto_start(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;

    if (m->is_busy)
        return;

    ui_item_hide(um->thread_moveto_start);
    set_normal_acceleration();
    longitudal_move_to(mt->start_longitudal_pos, TRUE, 0, NULL, NULL);
    buttons_reset();
    ui_item_show(um->thread_moveto_start);
}


static void key_thread_set_point_show(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    char str[16];

    struct text_style spindle_ts = {
            .bg_color = BLACK,
            .color = {241, 255, 146},
            .font = font_rus,
            .fontsize = 2,
    };

    struct text_style longitudal_ts = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 2,
    };

    struct img *img = NULL;

    img = img_thread_start_point();
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);
    if (img) {
        disp_fill_img(ut->disp, ut->x + 7, ut->y + 22, img);
        kmem_deref(&img);
    }

    snprintf(str, 12, "%d/%.1f`",
             mt_settings->spindle_start,
             (float)spindle_raw_to_angle(mt_settings->spindle_start) / 1000);
    disp_text(ut->disp, str, ut->x + 30, ut->y + 8, &spindle_ts);

    snprintf(str, 9, "%.3f", (float)mt_settings->longitudal_start / 1000);
    disp_text(ut->disp, str, ut->x + 30, ut->y + 32, &longitudal_ts);
}


static int onclick_key_thread_set_point(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    mt_settings->spindle_start = spindle_raw_angle();
    mt_settings->longitudal_start = abs_longitudal_curr_tool(m->ap);
    ui_item_show(ut);
    return FALSE;
}


static void key_thread_type_show(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
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

static int onclick_key_thread_type(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    if (mt_settings->is_type_inch) {
        mt_settings->is_type_inch = FALSE;
        ui_item_show(um->thread_standard_m);
    } else {
        mt_settings->is_type_inch = TRUE;
        ui_item_hide(um->thread_standard_m);
    }
    ui_item_show(um->thread_type);
    return FALSE;
}

static void key_feed_repeate_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
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

static void key_cross_or_diameter_show(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
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




static void key_cross_or_diameter_value_show(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    char *string;
    disp_rect(ut->disp, ut->x, ut->y, ut->width, ut->height, 1, DARK_GRAY);

    string = kref_sprintf("%.3f", mc_settings->cross_distance ?
                            (float)mc_settings->cross_distance / 1000 :
                            (float)mc_settings->target_diameter / 1000);
    string[8] = 0;
    disp_text(ut->disp, string, ut->x + 5, ut->y + 15, &input_ts);
    kmem_deref(&string);
}



static int on_click_prog_sel(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    if (is_switch_on(m->switch_run))
        return TRUE;
    ui_message_hide();
    kmem_deref(&um->ui_scope);
    m->prog = ui_sel_prog_run();
    mode_cut_settings_validate();
    ui_scope_init(um);
    return TRUE;
}

static int on_click_set_xy(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    if (is_switch_on(m->switch_run))
        return FALSE;

    ui_message_hide();
    kmem_deref(&um->ui_scope);
    ui_set_xy_run();
    ui_scope_init(um);
    return TRUE;
}

static int onclick_feed_return_longitudal(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (m->prog != PROG_FEED_LEFT &&
            m->prog != PROG_FEED_RIGHT) {
        return FALSE;
    }

    mc_settings->longitudal_ret_mode++;
    if (mc_settings->longitudal_ret_mode >= 5)
        mc_settings->longitudal_ret_mode = 0;

    if (mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_UP &&
            mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_DOWN) {
        mc_settings->last_repeate_number = 0;
    }

    ui_item_show(um->feed_return_mode);
    ui_item_show(um->feed_repeate);
    return FALSE;
}

static int onclick_key_thread_arrow(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    if (m->prog != PROG_THREAD_LEFT &&
            m->prog != PROG_THREAD_RIGHT) {
        return FALSE;
    }

    if (mt_settings->is_internal)
        mt_settings->is_internal = FALSE;
    else
        mt_settings->is_internal = TRUE;

    ui_item_show(um->thread_arrow);
    return FALSE;
}


static int onclick_feed_return_cross(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (m->prog != PROG_FEED_UP &&
            m->prog != PROG_FEED_DOWN) {
        return FALSE;
    }

    mc_settings->cross_ret_mode++;
    if (mc_settings->cross_ret_mode >= 5)
        mc_settings->cross_ret_mode = 0;

    ui_item_show(um->feed_return_mode);
    return FALSE;
}


static int onclick_feed_repeate(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (m->prog == PROG_FEED_LEFT ||
            m->prog == PROG_FEED_RIGHT) {
        if (mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_UP &&
                mc_settings->longitudal_ret_mode != CUT_LONGITUDAL_RETURN_DOWN) {
            mc_settings->last_repeate_number = 0;
            ui_item_show(um->feed_repeate);
            return FALSE;
        }
    }

    mc_settings->last_repeate_number ++;
    if (mc_settings->last_repeate_number >= 5)
        mc_settings->last_repeate_number = 0;
    ui_item_show(um->feed_repeate);
    return FALSE;
}

static int onclick_cross_or_diameter(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    kmem_deref(&um->ui_scope);
    if (mc_settings->cross_distance) {
        mc_settings->target_diameter = mc_settings->cross_distance * 2;
        mc_settings->cross_distance = 0;
    } else {
        mc_settings->cross_distance = mc_settings->target_diameter / 2;
    }
    mode_cut_settings_validate();
    ui_scope_init(um);
    return FALSE;
}

static int onclick_cross_or_diameter_val(struct ui_item *ut)
{
    struct ui_main *um = (struct ui_main *)ut->priv;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    ui_scope_hide(um->ui_scope);
    if (mc_settings->cross_distance)
        ui_keyboard_run("cross feed limit:", &mc_settings->cross_distance,
                        LINEAR_CROSS_RESOLUTION * 2, 1000 * 1000,
                        LINEAR_CROSS_RESOLUTION * 2, FALSE);
    else
        ui_keyboard_run("target diameter:", &mc_settings->target_diameter,
                        LINEAR_CROSS_RESOLUTION * 2, 1000 * 1000,
                        LINEAR_CROSS_RESOLUTION * 2, FALSE);
    ui_scope_show(um->ui_scope);
    return FALSE;
}

void show_longitudal(struct ui_main *um)
{
    struct machine *m = &machine;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    um->feed_return_mode =
            ui_button_register("key_sel_feed_return_longitudal", um->ui_scope,
                               0, 90, 70, 55,
                               key_sel_feed_return_longitudal_show,
                               onclick_feed_return_longitudal, um, 0);

    um->feed_limit =
            ui_input_register("input_feed_limit", um->ui_scope,
                              "longitudial feed limit:",
                              77, 90,
                              &mc_settings->longitudal_distance,
                              LINEAR_LONGITUDAL_RESOLUTION, 1500 * 1000,
                              LINEAR_LONGITUDAL_RESOLUTION, FALSE,
                              8, "%.3f", NULL, NULL);

    um->feed_repeate =
            ui_button_register("key_feed_repeate", um->ui_scope,
                               240, 90, 75, 55,
                               key_feed_repeate_show,
                               onclick_feed_repeate, um, 0);

    if (m->prog == PROG_FEED_LEFT_UP ||
            m->prog == PROG_FEED_RIGHT_UP ||
            m->prog == PROG_FEED_LEFT_DOWN ||
            m->prog == PROG_FEED_RIGHT_DOWN) {

        ui_button_register("key_cross_or_diameter", um->ui_scope,
                             0, 90 + (55 + 15) * 1, 70, 55,
                             key_cross_or_diameter_show,
                             onclick_cross_or_diameter, um, 0);

        um->cross_or_diameter_val =
                ui_button_register("key_cross_or_diameter_val", um->ui_scope,
                                   77, 90 + (55 + 15) * 1, 155, 55,
                                   key_cross_or_diameter_value_show,
                                   onclick_cross_or_diameter_val, um, 0);

        um->feed_step =
                ui_input_register("input_feed_step", um->ui_scope,
                                  "feed step:",
                                  0, 90 + (55 + 15) * 2,
                                  &mc_settings->cut_depth_step,
                                  5, 10 * 1000, 5, FALSE,
                                  6, "%.3f", NULL, img_step);
    }
}

void show_cross(struct ui_main *um)
{
    struct machine *m = &machine;
    struct mode_cut *mc = um->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    ui_button_register("key_cross_or_diameter", um->ui_scope,
                         0, 90, 70, 55,
                         key_cross_or_diameter_show,
                         onclick_cross_or_diameter, um, 0);

    um->cross_or_diameter_val =
            ui_button_register("key_cross_or_diameter_val", um->ui_scope,
                               77, 90, 155, 55,
                               key_cross_or_diameter_value_show,
                               onclick_cross_or_diameter_val, um, 0);

    um->feed_return_mode =
            ui_button_register("key_feed_return_mode", um->ui_scope,
                               240, 90, 75, 55,
                               key_sel_feed_return_cross_show,
                               onclick_feed_return_cross, um, 0);

    if (m->prog == PROG_FEED_UP_LEFT ||
            m->prog == PROG_FEED_UP_RIGHT ||
            m->prog == PROG_FEED_DOWN_LEFT ||
            m->prog == PROG_FEED_DOWN_RIGHT) {

        ui_item_register("ui_item_longitudal_arrow", um->ui_scope,
                         um->disp,
                         0, 90 + (55 + 15) * 1, 70, 55,
                         ui_item_longitudal_arrow_show,
                         NULL, um, 0);

        ui_input_register("input_feed_limit", um->ui_scope,
                          "longitudial feed limit:",
                          77, 90 + (55 + 15) * 1,
                          &mc_settings->longitudal_distance,
                          LINEAR_LONGITUDAL_RESOLUTION, 1500 * 1000,
                          LINEAR_LONGITUDAL_RESOLUTION, FALSE,
                          8, "%.3f", NULL, NULL);

        ui_input_register("input_cut_depth_step", um->ui_scope,
                          "feed step:",
                          0, 90 + (55 + 15) * 2,
                          &mc_settings->cut_depth_step,
                          5, 5 * 1000, 5, FALSE,
                          6, "%.3f", NULL, img_step);
    }
}

static void thread_standard_onchanged(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    const struct thread_metric *found_tm = NULL;
    const struct thread_metric *default_tm = NULL;
    const struct thread_metric *tm;
    mt_settings->tm = NULL;

    for (tm = thread_metric_table; tm->diameter; tm++) {
        if (tm->diameter != mt_settings->standart_diameter)
            continue;

        if (tm->is_default)
            default_tm = tm;

        if (tm->step == mt_settings->thread_size) {
            found_tm = tm;
            mt_settings->tm = tm;
            break;
        }
    }

    if (!found_tm && default_tm) {
        mt_settings->thread_size = default_tm->step;
        mt_settings->tm = default_tm;
        ui_item_show(um->thread_size);
    }

    if (mt_settings->tm) {
        int diameter = abs_cross_curr_tool(m->ap) * 2;
        struct thread_metric_info tmi;
        standart_thread_info(mt_settings->tm,
                             mt_settings->is_internal,
                             diameter, &tmi);
        mt_settings->max_cut_depth = tmi.depth;
        ui_item_show(um->thread_depth);
    }
}


static void thread_size_onchanged(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um = (struct ui_main *)priv;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    const struct thread_metric *tm;
    mt_settings->tm = NULL;

    if (mt_settings->is_type_inch)
        return;

    if (!mt_settings->standart_diameter)
        return;

    if(!standart_steps_list(mt_settings->standart_diameter, NULL))
        return;

    for (tm = thread_metric_table; tm->diameter; tm++) {
        if (tm->diameter != mt_settings->standart_diameter)
            continue;

        if (tm->step == mt_settings->thread_size) {
            mt_settings->tm = tm;
            break;
        }
    }

    if (mt_settings->tm) {
        int diameter = abs_cross_curr_tool(m->ap) * 2;
        struct thread_metric_info tmi;
        standart_thread_info(tm, mt_settings->is_internal, diameter, &tmi);
        mt_settings->max_cut_depth = tmi.depth;
        ui_item_show(um->thread_depth);
    }
}

static void ui_thread_info(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    struct disp *disp = m->disp2;
    int cut_speed;
    char str[40];

    struct text_style ts = {
            .bg_color = BLACK,
            .color = LIGHT_GRAY,
            .font = font_rus,
            .fontsize = 2,
    };

    struct text_style ts_red = {
            .bg_color = BLACK,
            .color = RED,
            .font = font_rus,
            .fontsize = 2,
    };

    struct text_style ts_green = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 2,
    };

    int text_height = disp_text_height(&ts);
    int row_height = text_height + 2;
    const int row_start = 120;
    int row = 0;

    int calc_time = thread_calc_work_time(mt_settings->length,
                                          mt->calc_cut_pass_num,
                                          m->sm_longitudial->max_freq);
    char *calc_time_str = interval_to_str(calc_time);
    snprintf(str, sizeof str, msg_thread_calc_time, calc_time_str);
    kmem_deref(&calc_time_str);
    disp_text(disp, str, 0, row_start + row_height * row, &ts);
    row ++;

    snprintf(str, sizeof str, msg_thread_feed_number, mt->calc_cut_pass_num);
    disp_text(disp, str, 0, row_start + row_height * row, &ts);
    row ++;

    snprintf(str, sizeof str, msg_thread_spindle_max, mt->krpm_max / 1000);
    disp_text(disp, str, 0, row_start + row_height * row, &ts);
    row ++;

    cut_speed = cut_speed_calculate(abs_cross_curr_tool(m->ap) * 2,
                                    mt->krpm_max / 1000);
    snprintf(str, sizeof str, msg_thread_cut_speed_max, (float)cut_speed / 1000);
    disp_text(disp, str, 0, row_start + row_height * row, &ts);
    row ++;

    if (mt_settings->is_type_inch)
        return;

    if (mt_settings->tm) {
        const struct thread_metric *tm = mt_settings->tm;
        char name[32];
        struct text_style *diameter_ts;
        int x_offset;
        int diameter = abs_cross_curr_tool(m->ap) * 2;
        struct thread_metric_info tmi;

        standart_thread_info(tm, mt_settings->is_internal, diameter, &tmi);

        if (tm->is_default) {
            snprintf(name, sizeof name, "M%d (%dx%.2f) d%.2f",
                     tm->diameter, tm->diameter, (float)tm->step / 1000,
                     (float)tm->drill_size / 1000);
        } else {
            snprintf(name, sizeof name, "M%dx%.2f d%.2f",
                     tm->diameter, (float)tm->step / 1000,
                     (float)tm->drill_size / 1000);
        }

        snprintf(str, sizeof str, msg_thread_m_standard, name);
        disp_text(disp, str, 0, row_start + row_height * row, &ts);
        row ++;

        if (diameter >= tmi.diameter_min && diameter <= tmi.diameter_max)
            diameter_ts = &ts_green;
        else
            diameter_ts = &ts_red;

        disp_text(disp, msg_thread_diameter_start, 0,
                  row_start + row_height * row, &ts);
        snprintf(str, sizeof str, "%.3f", (float)tmi.diameter / 1000);
        x_offset = disp_text_width(&ts, strlen(msg_thread_diameter_start));
        disp_text(disp, str, x_offset, row_start + row_height * row, diameter_ts);
        x_offset += disp_text_width(&ts, strlen(str));

        snprintf(str, sizeof str, " (%.3f-%.3f)", (float)tmi.diameter_min / 1000,
                 (float)tmi.diameter_max / 1000);
        disp_text(disp, str, x_offset, row_start + row_height * row, &ts);
        row ++;

        snprintf(str, sizeof str, msg_thread_diameter_end,
                 (float)tmi.end_diameter / 1000,
                 (float)tmi.end_diameter_min / 1000,
                 (float)tmi.end_diameter_max / 1000);
        disp_text(disp, str, 0, row_start + row_height * row, &ts);
        row ++;

        snprintf(str, sizeof str, msg_thread_max_depth,
                 (float)tmi.depth / 1000, (float)tmi.depth_min / 1000,
                 (float)tmi.depth_max / 1000);
        disp_text(disp, str, 0, row_start + row_height * row, &ts);
        row ++;
    }

    if (mt_settings->standart_diameter) {
        char list_str[36];
        u32 list[7];
        int count;

        count = standart_steps_list(mt_settings->standart_diameter, list);
        if (count) {
            int i;
            int pos = 0;
            for (i = 0; i < count; i++) {
                u32 step = list[i];
                pos += sprintf(list_str + pos, "%.2f,", (float)step / 1000);
            }
            list_str[pos - 1] = 0;
            snprintf(str, sizeof str, msg_thread_steps, list_str);
            disp_text(disp, str, 0, row_start + row_height * row, &ts);
            row ++;
        }
    }

}

static void thread_settings_onchange_cb(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct ui_main *um = m->ui_main;
    if (is_switch_on(m->switch_run))
        return;
    thread_state_init();
    ui_item_update(um->thread_info);
}


void show_thread(struct ui_main *um)
{
    struct machine *m = &machine;
    struct mode_thread *mt = um->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;

    um->thread_arrow =
            ui_button_register("key_thread_arrow", um->ui_scope,
                               0, 90 + (55 + 10) * 0, 70, 55,
                               key_thread_arrow_show,
                               onclick_key_thread_arrow, um, 0);


    ui_input_register("input_thread_length", um->ui_scope,
                      "thread length:",
                      77, 90 + (55 + 10) * 0,
                      &mt_settings->length,
                      LINEAR_LONGITUDAL_RESOLUTION, 1500 * 1000,
                      LINEAR_LONGITUDAL_RESOLUTION, FALSE,
                      8, "%.3f", NULL, NULL);

    um->thread_repeate =
            ui_button_register("key_thread_repeate", um->ui_scope,
                               235, 90 + (55 + 10) * 0, 75, 55,
                               key_thread_repeate_show,
                               onclick_thread_repeate, um, 0);

    um->thread_type =
            ui_button_register("key_thread_type", um->ui_scope,
                               0, 90 + (55 + 10) * 1, 70, 55,
                               key_thread_type_show,
                               onclick_key_thread_type, um, 0);

    um->thread_size =
            ui_input_register("input_thread_size", um->ui_scope,
                             "thread size:",
                              77, 90 + (55 + 10) * 1,
                              &mt_settings->thread_size,
                              200, 15000,
                              LINEAR_LONGITUDAL_RESOLUTION, FALSE,
                              8, "%.3f", NULL, NULL);

    ui_input_set_onchanged_cb(um->thread_size,
                              thread_size_onchanged, um);

    um->thread_standard_m =
            ui_input_register("input_standart_diameter", um->ui_scope,
                              "M",
                              235, 90 + (55 + 10) * 1,
                              &mt_settings->standart_diameter,
                              6, 600, 1, TRUE,
                              4, "%d", "M", NULL);

    ui_input_set_onchanged_cb(um->thread_standard_m,
                             thread_standard_onchanged, um);

    if (mt_settings->is_type_inch)
        ui_item_hide(um->thread_standard_m);

    um->feed_step =
            ui_input_register("input_cut_depth_step", um->ui_scope,
                              "cut depth step:",
                              0, 90 + (55 + 10) * 2,
                              &mt_settings->cut_depth_step,
                              5, 10 * 1000, 5, FALSE,
                              6, "%.3f", NULL, img_step);

    um->thread_depth =
            ui_input_register("input_max_cut_depth", um->ui_scope,
                              "max cut depth:",
                              0, 90 + (55 + 10) * 3,
                              &mt_settings->max_cut_depth,
                              10, 10 * 1000, 10, FALSE,
                              6, "%.3f", NULL, img_thread_depth);

    ui_input_register("input_thread_offset", um->ui_scope,
                      "thread offset:",
                      0, 90 + (55 + 10) * 4,
                      &mt_settings->thread_offset,
                      -20000, 20000, 5, FALSE,
                      6, "%.3f", NULL, img_thread_offset);

    ui_button_register("key_thread_set_point", um->ui_scope,
                       0, 90 + (55 + 10) * 5, 180, 55,
                       key_thread_set_point_show,
                       onclick_key_thread_set_point, um, 0);

    um->thread_moveto_start =
            ui_button_confirmation_register("key_thread_moveto_start",
                    um->ui_scope, 235, 90 + (55 + 10) * 5, 70, 55,
                    key_thread_moveto_start_show,
                    onclick_key_thread_moveto_start, um);

    um->thread_info =
            ui_item_register("ui_thread_info", NULL, m->disp2,
                             0, 120, 480, 200,
                             ui_thread_info, NULL, NULL, 0);
    
    ui_scope_set_onchange_handler(um->ui_scope, thread_settings_onchange_cb);
}


static void ui_scope_init(struct ui_main *um)
{
    struct machine *m = &machine;
    um->ui_scope = ui_scope_create("ui_main_scope");

    disp_clear(um->disp);

    ui_button_register("key_prog_sel", um->ui_scope,
                       30, 10, 100, 55,
                       key_prog_sel_show, on_click_prog_sel, um, 0);

    ui_button_register("key_set_xy", um->ui_scope,
                         190, 10, 100, 55,
                         key_set_xy_show, on_click_set_xy, um, 0);


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

    if (m->prog == PROG_FEED_UP ||
            m->prog == PROG_FEED_DOWN) {

        ui_button_register("key_cross_or_diameter", um->ui_scope,
                             10, 90, 70, 55,
                             key_cross_or_diameter_show,
                             onclick_cross_or_diameter, um, 0);

        ui_button_register("key_cross_or_diameter_val", um->ui_scope,
                             100, 90, 160, 55,
                             key_cross_or_diameter_value_show,
                             onclick_cross_or_diameter_val, um, 0);
    }

    if (m->prog == PROG_THREAD_LEFT ||
            m->prog == PROG_THREAD_RIGHT) {
        show_thread(um);
        return;
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

void program_finished_show(void)
{
    struct machine *m = &machine;
    struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3,
    };
    disp_text(m->disp2, msg_prog_finished, 184, 270, &ts);
}

static void ui_main_destructor(void *mem)
{
    struct ui_main *um = (struct ui_main *)mem;
    kmem_deref(&um->ui_scope);
}


static void ui_main_thread(void *priv)
{
    struct machine *m = &machine;
    struct ui_main *um;
    um = kzref_alloc("ui_main", sizeof *um, ui_main_destructor);
    m->ui_main = um;
    um->disp = m->disp1;
    um->mc = &m->mc;
    um->mt = &m->mt;

    ui_scope_init(um);

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
            kmem_deref(&um->ui_scope); // TODO clean disp2
            ui_move_to_run();
            ui_scope_init(um);
        }
    }
}

void ui_main_start(void)
{
    struct machine *m = &machine;
    m->ui_tid = thread_register("ui_thread", 1500,
                                ui_main_thread, NULL);
}


