/*
 * mode_cut.c
 *
 *  Created on: Jun 26, 2025
 *      Author: stelhs
 */

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

static struct mode_cut mode_cut;

struct text_style stat_text_style = {
        .bg_color = BLACK,
        .color = GRAY,
        .font = font_rus,
        .fontsize = 2,
};

static void ui_uptime_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    u32 uptime = (now() - mc->start_time) / 1000;
    snprintf(str, size, "%lu/%d", uptime, mc->calc_time);
}

static void ui_number_passes_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    struct ui_main *um = ui_main;
    snprintf(str, size, "%d/%d", mc->pass_number, um->pass_repeate_cnt + 1);
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

static void ui_cut_depth_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->cut_depth / 1000);
}

static void ui_cross_miss_up_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->cross_miss_up / 1000);
}

static void ui_cross_miss_down_getter(struct ui_item *ut,
                                      char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->cross_miss_down / 1000);
}

static void ui_longitudal_miss_left_getter(struct ui_item *ut,
                                           char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->longitudal_miss_left / 1000);
}

static void ui_longitudal_miss_right_getter(struct ui_item *ut,
                                            char *str, size_t size)
{
    struct mode_cut *mc = (struct mode_cut *)ut->priv;
    snprintf(str, size, "%.3f", (float)mc->longitudal_miss_right / 1000);
}


static void display_status_show(struct mode_cut *mc)
{
    struct mode_cut_ui_items *ui_items = mc->ui_items;
    struct text_style *ts = &stat_text_style;
    struct img *img;
    int text_height = disp_text_height(&stat_text_style);
    int row_height = text_height + 2;
    const int row_start = 90;

    // draw text lines
    disp_text(mc->disp, msg_uptime, 0, row_start + row_height * 0, ts);
    disp_text(mc->disp, msg_feed_number, 0, row_start + row_height * 1, ts);
    disp_text(mc->disp, msg_cut_speed, 0, row_start + row_height * 2, ts);
    disp_text(mc->disp, msg_feed_speed, 0, row_start + row_height * 3, ts);
    disp_text(mc->disp, msg_cut_depth, 0, row_start + row_height * 4, ts);
    disp_text(mc->disp, msg_cuting_miss, 0,
              row_start + row_height * 5 + 10, ts);

    // draw 'miss' table
    disp_rect(mc->disp, 90, row_start + row_height * 5,  90, 37, 1, GRAY);
    disp_rect(mc->disp, 90 + 90, row_start + row_height * 5,  90, 37, 1, GRAY);
    disp_rect(mc->disp, 90 + 90 + 90, row_start + row_height * 5,
              100, 37, 1, GRAY);
    disp_rect(mc->disp, 90 + 90 + 90 + 100, row_start + row_height * 5,
              100, 37, 1, GRAY);

    img = img_cross_arrow_up1();
    disp_fill_img(mc->disp, 95, row_start + row_height * 5 + 2, img);
    kmem_deref(&img);

    img = img_cross_arrow_down1();
    disp_fill_img(mc->disp, 90 + 95, row_start + row_height * 5 + 2, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_left1();
    disp_fill_img(mc->disp, 90 + 90 + 95, row_start + row_height * 5 + 6, img);
    kmem_deref(&img);

    img = img_longitudal_arrow_right1();
    disp_fill_img(mc->disp, 90 + 90 + 90 + 105,
                  row_start + row_height * 5 + 6, img);
    kmem_deref(&img);


    ui_items->uptime =
            ui_item_text_register("ui_uptime", mc->disp,
                                 220, row_start + row_height * 0, 9, ts,
                                 ui_uptime_getter, NULL, mc);

    ui_items->number_passes =
            ui_item_text_register("ui_number_passes", mc->disp,
                                 220, row_start + row_height * 1,
                                 5, ts, ui_number_passes_getter, NULL, mc);

    ui_items->cutting_speed =
            ui_item_text_register("ui_cutting_speed", mc->disp,
                                 320, row_start + row_height * 2, 6, ts,
                                 ui_cutting_speed_getter, NULL, mc);

    ui_items->feed_speed =
            ui_item_text_register("ui_feed_speed", mc->disp,
                                 305, row_start + row_height * 3, 6, ts,
                                 ui_feed_speed_getter, NULL, mc);

    ui_items->cut_depth =
            ui_item_text_register("ui_cut_depth", mc->disp,
                                 270, row_start + row_height * 4, 5, ts,
                                 ui_cut_depth_getter, NULL, mc);

    ui_items->cross_miss_up =
            ui_item_text_register("ui_cross_miss_up", mc->disp,
                                 117, row_start + row_height * 5 + 10, 5, ts,
                                 ui_cross_miss_up_getter, NULL, mc);

    ui_items->cross_miss_down =
            ui_item_text_register("ui_cross_miss_down", mc->disp,
                                 203, row_start + row_height * 5 + 10, 5, ts,
                                 ui_cross_miss_down_getter, NULL, mc);

    ui_items->longitudal_miss_left =
            ui_item_text_register("ui_longitudal_miss_left", mc->disp,
                                 304, row_start + row_height * 5 + 10, 5, ts,
                                 ui_longitudal_miss_left_getter, NULL, mc);

    ui_items->longitudal_miss_right =
            ui_item_text_register("ui_longitudal_miss_right", mc->disp,
                                 405, row_start + row_height * 5 + 10, 5, ts,
                                 ui_longitudal_miss_right_getter, NULL, mc);
}

static void display_status_handler(struct mode_cut *mc)
{
    struct mode_cut_ui_items *ui_items = mc->ui_items;
    ui_item_show(ui_items->uptime);
    ui_item_show(ui_items->number_passes);
    ui_item_show(ui_items->cutting_speed);
    ui_item_show(ui_items->feed_speed);
    ui_item_show(ui_items->cut_depth);
    ui_item_show(ui_items->cross_miss_up);
    ui_item_show(ui_items->cross_miss_down);
    ui_item_show(ui_items->longitudal_miss_left);
    ui_item_show(ui_items->longitudal_miss_right);
}

static void display_finished_show(struct mode_cut *mc)
{
    struct text_style ts = {
            .bg_color = BLACK,
            .color = GREEN,
            .font = font_rus,
            .fontsize = 3,
    };
    disp_text(mc->disp, msg_prog_finished, 184, 270, &ts);
}


static void display_status_hide(void)
{
    ui_message_hide();
}


static void
stabilization_cross_handler(int target_pos)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_cross_feed;
    int curr_cross;
    int distance;
    static u32 stop_time = 0;
    bool dir;

    if (is_stepper_motor_run(sm))
        return;

    if ((now() - stop_time) < 200)
        return;

    curr_cross = abs_cross_curr_tool(m->ap);
    if (curr_cross == target_pos)
        return;

    distance = calc_cross_to_target(target_pos, &dir);
    stepper_motor_run(sm, 300, 0, dir, distance);
    stop_time = now();
    return;
}


