/*
 * mode_cut.c
 *
 *  Created on: Jun 26, 2025
 *      Author: stelhs
 */

#include <stdlib.h>
#include "mode_cut.h"
#include "machine.h"
#include "ui_main.h"
#include "ui_item.h"
#include "periphery.h"
#include "abs_position.h"
#include "stm32_lib/buttons.h"
#include "stm32_lib/kref_alloc.h"
#include "msg_rus.h"
#include "periphery.h"
#include "images.h"


struct mode_cut_ui_items {
    struct ui_item *text;
    struct ui_item *uptime;
    struct ui_item *number_passes;
    struct ui_item *cutting_speed;
    struct ui_item *feed_speed;
    struct ui_item *start_cross;
    struct ui_item *start_longitudal;
    struct ui_item *relative_cross;
    struct ui_item *finish_cross;
    struct ui_item *finish_longitudal;
    struct ui_item *relative_longitudal;
    struct ui_item *cross_miss_up;
    struct ui_item *cross_miss_down;
    struct ui_item *longitudal_miss_left;
    struct ui_item *longitudal_miss_right;
};

struct text_style normal_text_style = {
        .bg_color = BLACK,
        .color = LIGHT_GRAY,
        .font = font_rus,
        .fontsize = 2,
};

struct text_style cross_text_style = {
        .bg_color = BLACK,
        .color = GREEN,
        .font = font_rus,
        .fontsize = 2,
};

struct text_style longitudal_text_style = {
        .bg_color = BLACK,
        .color = BLUE,
        .font = font_rus,
        .fontsize = 2,
};


static void ui_uptime_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    u32 uptime = (now() - mc->stat.start_time) / 1000;
    snprintf(str, size, "%lu/%d", uptime, mc->stat.calc_time);
}

static void ui_number_passes_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%d/%d", mc->cut_pass_cnt, mc->cut_pass_rest);
}

static void ui_cutting_speed_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "0.715");
    UNUSED(mc); // TODO
}

static void ui_feed_speed_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "0.045");
    UNUSED(mc); // TODO
}

static void ui_start_cross_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->start_cross_pos / 1000 * 2);
}

static void ui_finish_cross_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->end_cross_pos / 1000 * 2);
}

static void ui_relative_cross_getter(struct ui_item *ut, char *str, size_t size)
{
    struct machine *m = &machine;
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    struct stepper_motor *sm = m->sm_cross_feed;
    int pos = stepper_motor_pos(sm);
    int percent;

    switch (mc->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_RIGHT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_RIGHT_DOWN:
    case PROG_THREAD_LEFT:
    case PROG_THREAD_RIGHT:
        if (mc->cross_distance)
            percent = (abs(mc->cross_pos - mc->start_cross_pos) * 100) /
                            mc->cross_distance;
        else
            percent = 0;
        break;
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_LEFT:
    case PROG_FEED_DOWN_RIGHT:
    case PROG_FEED_UP:
    case PROG_FEED_DOWN:
        if (mc->cross_distance)
            percent = (pos * 100) / mc->cross_distance;
        else
            percent = 0;
        break;
    }
    snprintf(str, size, "%d%%", percent);
}

static void ui_start_longitudal_getter(struct ui_item *ut,
                                       char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->start_longitudal_pos / 1000);
}

static void ui_finish_longitudal_getter(struct ui_item *ut,
                                        char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->end_longitudal_pos / 1000);
}

static void ui_relative_longitudal_getter(struct ui_item *ut,
                                          char *str, size_t size)
{
    struct machine *m = &machine;
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    struct stepper_motor *sm = m->sm_longitudial_feed;
    int pos = stepper_motor_pos(sm);
    int percent;

    switch (mc->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_RIGHT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_RIGHT_DOWN:
    case PROG_THREAD_LEFT:
    case PROG_THREAD_RIGHT:
        if (mc->longitudal_distance)
            percent = (pos * 100) / mc->longitudal_distance;
        else
            percent = 0;
        break;
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_LEFT:
    case PROG_FEED_DOWN_RIGHT:
    case PROG_FEED_UP:
    case PROG_FEED_DOWN:
        if (mc->longitudal_distance)
            percent = (abs(mc->longitudal_pos - mc->start_longitudal_pos) * 100) /
                            mc->longitudal_distance;
        else
            percent = 0;
        break;
    }
    snprintf(str, size, "%d%%", percent);
}

static void ui_cross_miss_up_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->stat.cross_miss_up / 1000);
}

static void ui_cross_miss_down_getter(struct ui_item *ut,
                                      char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->stat.cross_miss_down / 1000);
}

