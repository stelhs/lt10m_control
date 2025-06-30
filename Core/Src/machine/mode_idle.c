/*
 * mode_idle.c
 *
 *  Created on: Jun 26, 2025
 *      Author: stelhs
 */

#include "mode_idle.h"

#include "machine.h"
#include "stepper_motor.h"
#include "ui_move_to.h"
#include "stm32_lib/buttons.h"
#include "periphery.h"
#include "abs_position.h"


static void move_buttons_move_to_handler(void)
{
    struct machine *m = &machine;
    int pos;
    int move_step = ui_move_to_step();

    if (is_button_clicked(m->btn_up)) {
        pos = cross_up_new_position(move_step / 2);
        ui_moveto_blink_up_arrow();
        cross_move_to(pos, TRUE);
        ui_moveto_blink_stop();
    }

    if (is_button_clicked(m->btn_down)) {
        pos = cross_down_new_position(move_step / 2);
        ui_moveto_blink_down_arrow();
        cross_move_to(pos, TRUE);
        ui_moveto_blink_stop();
    }

    if (is_button_clicked(m->btn_left)) {
        pos = longitudal_left_new_position(move_step);
        ui_moveto_blink_left_arrow();
        longitudal_move_to(pos, TRUE);
        ui_moveto_blink_stop();
    }

    if (is_button_clicked(m->btn_right)) {
        pos = longitudal_right_new_position(move_step);
        ui_moveto_blink_right_arrow();
        longitudal_move_to(pos, TRUE);
        ui_moveto_blink_stop();
    }
}

// IRQ context
static void
sm_cross_high_acceleration_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    if (sm->freq < sm->target_freq) {
        if (sm->freq < 1000)
            freq += 10;
        else if (sm->freq < 2000)
            freq += 20;
        else if (sm->freq < 5000)
            freq += 30;
        else if (sm->freq < 8000)
            freq += 10;
        else if (sm->freq < 10000)
            freq += 5;
        else
            freq ++;
        if (freq > sm->target_freq)
            freq = sm->target_freq;
    }

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}

// IRQ context
static void
sm_longitudial_high_acceleration_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    if (sm->freq < sm->target_freq) {
        if (sm->freq < 1000)
            freq += 25;
        else if (sm->freq < 2000)
            freq += 200;
        else if (sm->freq < 6000)
            freq += 150;
        else if (sm->freq < 12000)
            freq += 50;
        else
            freq += 10;
        if (freq > sm->target_freq)
            freq = sm->target_freq;
    }

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}


static void set_high_acceleration(void)
{
    struct machine *m = &machine;
    stepper_motor_disable(m->sm_cross_feed);
    stepper_motor_disable(m->sm_longitudial_feed);
    stepper_motor_set_freq_changer_handler(m->sm_cross_feed,
                                      sm_cross_high_acceleration_changer);
    stepper_motor_set_freq_changer_handler(m->sm_longitudial_feed,
                                sm_longitudial_high_acceleration_changer);
}


static void move_button_high_speed_handler(void)
{
    struct machine *m = &machine;

    if (is_button_changed(m->btn_up)) {
        if (is_button_held_down(m->btn_up)) {
            stepper_motor_enable(m->sm_cross_feed);
            stepper_motor_run(m->sm_cross_feed, 200, 0,
                              MOVE_UP, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_cross_feed);
            stepper_motor_stop(m->sm_cross_feed);
            button_reset(m->btn_up);
        }
    }

    if (is_button_changed(m->btn_down)) {
        if (is_button_held_down(m->btn_down)) {
            stepper_motor_enable(m->sm_cross_feed);
            stepper_motor_run(m->sm_cross_feed, 200, 0,
                              MOVE_DOWN, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_cross_feed);
            stepper_motor_stop(m->sm_cross_feed);
            button_reset(m->btn_down);
        }
    }

    if (is_button_changed(m->btn_left)) {
        if (is_button_held_down(m->btn_left)) {
            stepper_motor_enable(m->sm_longitudial_feed);
            stepper_motor_run(m->sm_longitudial_feed, 300, 0,
                              MOVE_LEFT, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_longitudial_feed);
            stepper_motor_stop(m->sm_longitudial_feed);
            button_reset(m->btn_left);
        }
    }

    if (is_button_changed(m->btn_right)) {
        if (is_button_held_down(m->btn_right)) {
            stepper_motor_enable(m->sm_longitudial_feed);
            stepper_motor_run(m->sm_longitudial_feed, 300, 0,
                              MOVE_RIGHT, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_longitudial_feed);
            stepper_motor_stop(m->sm_longitudial_feed);
            button_reset(m->btn_right);
        }
    }
}

