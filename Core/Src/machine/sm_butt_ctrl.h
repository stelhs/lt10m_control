/*
 * sm_butt_ctrl.h
 *
 *  Created on: Jun 3, 2025
 *      Author: stelhs
 */

#ifndef SM_BUTT_CTRL_H_
#define SM_BUTT_CTRL_H_

#include "stepper_motor.h"
#include "stm32_lib/buttons.h"

struct sm_butt_ctrl {
    struct button *bt;
    struct stepper_motor *sm;
    bool direction;
    int inc_interval;
    int start_inc_freq;
    int inc_freq;
    int start_speed;
    int max_freq;
    int step;
    struct timeout t;
};

struct sm_butt_ctrl *
sm_butt_ctrl_register(struct button *bt, struct stepper_motor *sm,
                      bool direction, int start_speed, int inc_interval,
                      int inc_freq);
void sm_butt_ctrl_do(struct sm_butt_ctrl *sbc);
void sm_butt_ctrl_set_max_freq(struct sm_butt_ctrl *sbc, int max_speed);

#endif /* SM_BUTT_CTRL_H_ */