static void ui_longitudal_miss_left_getter(struct ui_item *ut,
                                           char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->stat.longitudal_miss_left / 1000);
}

static void ui_longitudal_miss_right_getter(struct ui_item *ut,
                                            char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->stat.longitudal_miss_right / 1000);
}

static void ui_text(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct img *img;
    struct text_style *ts = &normal_text_style;
    int text_height = disp_text_height(ts);
    int row_height = text_height + 2;
    const int row_start = 90;

    // draw text lines
    disp_text(disp, msg_uptime, 0, row_start + row_height * 0, ts);
    disp_text(disp, msg_feed_number, 0, row_start + row_height * 1, ts);
    disp_text(disp, msg_cut_speed, 0, row_start + row_height * 2, ts);
    disp_text(disp, msg_feed_speed, 0, row_start + row_height * 3, ts);

    disp_text(disp, msg_coordinates, 0, row_start + row_height * 4, ts);
    disp_text(disp, msg_start, 132, row_start + row_height * 4, ts);
    disp_text(disp, msg_end, 240, row_start + row_height * 4, ts);
    disp_text(disp, msg_readiness, 350, row_start + row_height * 4, ts);

    disp_text(disp, msg_cross_position, 0, row_start + row_height * 5, ts);
    disp_text(disp, msg_longitudal_position, 0, row_start + row_height * 6, ts);

    // draw 'miss' table
    disp_rect(disp, 90, row_start + row_height * 7,  90, 37, 1, GRAY);
    disp_rect(disp, 90 + 90, row_start + row_height * 7,  90, 37, 1, GRAY);
    disp_rect(disp, 90 + 90 + 90, row_start + row_height * 7,
              100, 37, 1, GRAY);
    disp_rect(disp, 90 + 90 + 90 + 100, row_start + row_height * 7,
              100, 37, 1, GRAY);

    img = img_cross_arrow_up1();
    disp_fill_img(disp, 95, row_start + row_height * 7 + 2, img);
    kmem_deref(&img);

    img = img_cross_arrow_down1();
    disp_fill_img(disp, 90 + 95, row_start + row_height * 7 + 2, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_left1();
    disp_fill_img(disp, 90 + 90 + 95, row_start + row_height * 7 + 6, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_right1();
    disp_fill_img(disp, 90 + 90 + 90 + 105,
                  row_start + row_height * 7 + 6, img);
    kmem_deref(&img);
}

static void display_status_init(struct mode_cut *mc)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct text_style *ts = &normal_text_style;
    int text_height = disp_text_height(ts);
    struct mode_cut_ui_items *ui_items = mc->ui_items;
    int row_height = text_height + 2;
    const int row_start = 90;

    ui_items->text =
            ui_item_register("ui_text", disp,
                                 0, 90, 480, 230,
                                 ui_text, NULL, mc, 0);

    ui_items->uptime =
            ui_item_text_register("ui_uptime", disp,
                                 220, row_start + row_height * 0, 9, ts,
                                 ui_uptime_getter, NULL, mc);

    ui_items->number_passes =
            ui_item_text_register("ui_number_passes", disp,
                                 220, row_start + row_height * 1,
                                 5, ts, ui_number_passes_getter, NULL, mc);

    ui_items->cutting_speed =
            ui_item_text_register("ui_cutting_speed", disp,
                                 320, row_start + row_height * 2, 6, ts,
                                 ui_cutting_speed_getter, NULL, mc);

    ui_items->feed_speed =
            ui_item_text_register("ui_feed_speed", disp,
                                 305, row_start + row_height * 3, 6, ts,
                                 ui_feed_speed_getter, NULL, mc);

    ui_items->start_cross =
            ui_item_text_register("ui_start_point", disp,
                                 132, row_start + row_height * 5, 8,
                                 &cross_text_style,
                                 ui_start_cross_getter, NULL, mc);

    ui_items->finish_cross =
            ui_item_text_register("ui_finish_cross", disp,
                                 240, row_start + row_height * 5, 8,
                                 &cross_text_style,
                                 ui_finish_cross_getter, NULL, mc);

    ui_items->relative_cross =
            ui_item_text_register("ui_relative_cross", disp,
                                 360, row_start + row_height * 5, 8,
                                 &cross_text_style,
                                 ui_relative_cross_getter, NULL, mc);

    ui_items->start_longitudal =
            ui_item_text_register("ui_start_longitudal", disp,
                                 132, row_start + row_height * 6, 8,
                                 &longitudal_text_style,
                                 ui_start_longitudal_getter, NULL, mc);

    ui_items->finish_longitudal =
            ui_item_text_register("ui_finish_longitudal", disp,
                                 240, row_start + row_height * 6, 8,
                                 &longitudal_text_style,
                                 ui_finish_longitudal_getter, NULL, mc);

    ui_items->relative_longitudal =
            ui_item_text_register("ui_relative_longitudal", disp,
                                 360, row_start + row_height * 6, 8,
                                 &longitudal_text_style,
                                 ui_relative_longitudal_getter, NULL, mc);

    ui_items->cross_miss_up =
            ui_item_text_register("ui_cross_miss_up", disp,
                                 117, row_start + row_height * 7 + 10, 5, ts,
                                 ui_cross_miss_up_getter, NULL, mc);

    ui_items->cross_miss_down =
            ui_item_text_register("ui_cross_miss_down", disp,
                                 203, row_start + row_height * 7 + 10, 5, ts,
                                 ui_cross_miss_down_getter, NULL, mc);

    ui_items->longitudal_miss_left =
            ui_item_text_register("ui_longitudal_miss_left", disp,
                                 304, row_start + row_height * 7 + 10, 5, ts,
                                 ui_longitudal_miss_left_getter, NULL, mc);

    ui_items->longitudal_miss_right =
            ui_item_text_register("ui_longitudal_miss_right", disp,
                                 405, row_start + row_height * 7 + 10, 5, ts,
                                 ui_longitudal_miss_right_getter, NULL, mc);
}

static void display_status_handler(struct mode_cut *mc)
{
    struct machine *m = &machine;
    struct mode_cut_ui_items *ui_items = mc->ui_items;
    if (m->is_disp2_needs_redraw) {
        m->is_disp2_needs_redraw = FALSE;
        ui_item_show(ui_items->text);
        ui_item_update(ui_items->number_passes);
        ui_item_update(ui_items->cutting_speed);
        ui_item_update(ui_items->feed_speed);
        ui_item_update(ui_items->start_cross);
        ui_item_update(ui_items->finish_cross);
        ui_item_update(ui_items->relative_cross);
        ui_item_update(ui_items->start_longitudal);
        ui_item_update(ui_items->finish_longitudal);
        ui_item_update(ui_items->relative_longitudal);
        ui_item_update(ui_items->cross_miss_up);
        ui_item_update(ui_items->cross_miss_down);
        ui_item_update(ui_items->longitudal_miss_left);
        ui_item_update(ui_items->longitudal_miss_right);
        return;
    }

    ui_item_show(ui_items->uptime);
    ui_item_show(ui_items->number_passes);
    ui_item_show(ui_items->cutting_speed);
    ui_item_show(ui_items->feed_speed);
    ui_item_show(ui_items->start_cross);
    ui_item_show(ui_items->finish_cross);
    ui_item_show(ui_items->relative_cross);
    ui_item_show(ui_items->start_longitudal);
    ui_item_show(ui_items->finish_longitudal);
    ui_item_show(ui_items->relative_longitudal);
    ui_item_show(ui_items->cross_miss_up);
    ui_item_show(ui_items->cross_miss_down);
    ui_item_show(ui_items->longitudal_miss_left);
    ui_item_show(ui_items->longitudal_miss_right);
}

static void display_finished_show(struct mode_cut *mc)
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


static void display_status_hide(void)
{
    ui_message_hide();
}

static int cross_distance(struct mode_cut *mc)
{
    struct mode_cut_settings *mc_settings = &mc->settings;
    return mc_settings->cross_distance ?
                     mc_settings->cross_distance :
                     abs(mc->start_cross_pos -
                             (mc_settings->target_diameter / 2));
}

static void calc_estimate_cut_longitudal_time(struct mode_cut *mc)
{
    struct mode_cut_settings *mc_settings = &mc->settings;
    int one_pass = mc->longitudal_distance * mc_settings->feed_rate / 1000000 +
            7 + // cross return time sec
            + mc->cross_distance / 1000 / 8; // longitudal return time sec

    mc->stat.calc_time = one_pass * (mc->cut_pass_rest + 1) -
            (now() - mc->stat.start_time) / 1000;
}

static void calc_estimate_cut_cross_time(struct mode_cut *mc)
{
    struct mode_cut_settings *mc_settings = &mc->settings;
    int one_pass = mc->cross_distance * mc_settings->feed_rate / 1000000 +
            7 + // longitudal return time sec
            + mc->cross_distance / 1000 / 8; // cross return time sec
    mc->stat.calc_time = one_pass * (mc->cut_pass_rest + 1) -
            (now() - mc->stat.start_time) / 1000;
}

static void calc_longitudal_cut_passes(struct mode_cut *mc)
{
    int cross_distance_val = 0;
    bool dir;
    struct mode_cut_settings *mc_settings = &mc->settings;
    mc->cut_pass_rest =
            (mc_settings->last_repeate_number - mc->cut_pass_last_cnt);

    if (mc->cut_pass_rest < 0)
        mc->cut_pass_rest = 0;

    mc->end_longitudal_pos =
                calc_longitudal_position(mc->start_longitudal_pos,
                                         mc_settings->longitudal_distance,
                                         mc->longitudal_pass_dir);

    if (mc->prog != PROG_FEED_LEFT &&
            mc->prog != PROG_FEED_RIGHT) {
        cross_distance_val = cross_distance(mc);
    }

    mc->end_cross_pos =
            calc_cross_position(mc->start_cross_pos,
                                cross_distance_val,
                                mc->cross_pass_dir);

    cross_distance_val = calc_cross_to_target(mc->cross_pos,
                                          mc->end_cross_pos, &dir);
    mc->cross_distance = abs(mc->end_cross_pos - mc->start_cross_pos);
    calc_estimate_cut_longitudal_time(mc);

    if (cross_distance_val == 0) {
        mc->cut_depth = 0;
        return;
    }

    if (dir != mc->cross_pass_dir) {
        mc->cut_depth = 0;
        return;
    }

    mc->cut_depth = (cross_distance_val > (mc_settings->cut_depth / 2)) ?
                     mc_settings->cut_depth / 2 : cross_distance_val;

    mc->cut_pass_rest += cross_distance_val / mc->cut_depth - 1;
    calc_estimate_cut_longitudal_time(mc);
}

static void calc_cross_cut_passes(struct mode_cut *mc)
{
    int longitudal_distance = 0;
    bool dir;
    struct mode_cut_settings *mc_settings = &mc->settings;
    mc->cut_pass_rest = 0;

    mc->end_cross_pos =
                calc_cross_position(mc->start_cross_pos, cross_distance(mc),
                                    mc->cross_pass_dir);

    if (mc->prog != PROG_FEED_UP &&
            mc->prog != PROG_FEED_DOWN) {
        longitudal_distance = mc_settings->longitudal_distance;
    }

    mc->end_longitudal_pos =
            calc_longitudal_position(mc->start_longitudal_pos,
                                     longitudal_distance,
                                     mc->longitudal_pass_dir);

    longitudal_distance =
            calc_longitudal_to_target(mc->longitudal_pos,
                                      mc->end_longitudal_pos, &dir);
    mc->longitudal_distance =
            abs(mc->end_longitudal_pos - mc->start_longitudal_pos);

    calc_estimate_cut_cross_time(mc);

    if (longitudal_distance == 0) {
        mc->cut_depth = 0;
        return;
    }

    if (dir != mc->longitudal_pass_dir) {
        mc->cut_depth = 0;
        return;
    }

    mc->cut_depth = (longitudal_distance > mc_settings->cut_depth) ?
                     mc_settings->cut_depth : longitudal_distance;
    mc->cut_pass_rest += longitudal_distance / mc->cut_depth - 1;
    calc_estimate_cut_cross_time(mc);
}

static void update_longitudal_miss(struct mode_cut *mc)
{
    struct machine *m = &machine;
    int pos = abs_longitudal_curr_tool(m->ap);
    int miss = abs(pos - mc->end_longitudal_pos);
    if (is_longitudal_target_position_left(m->ap, pos, mc->end_longitudal_pos)) {
        if (miss > mc->stat.longitudal_miss_right)
            mc->stat.longitudal_miss_right = miss;
    } else {
        if (miss > mc->stat.longitudal_miss_left)
            mc->stat.longitudal_miss_left = miss;
    }
}

static void update_cross_miss(struct mode_cut *mc)
{
    struct machine *m = &machine;
    int pos = abs_cross_curr_tool(m->ap);
    int miss = abs(pos - mc->end_cross_pos);
    if (is_cross_target_position_down(m->ap, pos, mc->end_cross_pos)) {
        if (miss > mc->stat.cross_miss_down)
            mc->stat.cross_miss_down = miss;
    } else {
        if (miss > mc->stat.cross_miss_up)
            mc->stat.cross_miss_up = miss;
    }
}

void mode_cut_settings_validate(void)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    if (!mc_settings->cross_distance) {
        int curr_diameter = abs_cross_curr_tool(m->ap) * 2;
        switch (mc->prog) {
        case PROG_FEED_LEFT_UP:
        case PROG_FEED_RIGHT_UP:
        case PROG_FEED_UP_LEFT:
        case PROG_FEED_UP_RIGHT:
        case PROG_FEED_UP:
            if (m->ap->is_cross_inc_down) {
                if (mc_settings->target_diameter > curr_diameter)
                    mc_settings->target_diameter = curr_diameter;
            }  else {
                if (mc_settings->target_diameter < curr_diameter)
                    mc_settings->target_diameter = curr_diameter;
            }
            break;

        case PROG_FEED_DOWN:
        case PROG_FEED_LEFT_DOWN:
        case PROG_FEED_RIGHT_DOWN:
        case PROG_FEED_DOWN_LEFT:
        case PROG_FEED_DOWN_RIGHT:
            if (m->ap->is_cross_inc_down) {
                if (mc_settings->target_diameter < curr_diameter)
                    mc_settings->target_diameter = curr_diameter;
            }  else {
                if (mc_settings->target_diameter > curr_diameter)
                    mc_settings->target_diameter = curr_diameter;
            }
            break;

        default:
            break;
        }
    }
}


