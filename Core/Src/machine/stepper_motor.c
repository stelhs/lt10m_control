/*
 * stepper_motor.c
 *
 *  Created on: May 29, 2025
 *      Author: stelhs
 */

#include "stm32_lib/gpio.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/kref_alloc.h"
#include "stepper_motor.h"


static void stepper_motor_destructor(void *mem)
{
    struct stepper_motor *sm = (struct stepper_motor *)mem;
    UNUSED(sm);
}


struct stepper_motor *
stepper_motor_register(char *name, TIM_HandleTypeDef *htim, u32 channel_num,
                       struct gpio *dir, struct gpio *en, int timer_freq)
{
    struct stepper_motor *sm;

    sm = kzref_alloc(name, sizeof *sm, stepper_motor_destructor);
    sm->name = name;
    sm->channel_num = channel_num;
    sm->htim = htim;
    sm->dir = dir;
    sm->en = en;
    sm->timer_freq = timer_freq;

    stepper_motor_disable(sm);
    return sm;
}

void
stepper_motor_set_freq_changer_handler(struct stepper_motor *sm,
               void (*freq_changer_handler)(struct stepper_motor *))
{
    irq_disable();
    sm->freq_changer_handler = freq_changer_handler;
    irq_enable();
}

// IRQ context
void stepper_motor_set_freq(struct stepper_motor *sm, int freq)
{
    u32 period = sm->timer_freq / freq;
    __HAL_TIM_SET_AUTORELOAD(sm->htim, period - 1);
    __HAL_TIM_SET_COMPARE(sm->htim, sm->channel_num, (period - 1) / 2); // Duty Cycle 50%
    sm->freq = freq;
}

void stepper_motor_enable(struct stepper_motor *sm)
{
    gpio_down(sm->en);
}

void stepper_motor_disable(struct stepper_motor *sm)
{
    gpio_up(sm->en);
}

void stepper_motor_run(struct stepper_motor *sm, int start_freq,
                       int target_freq, bool dir)
{
    printf("%s: stepper_motor_run %d->%d %d\r\n",
           sm->name, start_freq, target_freq, dir);

    sm->target_freq = target_freq;
    sm->freq = start_freq;
    // TODO
    u32 period = sm->timer_freq / start_freq;
    __HAL_TIM_SET_AUTORELOAD(sm->htim, period - 1);
    __HAL_TIM_SET_COMPARE(sm->htim, sm->channel_num, (period - 1) / 2); // Duty Cycle 50%

    __HAL_TIM_SET_COUNTER(sm->htim, 0);
    HAL_TIM_PWM_Start(sm->htim, sm->channel_num);
    HAL_TIM_Base_Start_IT(sm->htim);
    sm->is_run = TRUE;
    dir ? gpio_up(sm->dir) : gpio_down(sm->dir);
}

void stepper_motor_stop(struct stepper_motor *sm)
{
    printf("%s: stepper_motor_stop\r\n", sm->name);
    HAL_TIM_PWM_Stop(sm->htim, sm->channel_num);
    HAL_TIM_Base_Stop_IT(sm->htim);
    sm->is_run = FALSE;
    gpio_down(sm->dir);
}

void stepper_motor_set_speed(struct stepper_motor *sm, int target_freq)
{
    printf("%s: freq = %d\r\n", sm->name, sm->freq);
    irq_disable();
    sm->target_freq = target_freq;
    irq_enable();
}

void stepper_motor_set_autostop(struct stepper_motor *sm, int pulse)
{
    irq_disable();
    sm->autostop_pulse_cnt = pulse;
    irq_enable();
}

void stepper_motor_wait_autostop(struct stepper_motor *sm)
{
    while (is_stepper_motor_run(sm)) {
        yield();
    }
}

u32 stepper_motor_pos(struct stepper_motor *sm)
{
    u32 pulse_cnt;
    irq_disable();
    pulse_cnt = sm->pulse_cnt;
    irq_enable();
    return pulse_cnt;
}

void stepper_motor_reset_pos(struct stepper_motor *sm)
{
    irq_disable();
    sm->pulse_cnt = 0;
    irq_enable();
}

bool is_stepper_motor_run(struct stepper_motor *sm)
{
    bool is_run;
    irq_disable();
    is_run = sm->is_run;
    irq_enable();
    return is_run;
}

