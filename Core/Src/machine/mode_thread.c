/*
 * mode_thread.c
 *
 *  Created on: Jul 8, 2025
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
#include "thread_table.h"


static struct text_style normal_text_style = {
        .bg_color = BLACK,
        .color = LIGHT_GRAY,
        .font = font_rus,
        .fontsize = 2,
};

void standart_thread_info(const struct thread_metric *tm, bool is_internal,
                          int diameter, struct thread_metric_info *tmi)
{
    if (is_internal) {
        tmi->diameter_max = tm->internal_minor.max;
        tmi->diameter_min = tm->internal_minor.min;
        tmi->diameter = tmi->diameter_min +
                            (tmi->diameter_max - tmi->diameter_min) / 2;
        if (diameter >= tm->internal_minor.min &&
                diameter <= tm->internal_minor.max) {
            tmi->depth_max = (diameter - tm->internal_major.min) / 2;
            tmi->depth_min = (diameter - tm->internal_major.max) / 2;
            tmi->depth = tmi->depth_min + (tmi->depth_max - tmi->depth_min) / 2;
        } else {
            tmi->depth_max = (tm->internal_major.max - tm->internal_minor.min) / 2;
            tmi->depth_min = (tm->internal_major.min - tm->internal_minor.max) / 2;
            tmi->depth = tmi->depth_min + (tmi->depth_max - tmi->depth_min) / 2;
        }

        tmi->end_diameter_max = tm->internal_major.max;
        tmi->end_diameter_min = tm->internal_major.min;
        tmi->end_diameter = tmi->end_diameter_min +
                            (tmi->end_diameter_max - tmi->end_diameter_min) / 2;
        return;
    }

    if (diameter >= tm->bolt_major.min && diameter <= tm->bolt_major.max) {
        tmi->depth_max = (diameter - tm->bolt_minor.min) / 2;
        tmi->depth_min = (diameter - tm->bolt_minor.max) / 2;
        tmi->depth = tmi->depth_min + (diameter - tm->bolt_major.min) / 2;
    } else {
        tmi->depth_max = (tm->bolt_major.max - tm->bolt_minor.min) / 2;
        tmi->depth_min = (tm->bolt_major.min - tm->bolt_minor.max) / 2;
        tmi->depth = tmi->depth_min + (tmi->depth_max - tmi->depth_min) / 2;
    }
    tmi->diameter_max = tm->bolt_major.max;
    tmi->diameter_min = tm->bolt_major.min;
    tmi->diameter = tmi->diameter_min +
                        (tmi->diameter_max - tmi->diameter_min) / 2;

    tmi->end_diameter_max = tm->bolt_minor.max;
    tmi->end_diameter_min = tm->bolt_minor.min;
    tmi->end_diameter = tmi->end_diameter_min +
                        (tmi->end_diameter_max - tmi->end_diameter_min) / 2;

    return;
}

int standart_steps_list(int diameter, u32 *list)
{
    const struct thread_metric *tm;
    int cnt = 0;
    for (tm = thread_metric_table; tm->diameter; tm++) {
        if (tm->diameter != diameter)
            continue;
        if (list)
            list[cnt] = tm->step;
        cnt++;
    }
    return cnt;
}

static void display_status_handler(struct mode_thread *mt)
{
    struct machine *m = &machine;
    if (m->is_disp2_needs_redraw) {
        m->is_disp2_needs_redraw = FALSE;
        ui_item_show(mt->status_text);
        ui_scope_update(mt->ui_status_scope);
        return;
    }
    ui_scope_show(mt->ui_status_scope);
}

static void thread_process_handler(void *priv)
{
    struct mode_thread *mt = (struct mode_thread *)priv;
    display_status_handler(mt);
}


static int target_longitudal_freq(struct mode_thread *mt)
{
    struct machine *m = &machine;
    return feed_rate_to_freq(m->sm_longitudial, spindle_speed(),
                             mt->step_size);
}



static void gap_work_out(void)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    int max_freq = target_longitudal_freq(mt);
    int pos;

    pos = calc_longitudal_position(mt->start_longitudal_pos,
                                   1000, !mt->longitudal_dir);
    longitudal_move_to(pos, FALSE, max_freq, thread_process_handler, mt);
    sleep(300);
    longitudal_move_to(mt->start_longitudal_pos, FALSE, max_freq,
                       thread_process_handler, mt);
    sleep(300);
}

static int thread_cut_run(struct mode_thread *mt, int distance, int freq)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_longitudial;

    stepper_motor_set_freq_changer_handler(m->sm_longitudial,
                                            sm_longitudial_high_acceleration_changer);
    sm->is_allow_run_out = TRUE;
    stepper_motor_run(sm, 500, freq,
                      mt->longitudal_dir, distance);
    while(is_stepper_motor_run(sm)) {
        yield();

        if (!is_switch_on(m->switch_run)) {
            stepper_motor_stop(sm);
            stepper_motor_set_freq_changer_handler(m->sm_longitudial,
                                                    sm_normal_acceleration_changer);
            sm->is_allow_run_out = FALSE;
            return -1;
        }
    }
    stepper_motor_set_freq_changer_handler(m->sm_longitudial,
                                            sm_normal_acceleration_changer);
    sm->is_allow_run_out = FALSE;
    return 0;
}

static int thread_return_run(struct mode_thread *mt)
{
    int parking_cross_pos;
    int rc;

    parking_cross_pos =
            calc_cross_position(mt->start_cross_pos,
                                mt->step_size + 500, !mt->cross_dir);

    rc = cross_move_to(parking_cross_pos, FALSE, thread_process_handler, mt);
    if(rc)
        return rc;

    rc = longitudal_move_to(mt->start_longitudal_pos, FALSE, 0,
                            thread_process_handler, mt);
    if(rc)
        return rc;

    if (parking_cross_pos != mt->start_cross_pos) {
        rc = cross_move_to(mt->start_cross_pos, TRUE,
                           thread_process_handler, mt);
        if(rc)
            return rc;
    }
    return 0;
}


static int calc_longitudal_error(struct mode_thread *mt,
                                 int start_pos, int curr_pos)
{
    struct machine *m = &machine;
    int error;

    error = abs(start_pos - curr_pos);
    if (is_longitudal_target_position_left(m->ap, curr_pos, start_pos)) {
        if (mt->longitudal_dir == MOVE_RIGHT)
            error *= -1;
    } else {
        if (mt->longitudal_dir == MOVE_LEFT)
            error *= -1;
    }
    return error;
}

static int add_raw_angle(int start, int add)
{
    start %= SPINDLE_ENC_RESOLUTION;
    add %= SPINDLE_ENC_RESOLUTION;
    start += add;
    if (start < 0)
        return SPINDLE_ENC_RESOLUTION + start;
    return start % SPINDLE_ENC_RESOLUTION;
}


static int length_to_raw_angle(int length, int step_size)
{
    length %= step_size;
    return (length * SPINDLE_ENC_RESOLUTION) / step_size;
}


static bool calculate_thread(struct mode_thread *mt)
{
    struct mode_thread_settings *mt_settings = &mt->settings;
    mt->cut_offset -= mt_settings->cut_depth_step / 2;
    mt_settings->longitudal_start += mt_settings->cut_depth_step / 2;
    mt->curr_depth += mt_settings->cut_depth_step;
    mt->curr_depth_step = mt_settings->cut_depth_step;

    mt->cut_pass_rest = (mt->max_depth - mt->curr_depth) / mt_settings->cut_depth_step;

    if (mt->curr_depth >= mt->max_depth) {
        mt->curr_depth_step -= (mt->curr_depth - mt->max_depth);
        mt->curr_depth = mt->max_depth;
    }

    if (mt->curr_depth >= mt->max_depth) {
        if (mt->repeate_number < mt->settings.last_repeate_number) {
            mt->repeate_number ++;
            return TRUE;
        }
        return FALSE;
    }

    return TRUE;
}


static void ui_text(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct text_style *ts = &normal_text_style;
    int text_height = disp_text_height(ts);
    int row_height = text_height + 2;
    const int row_start = 115;
    int row = 0;

    disp_text(disp, msg_uptime, 0, row_start + row_height * row, ts);
    row++;

    disp_text(disp, msg_feed_number, 0, row_start + row_height * row, ts);
    row++;

    disp_text(disp, msg_thread_depth, 0, row_start + row_height * row, ts);
}


static void ui_uptime_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_thread *mt = (struct mode_thread *)ut->priv;
    u32 uptime = (now() - mt->start_time) / 1000;
    char *uptime_str = interval_to_str(uptime);
    char *calc_time_str = interval_to_str(mt->calc_time);
    snprintf(str, size, "%s / %s ", uptime_str, calc_time_str);
    kmem_deref(&uptime_str);
    kmem_deref(&calc_time_str);
}

static void ui_number_passes_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_thread *mt = (struct mode_thread *)ut->priv;
    snprintf(str, size, "%d/%d", mt->cut_pass_cnt, mt->cut_pass_rest);
}

static void ui_curr_depth_getter(struct ui_item *ut, char *str, size_t size)
{
    struct mode_thread *mt = (struct mode_thread *)ut->priv;
    snprintf(str, size, "%.3f | %.3f", (float)mt->curr_depth_step / 1000,
             (float)mt->curr_depth / 1000);
}

static void thread_status_init(struct mode_thread *mt)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;
    struct text_style *ts = &normal_text_style;
    int text_height = disp_text_height(ts);
    int row_height = text_height + 2;
    const int row_start = 120;
    int row = 0;

    ui_message_hide();

    mt->ui_status_scope = ui_scope_create("mode_thread_ui_stat_scope");
    mt->status_text = ui_item_register("ui_text", NULL, disp,
                                       0, 115, 480, 205,
                                       ui_text, NULL, mt, 0);

    ui_item_text_register("ui_uptime", mt->ui_status_scope, disp,
                          220, row_start + row_height * row, 25, ts,
                          ui_uptime_getter, NULL, mt);
    row++;

    ui_item_text_register("ui_number_passes", mt->ui_status_scope, disp,
                         220, row_start + row_height * row, 5, ts,
                         ui_number_passes_getter, NULL, mt);
    row++;
    ui_item_text_register("ui_curr_depth", mt->ui_status_scope, disp,
                         220, row_start + row_height * row, 18, ts,
                         ui_curr_depth_getter, NULL, mt);

}

int thread_calc_work_time(int length, int pass_number, int sm_freq)
{
    struct machine *m = &machine;
    int speed = speed_by_freq(m->sm_longitudial, sm_freq);
    int one_pass_sec = length / speed + length / 10000 + 5;
    return one_pass_sec * pass_number;
}

void thread_state_init(void)
{
    struct machine *m = &machine;
    struct stepper_motor *sm_longitudal = m->sm_longitudial;
    struct mode_thread *mt = &m->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    int max_speed = speed_by_freq(sm_longitudal, sm_longitudal->max_freq);
    int error, error_raw_angle;

    switch (m->prog) {
    case PROG_THREAD_LEFT:
        mt->longitudal_dir = MOVE_LEFT; break;
    case PROG_THREAD_RIGHT:
        mt->longitudal_dir = MOVE_RIGHT; break;
    default:
        panic("mode_thread_run() incorrect program");
    }

    if (mt_settings->is_internal)
        mt->cross_dir = MOVE_DOWN;
    else
        mt->cross_dir = MOVE_UP;

    mt->max_depth = mt_settings->max_cut_depth;
    mt->curr_depth = 0;
    mt->cut_offset = 0;

    if (mt_settings->is_type_inch) {
        mt->step_size = 25400 / (mt_settings->thread_size / 1000);
    } else {
        mt->step_size = mt_settings->thread_size;
    }

    mt->krpm_min = 5000;
    mt->krpm_max = (1000 / ((mt->step_size * 1000) / max_speed)) * 60 * 1000;

    mt->start_longitudal_pos = abs_longitudal_curr_tool(m->ap);
    mt->end_longitudal_pos = calc_longitudal_position(mt->start_longitudal_pos,
                                                      mt_settings->length,
                                                      mt->longitudal_dir);

    mt->start_cross_pos = abs_cross_curr_tool(m->ap);
    mt->end_cross_pos = calc_cross_position(mt->start_cross_pos,
                                            mt->step_size,
                                            mt->cross_dir);
    mt->repeate_number = 0;

    mt->calc_cut_pass_num = mt->max_depth / mt_settings->cut_depth_step +
                            mt_settings->last_repeate_number;
    mt->calc_time =
            thread_calc_work_time(mt_settings->length,
                                  mt->calc_cut_pass_num,
                                  sm_longitudal->max_freq);

    error = calc_longitudal_error(mt, mt_settings->longitudal_start,
                                  mt->start_longitudal_pos);
    error_raw_angle = length_to_raw_angle(error, mt->step_size);
    mt->start_angle = add_raw_angle(mt_settings->spindle_start,
                                    error_raw_angle * -1);
    mt->cut_pass_cnt = 0;
}

static int thread_cutting_run(void)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    int new_cross_pos;
    int cut_freq;
    int rc;
    int i;

    m->is_disp2_needs_redraw = TRUE;
    thread_state_init();
    thread_status_init(mt);

    mt->start_time = now();
    mt->last_start_longitudal_pos = mt->start_longitudal_pos;

    set_normal_acceleration();

    int krpm = spindle_speed();
    if (krpm < mt->krpm_min || krpm > mt->krpm_max) {
        ui_message_show(msg_thread_rpm_not_correct, MSG_ERR);
        while(is_switch_on(m->switch_run)) {
            yield();
        }
        ui_message_hide();
        return -1;
    }

    cut_freq = target_longitudal_freq(mt);

    i = 0;
    while (calculate_thread(mt)) {
        int error_raw_angle;
        int error_fix_raw_angle;
        int offset_raw_angle;
        int entry_raw_angle;
        int curr_pos;
        int curr_angle;
        int error;
        int distance;

        i++;
        mt->cut_pass_cnt++;
        mt->calc_time =
                thread_calc_work_time(mt_settings->length,
                                      mt->calc_cut_pass_num, cut_freq);

        printf("iteration: %d\r\n", i);
        printf("mt->curr_depth = %d\r\n", mt->curr_depth);
        printf("mt->cut_offset = %d\r\n", mt->cut_offset);
        printf("mt->step_size = %d\r\n", mt->step_size);

        new_cross_pos = calc_cross_position(mt->start_cross_pos,
                                            mt->curr_depth,
                                            mt->cross_dir);
        rc = cross_move_to(new_cross_pos, TRUE, thread_process_handler, mt);
        if(rc)
            return rc;

        gap_work_out();

        curr_pos = abs_longitudal_curr_tool(m->ap);
        error = calc_longitudal_error(mt, mt->start_longitudal_pos, curr_pos);
        distance = mt_settings->length + error;

        printf("2 error = %d\r\n", error);
        printf("2 distance = %d\r\n", distance);

        // вычисление угла входа
        error_raw_angle = length_to_raw_angle(error, mt->step_size);
        printf("2 error_raw_angle = %d\r\n", error_raw_angle);

        error_fix_raw_angle = add_raw_angle(mt->start_angle,
                                            error_raw_angle * -1);
        offset_raw_angle = length_to_raw_angle(mt_settings->thread_offset +
                                               mt->cut_offset, mt->step_size);
        entry_raw_angle = add_raw_angle(error_fix_raw_angle, offset_raw_angle);

        printf("error_fix_raw_angle = %d\r\n", error_fix_raw_angle);
        printf("offset_raw_angle = %d\r\n", offset_raw_angle);
        printf("entry_raw_angle = %d\r\n", entry_raw_angle);

        // ждём угол входа
        do {
            curr_angle = spindle_raw_angle();
        } while (curr_angle != entry_raw_angle);

        rc = thread_cut_run(mt, distance, cut_freq);
        if (rc)
            return -1; // TODO

        rc = thread_return_run(mt);
        if (rc)
            return -1; // TODO
    }

    new_cross_pos = calc_cross_position(mt->start_cross_pos,
                                        mt->curr_depth,
                                        mt->cross_dir);

    rc = cross_move_to(new_cross_pos, TRUE, thread_process_handler, mt);
    if(rc)
        return rc;

    return 0;
}

void mode_thread_run(void)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    thread_cutting_run();
    ui_scope_update(m->ui_main->ui_scope);
    program_finished_show();
    beep_blink_start(500, 1000, 0);
    while(is_switch_on(m->switch_run)) {
        yield();
    }
    beep_blink_stop();
    ui_message_hide();
    kmem_deref(&mt->status_text);
    kmem_deref(&mt->ui_status_scope);
}