static void
stabilization_cross_handler(struct mode_cut *mc)
{
    struct machine *m = &machine;
    int target_pos = mc->cross_pos;
    struct stepper_motor *sm = m->sm_cross_feed;
    int cur_pos;
    int distance;
    static u32 stop_time = 0;
    bool dir;

    cur_pos = abs_cross_curr_tool(m->ap);
    if (cur_pos == target_pos) {
        stepper_motor_stop(sm);
        return;
    }

    if (is_stepper_motor_run(sm))
        return;

    if ((now() - stop_time) < 200)
        return;

    if (cur_pos == target_pos)
        return;

    distance = calc_cross_to_target(cur_pos, target_pos, &dir);
    stepper_motor_run(sm, 0, 200, dir, distance);
    if (dir == MOVE_UP)
        mc->stat.cross_miss_up = distance;
    else
        mc->stat.cross_miss_down = distance;
    stop_time = now();
    return;
}

int longitudal_cut_run(struct mode_cut *mc, bool dir)
{
    struct machine *m = &machine;
    struct mode_cut_settings *mc_settings = &mc->settings;
    struct stepper_motor *sm = m->sm_longitudial_feed;
    int prev_feed_rate = speed_to_freq(sm, mc_settings->feed_rate);
    int cur_pos = abs_longitudal_curr_tool(m->ap);

    mc->cut_pass_cnt ++;
    mc->longitudal_distance =
            abs(mc->end_longitudal_pos - cur_pos);
    stepper_motor_run(sm, 1000, prev_feed_rate, dir,
                      mc->longitudal_distance);
    while(is_stepper_motor_run(sm)) {
        yield();
        if (mc_settings->feed_rate != prev_feed_rate) {
            int freq = speed_to_freq(sm, mc_settings->feed_rate);
            prev_feed_rate = mc_settings->feed_rate;
            stepper_motor_set_target_freq(sm, freq);
            stepper_motor_set_freq(sm, freq);
        }

        if (!is_switch_on(m->switch_run)) {
            stepper_motor_stop(sm);
            return -1;
        }
        stabilization_cross_handler(mc);
        calc_longitudal_cut_passes(mc);
        display_status_handler(mc);
    }
    sleep(50);
    update_longitudal_miss(mc);
    return 0;
}

