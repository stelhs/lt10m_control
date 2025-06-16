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
#include "machine.h"


static void stepper_motor_destructor(void *mem)
{
    struct stepper_motor *sm = (struct stepper_motor *)mem;
    UNUSED(sm);
}


struct stepper_motor *
stepper_motor_register(char *name, TIM_HandleTypeDef *cnt_htim,
                       TIM_HandleTypeDef *pulse_htim, u32 channel_num,
                       struct gpio *dir, struct gpio *en, int timer_freq,
                       int min_freq, int max_freq)
{
    struct stepper_motor *sm;

    sm = kzref_alloc(name, sizeof *sm, stepper_motor_destructor);
    sm->name = name;
    sm->channel_num = channel_num;
    sm->pulse_htim = pulse_htim;
    sm->cnt_htim = cnt_htim;
    sm->dir = dir;
    sm->en = en;
    sm->timer_freq = timer_freq;
    sm->min_freq = min_freq;
    sm->max_freq = max_freq;
    sm->start_acceleration = 5;

    stepper_motor_disable(sm);
    return sm;
}

void
stepper_motor_set_freq_changer_handler(struct stepper_motor *sm,
               void (*freq_changer_handler)(struct stepper_motor *, bool))
{
    irq_disable();
    sm->freq_changer_handler = freq_changer_handler;
    irq_enable();
}

// IRQ context
void stepper_motor_set_freq(struct stepper_motor *sm, int freq)
{
    u32 period = sm->timer_freq / freq;
    __HAL_TIM_SET_AUTORELOAD(sm->pulse_htim, period - 1);
    __HAL_TIM_SET_COMPARE(sm->pulse_htim, sm->channel_num, (period - 1) / 2); // Duty Cycle 50%
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
                       int target_freq, bool dir, u32 distance_um)
{
    printf("%s: stepper_motor_run %d->%d %d\r\n",
           sm->name, start_freq, target_freq, dir);

    if (start_freq < sm->min_freq)
        panic("attempt to set frequency below allowed");

    if (target_freq > sm->max_freq)
        panic("attempt to set frequency higher than allowed");

    sm->start_freq = start_freq;
    sm->target_freq = target_freq;
    sm->freq = start_freq;
    sm->distance_um = distance_um;

    if (sm->freq_changer_handler)
        sm->freq_changer_handler(sm, TRUE);

    if (!dir)
        sm->cnt_htim->Instance->CCER |= TIM_CCER_CC1P;
    else
        sm->cnt_htim->Instance->CCER &= ~TIM_CCER_CC1P;

    __HAL_TIM_SET_COUNTER(sm->cnt_htim, 0);

    __HAL_TIM_SET_AUTORELOAD(sm->cnt_htim, distance_um / 5);
    HAL_TIM_Base_Start_IT(sm->cnt_htim);

    stepper_motor_set_freq(sm, start_freq);
    __HAL_TIM_SET_COUNTER(sm->pulse_htim, 0);
    HAL_TIM_PWM_Start(sm->pulse_htim, sm->channel_num);
    HAL_TIM_Base_Start_IT(sm->pulse_htim);
    sm->is_run = TRUE;
    dir ? gpio_up(sm->dir) : gpio_down(sm->dir);
}

void stepper_motor_stop(struct stepper_motor *sm)
{
    printf("%s: stepper_motor_stop\r\n", sm->name);
    HAL_TIM_PWM_Stop(sm->pulse_htim, sm->channel_num);
    HAL_TIM_Base_Stop_IT(sm->pulse_htim);
    HAL_TIM_Base_Stop_IT(sm->cnt_htim);
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

void stepper_motor_wait_autostop(struct stepper_motor *sm)
{
    while (is_stepper_motor_run(sm))
        yield();
}

u32 stepper_motor_pos(struct stepper_motor *sm)
{
    // 5 micron per impulse
    return __HAL_TIM_GET_COUNTER(sm->cnt_htim) * 5;
}

void stepper_motor_reset_pos(struct stepper_motor *sm)
{
    __HAL_TIM_SET_COUNTER(sm->cnt_htim, 0);
}

bool is_stepper_motor_run(struct stepper_motor *sm)
{
    return sm->is_run;
}


