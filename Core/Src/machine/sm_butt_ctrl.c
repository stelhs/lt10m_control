/*
 * sm_butt_ctrl.c
 *
 *  Created on: Jun 3, 2025
 *      Author: stelhs
 */
#include "sm_butt_ctrl.h"
#include "stm32_lib/kref_alloc.h"


static void sm_butt_ctrl_destructor(void *mem)
{
    struct sm_butt_ctrl *sbc = (struct sm_butt_ctrl *)mem;
    kmem_deref(&sbc->bt);
    kmem_deref(&sbc->sm);
}


struct sm_butt_ctrl *
sm_butt_ctrl_register(struct button *bt, struct stepper_motor *sm,
                      bool direction, int start_speed, int inc_interval,
                      int inc_freq)
{
    struct sm_butt_ctrl *sbc;
    sbc = kzref_alloc("sm_butt_ctrl", sizeof *sbc, sm_butt_ctrl_destructor);
    sbc->bt = bt;
    sbc->sm = sm;
    sbc->direction = direction;
    sbc->start_speed = start_speed;
    sbc->inc_interval = inc_interval;
    sbc->inc_freq = inc_freq;
    sbc->start_inc_freq = inc_freq;
    return sbc;
}

void sm_butt_ctrl_do(struct sm_butt_ctrl *sbc)
{
    if (is_button_changed(sbc->bt)) {
        if (is_button_being_pressed(sbc->bt)) {
            sbc->step = 1;
            sbc->inc_freq = sbc->start_inc_freq;
            timeout_start(&sbc->t, sbc->inc_interval);
            stepper_motor_run(sbc->sm, sbc->start_speed, sbc->direction);
            return;
        }

        stepper_motor_stop(sbc->sm);
        return;
    }

    if (sbc->sm->freq > sbc->max_freq) {
        stepper_motor_set_speed(sbc->sm, sbc->max_freq);
        return;
    }

    if (is_button_being_pressed(sbc->bt)) {
        if (is_timeout_elapsed(&sbc->t)) {
            if (sbc->sm->freq == sbc->max_freq)
                return;

            if ((sbc->sm->freq + sbc->inc_freq) > sbc->max_freq) {
                stepper_motor_set_speed(sbc->sm, sbc->max_freq);
                return;
            }

            timeout_start(&sbc->t, sbc->inc_interval);
            sbc->inc_freq += ((sbc->step * 1000) / 3500);
            stepper_motor_set_speed(sbc->sm, sbc->sm->freq + sbc->inc_freq);
            sbc->step += 1;
        }
    }
}

void sm_butt_ctrl_set_max_freq(struct sm_butt_ctrl *sbc, int max_freq)
{
    sbc->max_freq = max_freq;
    if (sbc->sm->freq > (sbc->max_freq - 300)) {
        sbc->inc_freq = sbc->start_inc_freq;
        sbc->step = 0;
    }
}