int cross_cut_run(struct mode_cut *mc, bool dir)
{
    struct machine *m = &machine;
    struct mode_cut_settings *mc_settings = &mc->settings;
    struct stepper_motor *sm = m->sm_cross_feed;
    int prev_feed_rate = speed_to_freq(sm, mc_settings->feed_rate);

    mc->cut_pass_cnt ++;
    mc->cross_distance = cross_distance(mc);
    stepper_motor_run(sm, 50, prev_feed_rate, dir, mc->cross_distance);
    while(is_stepper_motor_run(sm)) {
        yield();
        if (mc_settings->feed_rate != prev_feed_rate) {
            int freq = speed_to_freq(sm, mc_settings->feed_rate);
            prev_feed_rate = mc_settings->feed_rate;
            stepper_motor_set_target_freq(sm, freq);
            stepper_motor_set_freq(sm, freq);
        }

        if (!is_switch_on(m->switch_run)) {
            stepper_motor_stop(sm);
            return -1;
        }
        calc_cross_cut_passes(mc);
        display_status_handler(mc);
    }
    sleep(50);
    update_cross_miss(mc);
    update_longitudal_miss(mc);
    return 0;
}


int longitudal_return_run(struct mode_cut *mc)
{
    struct machine *m = &machine;
    struct mode_cut_settings *cm_settings = &mc->settings;
    int start_longitudal_pos, longitudal_pos;
    int parking_cross_pos;
    int rc;


    start_longitudal_pos = longitudal_pos = abs_longitudal_curr_tool(m->ap);
    switch(mc->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_RIGHT: {
        switch(cm_settings->longitudal_ret_mode) {
        case CUT_LONGITUDAL_RETURN_UP:
            start_longitudal_pos = mc->start_longitudal_pos;
        case CUT_LONGITUDAL_MOVE_UP:
            parking_cross_pos =
                    calc_cross_position(mc->cross_pos,
                                        CROSS_RETRACTION, MOVE_UP);
            break;

        case CUT_LONGITUDAL_RETURN_DOWN:
            start_longitudal_pos = mc->start_longitudal_pos;
        case CUT_LONGITUDAL_MOVE_DOWN:
            parking_cross_pos =
                    calc_cross_position(mc->cross_pos,
                                        CROSS_RETRACTION, MOVE_DOWN);
            break;

        case CUT_LONGITUDAL_NO_RETURN:
            parking_cross_pos = mc->cross_pos;
        default:
            break;
        }
        break;
    }

    case PROG_FEED_LEFT_UP:
        start_longitudal_pos = mc->start_longitudal_pos;
    case PROG_FEED_RIGHT_UP:
        parking_cross_pos =
                calc_cross_position(mc->cross_pos,
                                    CROSS_RETRACTION, MOVE_DOWN);
        break;

    case PROG_FEED_LEFT_DOWN:
        start_longitudal_pos = mc->start_longitudal_pos;
    case PROG_FEED_RIGHT_DOWN:
        parking_cross_pos =
                calc_cross_position(mc->cross_pos,
                                    CROSS_RETRACTION, MOVE_UP);
        break;
    default:
        panic("longitudal_return() incrorrect program");
    }

    if (parking_cross_pos != mc->cross_pos) {
        rc = cross_move_to(parking_cross_pos, TRUE);
        if(rc)
            return rc;
    }

    if (longitudal_pos != start_longitudal_pos) {
        rc = longitudal_move_to(start_longitudal_pos, FALSE);
        if(rc)
            return rc;

        if (parking_cross_pos != mc->cross_pos) {
            rc = cross_move_to(mc->cross_pos, TRUE);
            if(rc)
                return rc;
        }
    }
    return 0;
}

