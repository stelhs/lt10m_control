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
    struct ui_move_to *umt = &m->ui_move_to;
    int pos;
    int move_step = ui_move_to_step();

    if (is_button_clicked(m->btn_up)) {
        pos = cross_up_new_position(move_step / 2);
        ui_item_blink(umt->up_arrow, 300);
        cross_move_to(pos, TRUE, NULL, NULL);
        ui_item_blink_stop(umt->up_arrow);
    }

    if (is_button_clicked(m->btn_down)) {
        pos = cross_down_new_position(move_step / 2);
        ui_item_blink(umt->down_arrow, 300);
        cross_move_to(pos, TRUE, NULL, NULL);
        ui_item_blink_stop(umt->down_arrow);
    }

    if (is_button_clicked(m->btn_left)) {
        pos = longitudal_left_new_position(move_step);
        ui_item_blink(umt->left_arrow, 300);
        longitudal_move_to(pos, TRUE, 0, NULL, NULL);
        ui_item_blink_stop(umt->left_arrow);
    }

    if (is_button_clicked(m->btn_right)) {
        pos = longitudal_right_new_position(move_step);
        ui_item_blink(umt->right_arrow, 300);
        longitudal_move_to(pos, TRUE, 0, NULL, NULL);
        ui_item_blink_stop(umt->right_arrow);
    }
}


static void move_button_high_speed_handler(void)
{
    struct machine *m = &machine;

    if (is_button_changed(m->btn_up)) {
        if (is_button_held_down(m->btn_up)) {
            stepper_motor_enable(m->sm_cross);
            stepper_motor_run(m->sm_cross, 200, 0,
                              MOVE_UP, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_cross);
            stepper_motor_stop(m->sm_cross);
            button_reset(m->btn_up);
        }
    }

    if (is_button_changed(m->btn_down)) {
        if (is_button_held_down(m->btn_down)) {
            stepper_motor_enable(m->sm_cross);
            stepper_motor_run(m->sm_cross, 200, 0,
                              MOVE_DOWN, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_cross);
            stepper_motor_stop(m->sm_cross);
            button_reset(m->btn_down);
        }
    }

    if (is_button_changed(m->btn_left)) {
        if (is_button_held_down(m->btn_left)) {
            stepper_motor_enable(m->sm_longitudial);
            stepper_motor_run(m->sm_longitudial, 300, 0,
                              MOVE_LEFT, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_longitudial);
            stepper_motor_stop(m->sm_longitudial);
            button_reset(m->btn_left);
        }
    }

    if (is_button_changed(m->btn_right)) {
        if (is_button_held_down(m->btn_right)) {
            stepper_motor_enable(m->sm_longitudial);
            stepper_motor_run(m->sm_longitudial, 300, 0,
                              MOVE_RIGHT, NO_AUTO_STOP);
        }
        else {
            stepper_motor_disable(m->sm_longitudial);
            stepper_motor_stop(m->sm_longitudial);
            button_reset(m->btn_right);
        }
    }
}

static void move_button_low_speed_handler(void)
{
    struct machine *m = &machine;

    int cross_freq = potentiometer_val(m->pm_move_speed) * 5;
    int longitudal_freq = potentiometer_val(m->pm_move_speed) * 10;
    if (cross_freq < m->sm_cross->min_freq)
        cross_freq = m->sm_cross->min_freq;
    if (longitudal_freq < m->sm_longitudial->min_freq)
        longitudal_freq = m->sm_longitudial->min_freq;

    if (is_potentiometer_changed(m->pm_move_speed)) {
        stepper_motor_set_target_freq(m->sm_cross, cross_freq);
        stepper_motor_set_target_freq(m->sm_longitudial, longitudal_freq);
    }


    if (is_button_changed(m->btn_up)) {
        if (is_button_held_down(m->btn_up)) {
            if (m->sm_cross->last_dir != MOVE_UP) {
                int pos = cross_up_new_position(LINEAR_CROSS_RESOLUTION * 2);
                cross_move_to(pos, FALSE, NULL, NULL);
                buttons_reset();
            }
            stepper_motor_run(m->sm_cross, cross_freq, cross_freq,
                              MOVE_UP, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_cross);
    }

    if (is_button_changed(m->btn_down)) {
        if (is_button_held_down(m->btn_down)) {
            if (m->sm_cross->last_dir != MOVE_DOWN) {
                int pos = cross_down_new_position(LINEAR_CROSS_RESOLUTION * 2);
                cross_move_to(pos, FALSE, NULL, NULL);
                buttons_reset();
            }
            stepper_motor_run(m->sm_cross, cross_freq, cross_freq,
                              MOVE_DOWN, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_cross);
    }

    if (is_button_changed(m->btn_left)) {
        if (is_button_held_down(m->btn_left)) {
            if (m->sm_longitudial->last_dir != MOVE_LEFT) {
                int pos = longitudal_left_new_position(LINEAR_LONGITUDAL_RESOLUTION);
                longitudal_move_to(pos, FALSE, 0, NULL, NULL);
                buttons_reset();
            }
            stepper_motor_run(m->sm_longitudial,
                              longitudal_freq, longitudal_freq,
                              MOVE_LEFT, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_longitudial);
    }

    if (is_button_changed(m->btn_right)) {
        if (is_button_held_down(m->btn_right)) {
            if (m->sm_longitudial->last_dir != MOVE_RIGHT) {
                int pos = longitudal_right_new_position(LINEAR_LONGITUDAL_RESOLUTION);
                longitudal_move_to(pos, FALSE, 0, NULL, NULL);
                buttons_reset();
            }
            stepper_motor_run(m->sm_longitudial,
                              longitudal_freq, longitudal_freq,
                              MOVE_RIGHT, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_longitudial);
    }
}


void mode_idle_run(void)
{
    struct machine *m = &machine;

    if (is_switch_on(m->switch_high_speed))
        set_high_acceleration();
    else
        set_normal_acceleration();

    for(;;) {
        yield();
        if(m->is_busy)
            continue;

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
    }
}