static void move_button_low_speed_handler(void)
{
    struct machine *m = &machine;

    int cross_freq = potentiometer_val(m->pm_move_speed) * 5;
    int longitudal_freq = potentiometer_val(m->pm_move_speed) * 10;
    if (cross_freq < m->sm_cross_feed->min_freq)
        cross_freq = m->sm_cross_feed->min_freq;
    if (longitudal_freq < m->sm_longitudial_feed->min_freq)
        longitudal_freq = m->sm_longitudial_feed->min_freq;

    if (is_potentiometer_changed(m->pm_move_speed)) {
        stepper_motor_set_target_freq(m->sm_cross_feed, cross_freq);
        stepper_motor_set_target_freq(m->sm_longitudial_feed, longitudal_freq);
    }


    if (is_button_changed(m->btn_up)) {
        if (is_button_held_down(m->btn_up)) {
            if (m->sm_cross_feed->last_dir != MOVE_UP) {
                int pos = cross_up_new_position(LINEAR_CROSS_RESOLUTION * 2);
                cross_move_to(pos, FALSE);
                buttons_reset();
            }
            stepper_motor_run(m->sm_cross_feed, cross_freq, cross_freq,
                              MOVE_UP, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_cross_feed);
    }

    if (is_button_changed(m->btn_down)) {
        if (is_button_held_down(m->btn_down)) {
            if (m->sm_cross_feed->last_dir != MOVE_DOWN) {
                int pos = cross_down_new_position(LINEAR_CROSS_RESOLUTION * 2);
                cross_move_to(pos, FALSE);
                buttons_reset();
            }
            stepper_motor_run(m->sm_cross_feed, cross_freq, cross_freq,
                              MOVE_DOWN, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_cross_feed);
    }

    if (is_button_changed(m->btn_left)) {
        if (is_button_held_down(m->btn_left)) {
            if (m->sm_longitudial_feed->last_dir != MOVE_LEFT) {
                int pos = longitudal_left_new_position(LINEAR_LONGITUDAL_RESOLUTION);
                longitudal_move_to(pos, FALSE);
                buttons_reset();
            }
            stepper_motor_run(m->sm_longitudial_feed,
                              longitudal_freq, longitudal_freq,
                              MOVE_LEFT, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_longitudial_feed);
    }

    if (is_button_changed(m->btn_right)) {
        if (is_button_held_down(m->btn_right)) {
            if (m->sm_longitudial_feed->last_dir != MOVE_RIGHT) {
                int pos = longitudal_right_new_position(LINEAR_LONGITUDAL_RESOLUTION);
                longitudal_move_to(pos, FALSE);
                buttons_reset();
            }
            stepper_motor_run(m->sm_longitudial_feed,
                              longitudal_freq, longitudal_freq,
                              MOVE_RIGHT, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_longitudial_feed);
    }
}


void mode_idle_run(void)
{
    struct machine *m = &machine;
    void *msg;
    int position;

    if (is_switch_on(m->switch_high_speed))
        set_high_acceleration();
    else
        set_normal_acceleration();

    for(;;) {
        yield();

        if (is_button_changed(m->switch_high_speed)) {
            if (is_switch_on(m->switch_high_speed))
                set_high_acceleration();
            else
                set_normal_acceleration();
        }

        if (is_switch_on(m->switch_high_speed))
            move_button_high_speed_handler();
        else if (is_switch_on(m->switch_move_to))
            move_buttons_move_to_handler();
        else
            move_button_low_speed_handler();

        if (is_switch_on(m->switch_run)) {
            // TODO: stop all
            return;
        }

        switch (thread_recv_msg(&msg)) {
        case MACHINE_MSG_MOVETO_CROSS:
            position = (int)msg;
            ui_moveto_blink_up_down_arrow();
            cross_move_to(position, TRUE);
            buttons_reset();
            ui_moveto_blink_stop();
            ui_move_to_unlock_moveto();
            break;
        case MACHINE_MSG_MOVETO_LONGITUDAL:
            position = (int)msg;
            ui_moveto_blink_left_right_arrow();
            longitudal_move_to(position, TRUE);
            buttons_reset();
            ui_moveto_blink_stop();
            ui_move_to_unlock_moveto();
            break;
        }
    }
}