static int cross_return_run(struct mode_cut *mc)
{
    struct machine *m = &machine;
    struct mode_cut_settings *cm_settings = &mc->settings;
    int start_cross_pos, cross_pos;
    int parking_longitudal_pos;
    int rc;

    start_cross_pos = cross_pos = abs_cross_curr_tool(m->ap);
    switch(mc->prog) {
    case PROG_FEED_UP:
    case PROG_FEED_DOWN: {
        switch(cm_settings->cross_ret_mode) {
        case CUT_CROSS_RETURN_LEFT:
            start_cross_pos = mc->start_cross_pos;
        case CUT_CROSS_MOVE_LEFT:
            parking_longitudal_pos =
                calc_longitudal_position(mc->longitudal_pos,
                                         LONGITUDAL_RETRACTION, MOVE_LEFT);
            break;

        case CUT_CROSS_RETURN_RIGHT:
            start_cross_pos = mc->start_cross_pos;
        case CUT_CROSS_MOVE_RIGHT:
            parking_longitudal_pos =
                calc_longitudal_position(mc->longitudal_pos,
                                         LONGITUDAL_RETRACTION, MOVE_RIGHT);
            break;

        case CUT_CROSS_NO_RETURN:
            parking_longitudal_pos = mc->longitudal_pos;
        default:
            break;
        }
        break;
    }

    case PROG_FEED_UP_RIGHT:
        start_cross_pos = mc->start_cross_pos;
    case PROG_FEED_DOWN_RIGHT:
        parking_longitudal_pos =
                calc_longitudal_position(mc->longitudal_pos,
                                         LONGITUDAL_RETRACTION, MOVE_LEFT);
        break;

    case PROG_FEED_UP_LEFT:
        start_cross_pos = mc->start_cross_pos;
    case PROG_FEED_DOWN_LEFT:
        parking_longitudal_pos =
                calc_longitudal_position(mc->longitudal_pos,
                                         LONGITUDAL_RETRACTION, MOVE_RIGHT);
        break;
    default:
        panic("longitudal_return() incrorrect program");
    }

    if (parking_longitudal_pos != mc->longitudal_pos) {
        rc = longitudal_move_to(parking_longitudal_pos, FALSE);
        if(rc)
            return rc;
    }

    if (cross_pos != start_cross_pos) {
        rc = cross_move_to(start_cross_pos, TRUE);
        if(rc)
            return rc;
        if (parking_longitudal_pos != mc->longitudal_pos) {
            rc = longitudal_move_to(mc->longitudal_pos, FALSE);
            if(rc)
                return rc;
        }
    }
    return 0;
}