void run_feed_left_right_only(bool dir, int distance)
{
    struct machine *m = &machine;
    struct ui_main *um = ui_main;
    struct mode_cut *mc = &mode_cut;
    int start_cross = abs_cross_curr_tool(m->ap);
    int start_longitudal = abs_longitudal_curr_tool(m->ap);
    struct stepper_motor *sm = m->sm_longitudial_feed;
    int next_cross_pos, next_longitudal_pos, longitudal_pos;
    int i;

    stepper_motor_enable(m->sm_cross_feed);
    stepper_motor_enable(m->sm_longitudial_feed);

    for (i = 0; i < um->pass_repeate_cnt + 1; i++) {
        int prev_feed_rate = speed_to_freq(sm, m->feed_rate);
        mc->pass_number = i + 1;
        stepper_motor_run(sm, 1000, prev_feed_rate, dir, distance);
        while(is_stepper_motor_run(sm)) {
            yield();
            if (m->feed_rate != prev_feed_rate) {
                int freq = speed_to_freq(sm, m->feed_rate);
                prev_feed_rate = m->feed_rate;
                stepper_motor_set_freq(sm, freq);
                stepper_motor_set_target_freq(sm, freq);
            }

            if (!is_switch_on(m->switch_run)) {
                stepper_motor_stop(sm);
                return;
            }
            stabilization_cross_handler(start_cross);
            display_status_handler(mc);
        }

        next_longitudal_pos = longitudal_pos = abs_longitudal_curr_tool(m->ap);

        switch(um->crm) {
        case CUT_MODE_RETURN_UP:
            next_longitudal_pos = start_longitudal;
        case CUT_MODE_MOVE_UP:
            next_cross_pos = calc_cross_position(start_cross, 1000, MOVE_UP); // 1000 -> 1мм
            break;

        case CUT_MODE_RETURN_DOWN:
            next_longitudal_pos = start_longitudal;
        case CUT_MODE_MOVE_DOWN:
            next_cross_pos = calc_cross_position(start_cross, 1000, MOVE_DOWN); // 1000 -> 1мм
            break;

        case CUT_MODE_NO_RETURN:
            next_cross_pos = start_cross;
        default:
            break;
        }

        if (next_cross_pos != start_cross)
            cross_move_to(next_cross_pos, TRUE);
        if (longitudal_pos != next_longitudal_pos)
            longitudal_move_to(next_longitudal_pos, TRUE);
        if (next_cross_pos != start_cross)
            cross_move_to(start_cross, TRUE);
    }
}

static void ui_items_destructor(void *mem)
{
    struct mode_cut_ui_items *ui_items = (struct mode_cut_ui_items *)mem;
    kmem_deref(&ui_items->uptime);
    kmem_deref(&ui_items->number_passes);
    kmem_deref(&ui_items->cutting_speed);
    kmem_deref(&ui_items->feed_speed);
    kmem_deref(&ui_items->cut_depth);
    kmem_deref(&ui_items->cross_miss_up);
    kmem_deref(&ui_items->cross_miss_down);
    kmem_deref(&ui_items->longitudal_miss_left);
    kmem_deref(&ui_items->longitudal_miss_right);
}


void mode_cut_run(void)
{
    struct machine *m = &machine;
    struct ui_main *um = ui_main;
    struct mode_cut *mc = &mode_cut;
    struct mode_cut_ui_items *ui_items;

    ui_items = kzref_alloc("mode_cut_ui_items", sizeof *ui_items,
                           ui_items_destructor);
    mc->disp = m->disp2;
    mc->ui_items = ui_items;

    display_status_show(mc);
    mc->start_time = now();

    set_normal_acceleration();

    switch (um->prog) {
    case PROG_FEED_LEFT:
        run_feed_left_right_only(MOVE_LEFT, um->longitudial_limit);
        break;
    case PROG_FEED_RIGHT:
        run_feed_left_right_only(MOVE_RIGHT, um->longitudial_limit);
        break;
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
    case PROG_THREAD_LEFT:
    case PROG_THREAD_RIGHT:
    }

    display_finished_show(mc);
  //  beep_blink_start(500, 1000, 0);
    while(is_switch_on(m->switch_run)) {
        yield();
    }
 //   beep_blink_stop();
    display_status_hide();
    kmem_deref(&ui_items);
}
