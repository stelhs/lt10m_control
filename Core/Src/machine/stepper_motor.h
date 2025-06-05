/*
 * stepper_motor.h
 *
 *  Created on: May 29, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_STEPPER_MOTOR_H_
#define SRC_MACHINE_STEPPER_MOTOR_H_
#include "tim.h"
#include "stm32_lib/gpio.h"


struct stepper_motor {
    char *name;
    TIM_HandleTypeDef *htim;
    u32 channel_num;
    int timer_freq;
    struct gpio *dir;
    struct gpio *en;
    u32 pulse_cnt;
    u32 autostop_pulse_cnt;
    int freq;
    bool is_run;
};

struct stepper_motor *
stepper_motor_register(char *name, TIM_HandleTypeDef *htim, u32 channel_num,
                       struct gpio *dir, struct gpio *en, int timer_freq);

void stepper_motor_enable(struct stepper_motor *sm);
void stepper_motor_disable(struct stepper_motor *sm);

void stepper_motor_run(struct stepper_motor *sm, int freq, bool dir);

static inline void stepper_motor_run_forward(struct stepper_motor *sm, int freq) {
    stepper_motor_run(sm, freq, 1);
}

static inline void stepper_motor_run_backward(struct stepper_motor *sm, int freq) {
    stepper_motor_run(sm, freq, 0);
}

void stepper_motor_stop(struct stepper_motor *sm);
void stepper_motor_set_speed(struct stepper_motor *sm, int freq);
void stepper_motor_set_autostop(struct stepper_motor *sm, int pulse);
u32 stepper_motor_pos(struct stepper_motor *sm);
void stepper_motor_reset_pos(struct stepper_motor *sm);

// IRQ context
static inline void stepper_motor_isr(struct stepper_motor *sm)
{
    sm->pulse_cnt++;
    if (sm->autostop_pulse_cnt && sm->pulse_cnt >= sm->autostop_pulse_cnt) {
        sm->autostop_pulse_cnt = 0;
        stepper_motor_stop(sm);
    }
}

#endif /* SRC_MACHINE_STEPPER_MOTOR_H_ */