void longitudal_cut_auto_run(void)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;
    mc->start_cross_pos = abs_cross_curr_tool(m->ap);
    mc->start_longitudal_pos = abs_longitudal_curr_tool(m->ap);
    int i, rc;

    if (mc->prog == PROG_FEED_UP ||
            mc->prog == PROG_FEED_UP_LEFT ||
            mc->prog == PROG_FEED_UP_RIGHT ||
            mc->prog == PROG_FEED_DOWN ||
            mc->prog == PROG_FEED_DOWN_LEFT ||
            mc->prog == PROG_FEED_DOWN_RIGHT ||
            mc->prog == PROG_THREAD_LEFT ||
            mc->prog == PROG_THREAD_RIGHT) {
        panic("longitudal_cut_auto_run() incorrect program");
    }

    mc->longitudal_pass_dir = MOVE_RIGHT;
    if (mc->prog == PROG_FEED_LEFT ||
            mc->prog == PROG_FEED_LEFT_UP ||
            mc->prog == PROG_FEED_LEFT_DOWN) {
        mc->longitudal_pass_dir = MOVE_LEFT;
    }

    mc->cross_pass_dir = MOVE_DOWN;
    if (mc->prog == PROG_FEED_LEFT_UP ||
            mc->prog == PROG_FEED_RIGHT_UP) {
        mc->cross_pass_dir = MOVE_UP;
    }

    stepper_motor_enable(m->sm_cross_feed);
    stepper_motor_enable(m->sm_longitudial_feed);
    mc->cross_pos = mc->start_cross_pos;
    mc->cut_pass_last_cnt = 0;

    calc_longitudal_cut_passes(mc);
    display_status_handler(mc);

    if (mc->prog == PROG_FEED_LEFT ||
            mc->prog == PROG_FEED_RIGHT) {
        longitudal_cut_run(mc, mc->longitudal_pass_dir);
        rc = longitudal_return_run(mc);
        if (rc)
            return;
        calc_longitudal_cut_passes(mc);
        display_status_handler(mc);
    }

    while(mc->cut_depth) {
        int new_pos;
        yield();
        new_pos = calc_cross_position(mc->cross_pos,
                                      mc->cut_depth,
                                      mc->cross_pass_dir);
        rc = cross_move_to(new_pos, TRUE);
        if(rc)
            return;
        mc->cross_pos = new_pos;

        // remove previous miss. Only last miss needed.
        mc->stat.cross_miss_up = 0;
        mc->stat.cross_miss_down = 0;

        longitudal_cut_run(mc, mc->longitudal_pass_dir);
        rc = longitudal_return_run(mc); // TODO: cross_pos fix
        if (rc)
            return;
        calc_longitudal_cut_passes(mc);
        display_status_handler(mc);
    }

    for (i = 0; i < mc_settings->last_repeate_number; i++) {
        mc->cut_pass_last_cnt++;
        calc_longitudal_cut_passes(mc);
        display_status_handler(mc);
        rc = longitudal_cut_run(mc, mc->longitudal_pass_dir);
        if (rc)
            return;
        rc = longitudal_return_run(mc);
        if (rc)
            return;
    }
    if (mc->prog != PROG_FEED_LEFT &&
            mc->prog != PROG_FEED_RIGHT) {
        longitudal_move_to(mc->start_longitudal_pos, TRUE);
    }
}


