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
    TIM_HandleTypeDef *pulse_htim;
    TIM_HandleTypeDef *cnt_htim;
    u32 channel_num;
    int timer_freq;
    int min_freq;
    int max_freq;
    struct gpio *dir;
    struct gpio *en;
    int freq;
    int start_freq;
    int target_freq;
    int distance_um; // in micro-meters
    volatile bool is_run;
    u32 start_braking_point;
    void (*freq_changer_handler)(struct stepper_motor *, bool);
    u32 start_acceleration;
    u32 acceleration;
    int accel_prescaller_cnt;
    bool last_dir;
    int resolution;
    int timer_devider_cnt;
    int speed;
    int prev_position;
    int ref_speed;
    int ref_freq;
};

struct stepper_motor *
stepper_motor_register(char *name, TIM_HandleTypeDef *cnt_htim,
                       TIM_HandleTypeDef *pulse_htim, u32 channel_num,
                       struct gpio *dir, struct gpio *en, int timer_freq,
                       int min_freq, int max_freq, int resolution,
                       int ref_freq, int ref_speed);

void stepper_motor_set_freq_changer_handler(struct stepper_motor *sm,
               void (*freq_changer_handler)(struct stepper_motor *, bool));

void stepper_motor_set_freq(struct stepper_motor *sm, int freq); // IRQ context
void stepper_motor_set_target_freq(struct stepper_motor *sm, int target_freq);
void stepper_motor_enable(struct stepper_motor *sm);
void stepper_motor_disable(struct stepper_motor *sm);

void stepper_motor_run(struct stepper_motor *sm, int start_freq,
                       int target_freq, bool dir, int distance);

void stepper_motor_stop(struct stepper_motor *sm);
void stepper_motor_wait_autostop(struct stepper_motor *sm);
int stepper_motor_pos(struct stepper_motor *sm);
void stepper_motor_reset_pos(struct stepper_motor *sm);
bool is_stepper_motor_run(struct stepper_motor *sm);

// IRQ context
static inline void stepper_motor_stop_isr(struct stepper_motor *sm)
{
    stepper_motor_stop(sm);
}

// IRQ context
static inline void stepper_motor_timer_isr(struct stepper_motor *sm)
{
    int pos;
    sm->timer_devider_cnt++;
    if (sm->timer_devider_cnt > 1000) {
        sm->timer_devider_cnt = 0;
        pos = stepper_motor_pos(sm);
        sm->speed = pos - sm->prev_position;
        sm->prev_position = pos;
    }

    if (!sm->is_run)
        return;

    if (sm->freq_changer_handler)
        sm->freq_changer_handler(sm, FALSE);
}

static inline int speed_to_freq(struct stepper_motor *sm, int speed)
{
    return (speed * sm->ref_freq) / sm->ref_speed;
}


#endif /* SRC_MACHINE_STEPPER_MOTOR_H_ */
