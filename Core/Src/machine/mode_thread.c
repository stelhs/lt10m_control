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

static int target_longitudal_freq(struct mode_thread *mt)
{
    struct machine *m = &machine;
    return feed_rate_to_freq(m->sm_longitudial_feed, spindle_speed(),
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
    longitudal_move_to(pos, FALSE, max_freq);
    sleep(300);
    longitudal_move_to(mt->start_longitudal_pos, FALSE, max_freq);
    sleep(300);
}

static int thread_cut_run(struct mode_thread *mt, int distance, int freq)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_longitudial_feed;

    stepper_motor_set_freq_changer_handler(m->sm_longitudial_feed,
                                            sm_longitudial_high_acceleration_changer);
    sm->is_allow_run_out = TRUE;
    stepper_motor_run(sm, 500, freq,
                      mt->longitudal_dir, distance);
    while(is_stepper_motor_run(sm)) {
        yield();

        if (!is_switch_on(m->switch_run)) {
            stepper_motor_stop(sm);
            stepper_motor_set_freq_changer_handler(m->sm_longitudial_feed,
                                                    sm_normal_acceleration_changer);
            sm->is_allow_run_out = FALSE;
            return -1;
        }
    }
    stepper_motor_set_freq_changer_handler(m->sm_longitudial_feed,
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

    rc = cross_move_to(parking_cross_pos, TRUE);
    if(rc)
        return rc;

    rc = longitudal_move_to(mt->start_longitudal_pos, FALSE, 0);
    if(rc)
        return rc;

    if (parking_cross_pos != mt->start_cross_pos) {
        rc = cross_move_to(mt->start_cross_pos, TRUE);
        if(rc)
            return rc;
    }
    return 0;
}

static int calc_longitudal_error(struct mode_thread *mt)
{
    struct machine *m = &machine;
    int error;
    int curr_pos;

    curr_pos = abs_longitudal_curr_tool(m->ap);
    error = abs(mt->start_longitudal_pos - curr_pos);
    if (is_longitudal_target_position_left(m->ap, curr_pos,
                                           mt->start_longitudal_pos)) {
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

static int triag_height(int side) {
    return side * 866 / 1000;
}


static bool calculate_thread(struct mode_thread *mt)
{
    if ((mt->max_depth - mt->curr_depth) <= 300) {
        mt->curr_depth += mt->cut_depth_step / 4;
    } else {
        mt->cut_offset -= mt->cut_depth_step / 4;
        mt->curr_depth += mt->cut_depth_step / 2;
    }

    if (mt->curr_depth >= mt->max_depth)
        mt->curr_depth = mt->max_depth;


    if (mt->curr_depth >= mt->max_depth) {
        if (mt->repeate_number < mt->settings.last_repeate_number) {
            mt->repeate_number ++;
            return TRUE;
        }
        return FALSE;
    }

    return TRUE;
}




int mode_thread_run(void)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    int cut_freq;
    int krpm;
    int rc;
    int i;

    set_normal_acceleration();
    button_reset(m->btn_ok);

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

    mt->cut_depth_step = mt_settings->cut_depth_step;
    mt->curr_depth = 0;
    mt->cut_offset = 0;

    if (mt_settings->is_type_inch) {
        mt->step_size = 25400 / mt_settings->thread_size;
    } else {
        mt->step_size = mt_settings->thread_size;
    }

    int max_sm_feq = 15000;

    mt->krpm_min = 5000;
    mt->krpm_max = (1000 / ((mt->step_size * 1000) / max_sm_feq)) * 60 * 1000;

    mt->start_longitudal_pos = abs_longitudal_curr_tool(m->ap);
    mt->end_longitudal_pos = calc_longitudal_position(mt->start_longitudal_pos,
                                           mt_settings->length,
                                           mt->longitudal_dir);

    mt->start_cross_pos = abs_cross_curr_tool(m->ap);
    mt->end_cross_pos = calc_cross_position(mt->start_cross_pos,
                                            mt->step_size,
                                            mt->cross_dir);
    mt->repeate_number = 0;

    for (i = 0; calculate_thread(mt); i++);
    printf("Pass number: %d\r\n", i);

    mt->cut_depth_step = mt_settings->cut_depth_step;
    mt->curr_depth = 0;
    mt->cut_offset = 0;
    mt->repeate_number = 0;
    mt->max_depth = triag_height(mt->step_size) / 2; // TODO

    char *msg = kref_sprintf(msg_set_rpm,
                             (float)mt->krpm_min / 1000,
                             (float)mt->krpm_max / 1000);
    ui_message_show(msg, MSG_NORM);
    kmem_deref(&msg);

    while(1) {
        yield();
        if (!is_switch_on(m->switch_run)) {
            ui_message_hide();
            return -1;
        }

        if (is_button_clicked(m->btn_ok)) {
            krpm = spindle_speed();
            if (krpm < mt->krpm_min || krpm > mt->krpm_max)
                continue;

            ui_message_hide();
            break;
        }
    }

    mt->curr_depth = mt->cut_depth_step;
    if (mt->curr_depth >= mt->max_depth)
        mt->curr_depth = mt->max_depth;

    cut_freq = target_longitudal_freq(mt);

    int new_cross_pos = calc_cross_position(mt->start_cross_pos, mt->curr_depth,
                                            mt->cross_dir);
    rc = cross_move_to(new_cross_pos, TRUE);
    if(rc)
        return rc;

    printf("mt->start_longitudal_pos = %d\r\n", mt->start_longitudal_pos);
    printf("mt_settings->length = %d\r\n", mt_settings->length);

    // компенсация люфта
    gap_work_out();

    int error = calc_longitudal_error(mt);
    int distance = mt_settings->length + error;

    printf("1 error = %d\r\n", error);
    printf("1 distance = %d\r\n", distance);

    // вычисление угла входа
    int error_raw_angle = length_to_raw_angle(error, mt->step_size);
    printf("1 error_raw_angle = %d\r\n", error_raw_angle);
    int curr_angle = spindle_raw_angle();
    rc = thread_cut_run(mt, distance, cut_freq);
    if (rc)
        return rc; // TODO
    mt->start_angle = add_raw_angle(curr_angle, error_raw_angle);

    printf("curr_angle = %d\r\n", curr_angle);
    printf("mt->start_angle = %d\r\n", mt->start_angle);

    rc = thread_return_run(mt);
    if (rc)
        return rc; // TODO

    i = 0;
    while (calculate_thread(mt)) {
        int error_fix_raw_angle;
        int entry_raw_angle;
        int offset_raw_angle;
        i++;

        printf("iteration: %d\r\n", i);
        printf("mt->curr_depth = %d\r\n", mt->curr_depth);
        printf("mt->cut_offset = %d\r\n", mt->cut_offset);
        printf("mt->step_size = %d\r\n", mt->step_size);

        new_cross_pos = calc_cross_position(mt->start_cross_pos,
                                            mt->curr_depth,
                                            mt->cross_dir);
        rc = cross_move_to(new_cross_pos, TRUE);
        if(rc)
            return rc;

        gap_work_out();

        error = calc_longitudal_error(mt);
        distance = mt_settings->length + error;

        printf("2 error = %d\r\n", error);
        printf("2 distance = %d\r\n", distance);

        // вычисление угла входа
        error_raw_angle = length_to_raw_angle(error, mt->step_size);
        printf("2 error_raw_angle = %d\r\n", error_raw_angle);

        error_fix_raw_angle = add_raw_angle(mt->start_angle,
                                            error_raw_angle * -1);
        offset_raw_angle = length_to_raw_angle(mt->cut_offset, mt->step_size);
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

    return 0;
}

/*
static void ui_text(struct ui_item *ut)
{
    struct machine *m = &machine;
    struct disp *disp = m->disp2;

    struct text_style ts = {
            .bg_color = BLACK,
            .color = LIGHT_GRAY,
            .font = font_rus,
            .fontsize = 2,
    };

    int text_height = disp_text_height(ts);
    int row_height = text_height + 2;
    const int row_start = 120;

    // draw text lines
    disp_text(disp, msg_thread_calc_time, 0, row_start + row_height * 0, ts);
    disp_text(disp, msg_thread_feed_number, 0, row_start + row_height * 1, ts);
    disp_text(disp, msg_thread_spindle_max, 0, row_start + row_height * 1, ts);
    disp_text(disp, msg_thread_cut_speed_max, 0, row_start + row_height * 1, ts);

    disp_text(disp, msg_thread_m_standard, 0, row_start + row_height * 1, ts);
    disp_text(disp, msg_thread_major_diameter, 0, row_start + row_height * 1, ts);
    disp_text(disp, msg_thread_minor_diameter, 0, row_start + row_height * 1, ts);
    disp_text(disp, msg_thread_steps, 0, row_start + row_height * 1, ts);

}


void display_thread_settings(void)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    struct disp *disp = m->disp2;

    mc->status_text = ui_item_register("ui_text", NULL, disp,
                                       0, 90, 480, 230,
                                       ui_text, NULL, mc, 0);

}
*/