void cross_cut_auto_run(void)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    mc->start_cross_pos = abs_cross_curr_tool(m->ap);
    mc->start_longitudal_pos = abs_longitudal_curr_tool(m->ap);
    mc->longitudal_pos = mc->start_longitudal_pos;
    int rc;

    if (mc->prog == PROG_FEED_LEFT ||
            mc->prog == PROG_FEED_LEFT_UP ||
            mc->prog == PROG_FEED_RIGHT_UP ||
            mc->prog == PROG_FEED_RIGHT ||
            mc->prog == PROG_FEED_LEFT_DOWN ||
            mc->prog == PROG_FEED_RIGHT_DOWN ||
            mc->prog == PROG_THREAD_LEFT ||
            mc->prog == PROG_THREAD_RIGHT) {
        panic("cross_cut_auto_run() incorrect program");
    }

    mc->longitudal_pass_dir = MOVE_RIGHT;
    if (mc->prog == PROG_FEED_UP_LEFT ||
            mc->prog == PROG_FEED_DOWN_LEFT) {
        mc->longitudal_pass_dir = MOVE_LEFT;
    }

    mc->cross_pass_dir = MOVE_DOWN;
    if (mc->prog == PROG_FEED_UP ||
            mc->prog == PROG_FEED_UP_LEFT ||
            mc->prog == PROG_FEED_UP_RIGHT) {
        mc->cross_pass_dir = MOVE_UP;
    }

    stepper_motor_enable(m->sm_cross_feed);
    stepper_motor_enable(m->sm_longitudial_feed);
    mc->cross_pos = mc->start_cross_pos;
    mc->cut_pass_last_cnt = 0;

    calc_cross_cut_passes(mc);
    display_status_handler(mc);
    if (mc->prog == PROG_FEED_UP ||
            mc->prog == PROG_FEED_DOWN) {
        cross_cut_run(mc, mc->cross_pass_dir);
        rc = cross_return_run(mc);
        if (rc)
            return;
        calc_cross_cut_passes(mc);
        display_status_handler(mc);
    }

    while(mc->cut_depth) {
        int new_pos;
        yield();
        new_pos = calc_longitudal_position(mc->longitudal_pos,
                                           mc->cut_depth,
                                           mc->longitudal_pass_dir);
        rc = longitudal_move_to(new_pos, TRUE);
        if(rc)
            return;
        mc->longitudal_pos = new_pos;

        cross_cut_run(mc, mc->cross_pass_dir);
        calc_cross_cut_passes(mc);
        rc = cross_return_run(mc);
        if (rc)
            return;
        display_status_handler(mc);
    }
    if (mc->prog != PROG_FEED_UP &&


            mc->prog != PROG_FEED_DOWN) {
        longitudal_move_to(mc->start_longitudal_pos, TRUE);
    }
}


static void ui_items_destructor(void *mem)
{
    struct mode_cut_ui_items *ui_items = (struct mode_cut_ui_items *)mem;
    kmem_deref(&ui_items->text);
    kmem_deref(&ui_items->uptime);
    kmem_deref(&ui_items->number_passes);
    kmem_deref(&ui_items->cutting_speed);
    kmem_deref(&ui_items->feed_speed);
    kmem_deref(&ui_items->start_cross);
    kmem_deref(&ui_items->start_longitudal);
    kmem_deref(&ui_items->relative_cross);
    kmem_deref(&ui_items->finish_cross);
    kmem_deref(&ui_items->finish_longitudal);
    kmem_deref(&ui_items->relative_longitudal);
    kmem_deref(&ui_items->cross_miss_up);
    kmem_deref(&ui_items->cross_miss_down);
    kmem_deref(&ui_items->longitudal_miss_left);
    kmem_deref(&ui_items->longitudal_miss_right);
}


void mode_cut_run(void)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    struct mode_cut_ui_items *ui_items;

    ui_items = kzref_alloc("mode_cut_ui_items", sizeof *ui_items,
                           ui_items_destructor);
    mc->ui_items = ui_items;
    mc->stat.start_time = now();
    mc->stat.longitudal_miss_left = 0;
    mc->stat.longitudal_miss_right = 0;
    mc->stat.cross_miss_up = 0;
    mc->stat.cross_miss_down = 0;
    mc->cut_pass_cnt = 0;

    display_status_init(mc);

    set_normal_acceleration();

    switch (mc->prog) {
    case PROG_FEED_LEFT:
    case PROG_FEED_RIGHT:
    case PROG_FEED_LEFT_UP:
    case PROG_FEED_RIGHT_UP:
    case PROG_FEED_LEFT_DOWN:
    case PROG_FEED_RIGHT_DOWN:
        longitudal_cut_auto_run();
        break;

    case PROG_FEED_UP:
    case PROG_FEED_DOWN:
    case PROG_FEED_UP_LEFT:
    case PROG_FEED_UP_RIGHT:
    case PROG_FEED_DOWN_LEFT:
    case PROG_FEED_DOWN_RIGHT:
        cross_cut_auto_run();
        break;
    default:
        break;
    }

    display_finished_show(mc);
    beep_blink_start(500, 1000, 0);
    while(is_switch_on(m->switch_run)) {
        yield();
    }
    beep_blink_stop();
    display_status_hide();
    kmem_deref(&ui_items);
}
