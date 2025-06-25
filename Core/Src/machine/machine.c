#include <stdlib.h>
#include "main.h"
#include "stm32_lib/types.h"
#include "stm32_lib/buf.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/timers.h"
#include "stm32_lib/uart_debug.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "abs_position.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "periphery.h"
#include "ui_main.h"
#include "ui_move_to.h"

struct machine machine;
extern UART_HandleTypeDef huart1;


void halt(char *reason)
{
    panic(reason);
}


void panic(char *cause)
{
    printlog("PANIC: %s\r\n", cause);
    for (;;) {
        printlog("PANIC: %s\r\n", cause);
        kmem_print_stat();
        HAL_Delay(3000);
    }
}

static void dbg_os_stat(void *priv)
{
    printlog("LT10M board ver: %s\r\n", BUILD_VERSION);
    threads_stat_print();
    timers_print();
    kmem_print_stat();
    printlog("timer: %lu\r\n", osKernelGetTickCount());
}

void key_d(void *priv)
{
	struct machine *m = (struct machine *)priv;
    printf("key_d\r\n");

    printf("run 1 turn longitudal\r\n");
    stepper_motor_run(m->sm_longitudial_feed, 0, 15000, MOVE_RIGHT, 5);
}

void key_f(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_f\r\n");

    printf("run 1 turn cross\r\n");

/*    __HAL_TIM_SET_PRESCALER(&htim5, 10 - 1);
    htim5.Instance->EGR |= TIM_EGR_UG;
    __HAL_TIM_SET_COUNTER(&htim5, 0);
    __HAL_TIM_SET_AUTORELOAD(f&htim5, 1000);
    HAL_TIM_Base_Start_IT(&htim5);*/

    stepper_motor_run(m->sm_cross_feed, 18, 4000, MOVE_DOWN, 40000);
}

void key_g(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_g\r\n");
    printf("stop");
    stepper_motor_stop(m->sm_cross_feed);
    stepper_motor_stop(m->sm_longitudial_feed);
}

void key_j(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_j\r\n");

//    m->ap->raw.cross ++;
    m->ap->raw.longitudal ++;
//    printf("m->ap->raw.longitudal = %lu\r\n", m->ap->raw.longitudal);
}

void key_k(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_j\r\n");
    m->ap->raw.cross ++;
  //  m->ap->raw.longitudal --;
}

void key_l(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_l\r\n");

    abs_position_update(m->ap);
    return;
    printf("reset\r\n");
    __HAL_TIM_SET_COUNTER(&htim12, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim12, 5000);
    HAL_TIM_Base_Start_IT(&htim12);

}

int cross_move_to(int pos, bool is_accurate)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_cross_feed;

    int distance;
    int offset;
    bool dir;
    m->is_busy = TRUE;

    if (pos % sm->resolution)
        pos = (pos / sm->resolution) * sm->resolution;

    while (1) {
        yield();
        int curr_pos = abs_cross_curr_tool(m->ap);
        if (curr_pos == pos) {
            m->is_busy = FALSE;
            return 0;
        }

        offset = pos - curr_pos;
        distance = abs(offset);
        dir = MOVE_DOWN;
        if (pos > curr_pos)
            dir = MOVE_UP;
        if (m->ap->is_cross_inc_down)
            dir = !dir;
//        if (offset < 0)
  //          dir = !dir;

        stepper_motor_run(sm, 0, 0, dir, distance);
        while (is_stepper_motor_run(sm)) {
            yield();
            if (is_button_clicked(m->btn_enc)) {
                stepper_motor_stop(sm);
                m->is_busy = FALSE;
                return 0;
            }
        }
        if (!is_accurate) {
            m->is_busy = FALSE;
            return 0;
        }
        sleep(50);
    }
    m->is_busy = FALSE;
    return -1;
}

int longitudal_move_to(int pos, bool is_accurate)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_longitudial_feed;

    int distance;
    bool dir;
    int speed = sm->max_freq;
    int attempts = 0;
    m->is_busy = TRUE;

    if (pos % sm->resolution)
        pos = (pos / sm->resolution) * sm->resolution;

    while (1) {
        yield();
        int curr_pos = abs_longitudal_curr_tool(m->ap);
        printf("pos = %d, curr_pos = %d\r\n", pos, curr_pos);
        if (curr_pos == pos) {
            printf("SUCCESS\r\n");
            m->is_busy = FALSE;
            return 0;
        }

        distance = abs(pos - curr_pos);
        printf("distance = %d\r\n", distance);

        dir = MOVE_LEFT;
        if (pos > curr_pos)
            dir = MOVE_RIGHT;
        if (m->ap->is_longitudal_inc_left)
            dir = !dir;

        if (attempts == 2)
            speed /= 2;

        if (attempts == 3)
            speed /= 2;

        if (attempts >= 5) {
            attempts = 0;
            speed = sm->max_freq;
        }
        printf("attempts = %d\r\n", attempts);
        printf("dir = %s\r\n", dir ? "LEFT" : "RIGHT");
        printf("speed = %d\r\n", speed);

        stepper_motor_run(sm, 1000, speed, dir, distance);
        while (is_stepper_motor_run(sm)) {
            yield();
            if (is_button_clicked(m->btn_enc)) {
                stepper_motor_stop(sm);
                m->is_busy = FALSE;
                return 0;
            }
        }
        if (!is_accurate) {
            m->is_busy = FALSE;
            return 0;
        }
        sleep(300);
        attempts ++;
    }
    m->is_busy = FALSE;
    return -1;
}


// IRQ context
static void
sm_cross_high_speed_freq_changer(struct stepper_motor *sm, bool is_init)
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
sm_longitudial_high_speed_freq_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    if (sm->freq < sm->target_freq) {
        if (sm->freq < 1000)
            freq += 50;
        else if (sm->freq < 2000)
            freq += 400;
        else if (sm->freq < 6000)
            freq += 250;
        else if (sm->freq < 12000)
            freq += 100;
        else
            freq += 20;
        if (freq > sm->target_freq)
            freq = sm->target_freq;
    }

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}

// IRQ context
static void sm_low_speed_freq_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    int pos;

    if (is_init) {
        // Длина участка разгона
        sm->start_acceleration = 5;
        u32 accel_distance = (((sm->target_freq - sm->start_freq) *
                               (sm->target_freq - sm->start_freq))) /
                                      (sm->start_acceleration * 1000);
        sm->acceleration = sm->start_acceleration;

        if (accel_distance > sm->distance_um / 2)
            accel_distance = sm->distance_um / 2;
        sm->start_braking_point = sm->distance_um - accel_distance; // Точка начала торможения
        sm->accel_prescaller_cnt = 0;
        return;
    }

    pos = stepper_motor_pos(sm);

    if (pos >= sm->start_braking_point) { // Торможение
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 1) {
            sm->accel_prescaller_cnt = 0;
            if ((sm->distance_um - pos) > 0)
                sm->acceleration = ((freq * freq) / (sm->distance_um - pos)) / 1000;
            else
                sm->acceleration = sm->start_acceleration;
            if (sm->acceleration > sm->start_acceleration)
                sm->acceleration = sm->start_acceleration;
        }

        freq -= sm->acceleration;
        if (freq <= sm->start_freq)
            freq = sm->start_freq;
        if (freq != sm->freq) {
            stepper_motor_set_freq(sm, freq);
        }
        return;
    }

    if (sm->freq < sm->target_freq) { // Разгон
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 10) {
            sm->accel_prescaller_cnt = 0;
            sm->acceleration = (sm->start_acceleration /
                                (sm->max_freq / (sm->target_freq - freq)));
        }

        freq += sm->acceleration;
        if (freq >= sm->target_freq) {
            freq = sm->target_freq;
            sm->accel_prescaller_cnt = 0;
        }
    }

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}


static void set_low_speed(void)
{
    struct machine *m = &machine;
    stepper_motor_enable(m->sm_cross_feed);
    stepper_motor_enable(m->sm_longitudial_feed);
    stepper_motor_set_freq_changer_handler(m->sm_cross_feed,
                                      sm_low_speed_freq_changer);
    stepper_motor_set_freq_changer_handler(m->sm_longitudial_feed,
                                sm_low_speed_freq_changer);
}

static void set_high_speed(void)
{
    struct machine *m = &machine;
    stepper_motor_disable(m->sm_cross_feed);
    stepper_motor_disable(m->sm_longitudial_feed);
    stepper_motor_set_freq_changer_handler(m->sm_cross_feed,
                                      sm_cross_high_speed_freq_changer);
    stepper_motor_set_freq_changer_handler(m->sm_longitudial_feed,
                                sm_longitudial_high_speed_freq_changer);
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
        stepper_motor_set_freq(m->sm_cross_feed, cross_freq);
        stepper_motor_set_freq(m->sm_longitudial_feed, longitudal_freq);
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

int cross_up_new_position(int distance)
{
    struct machine *m = &machine;
    int curr_pos = abs_cross_curr_tool(m->ap);
    if (m->ap->is_cross_inc_down)
        return curr_pos - distance;
    return curr_pos + distance;
}

int cross_down_new_position(int distance)
{
    struct machine *m = &machine;
    int curr_pos = abs_cross_curr_tool(m->ap);
    if (m->ap->is_cross_inc_down)
        return curr_pos + distance;
    return curr_pos - distance;
}

int longitudal_left_new_position(int distance)
{
    struct machine *m = &machine;
    int curr_pos = abs_longitudal_curr_tool(m->ap);
    if (m->ap->is_longitudal_inc_left)
        return curr_pos + distance;
    return curr_pos - distance;
}

int longitudal_right_new_position(int distance)
{
    struct machine *m = &machine;
    int curr_pos = abs_longitudal_curr_tool(m->ap);
    if (m->ap->is_longitudal_inc_left)
        return curr_pos - distance;
    return curr_pos + distance;
}


static void move_buttons_move_to_handler(void)
{
    struct machine *m = &machine;
    int pos;
    int move_step = ui_move_to_step();

    if (is_button_clicked(m->btn_up)) {
        pos = cross_up_new_position(move_step);
        ui_moveto_blink_up_arrow();
        cross_move_to(pos, TRUE);
        ui_moveto_blink_stop();
    }

    if (is_button_clicked(m->btn_down)) {
        pos = cross_down_new_position(move_step);
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

static void program_idle_handler(void)
{
    struct machine *m = &machine;
    void *msg;
    int position;

    if (is_button_changed(m->switch_high_speed)) {
        if (is_switch_on(m->switch_high_speed))
            set_high_speed();
        else
            set_low_speed();
    }

    if (is_switch_on(m->switch_high_speed))
        move_button_high_speed_handler();
    else if (is_switch_on(m->switch_move_to))
        move_buttons_move_to_handler();
    else
        move_button_low_speed_handler();

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


void display_status_handler(void)
{
    struct machine *m = &machine;
    struct ui_status *us = &m->ui_stat;
    struct abs_position *ap = m->ap;
    int val;

    if (abs_pos_tool(m->ap) != us->curr_tool_num) {
        us->curr_tool_num = abs_pos_tool(m->ap);
        ui_status_tool_update();
    }

    val = abs_cross_curr_tool(ap);
    if (val != us->cross_pos) {
        char buf[10];
        int len1, len2;
        len1 = sprintf(buf, "%.3f", (float)us->cross_pos / 1000);
        len2 = sprintf(buf, "%.3f", (float)val / 1000);
        us->cross_pos = val;
        ui_status_cross_update(len1 != len2);
    }

    val = abs_longitudal_curr_tool(ap);
    if (val != us->longitudal_pos) {
        char buf[10];
        int len1, len2;
        len1 = sprintf(buf, "%.3f", (float)us->longitudal_pos / 1000);
        len2 = sprintf(buf, "%.3f", (float)val / 1000);
        us->longitudal_pos = val;
        ui_status_longitudal_update(len1 != len2);
    }

    if (ap->is_cross_inc_down != us->is_cross_inc_up) {
        us->is_cross_inc_up = ap->is_cross_inc_down;
        ui_status_cross_dir_update();
    }

    if (ap->is_longitudal_inc_left != us->is_longitudal_inc_right) {
        us->is_longitudal_inc_right = ap->is_longitudal_inc_left;
        ui_status_longitudal_dir_update();
    }
}

void buttons_reset(void)
{
    struct machine *m = &machine;
    button_reset(m->btn_up);
    button_reset(m->btn_down);
    button_reset(m->btn_left);
    button_reset(m->btn_right);
    button_reset(m->btn_enc);
}


static void monitoring_thread(void *priv)
{
    struct machine *m = &machine;
    for(;;) {
        yield();
        if (!m->is_busy)
            touch_handler(m->disp1->touch); // TODO
        abs_position_update(m->ap);
        display_status_handler();

        if (is_button_changed(m->switch_touch_lock)) {
            if (is_switch_on(m->switch_touch_lock))
                touch_enable(m->disp1->touch);
            else
                touch_disable(m->disp1->touch);
        }
    }
}


static void main_thread(void *priv)
{
    struct machine *m = &machine;

    uart_debug_plug(&huart1);
    uart_dbg_key_register("os_status", 's', dbg_os_stat, m);

    uart_dbg_key_register("d_key", 'd', key_d, m);
    uart_dbg_key_register("f_key", 'f', key_f, m);
    uart_dbg_key_register("g_key", 'g', key_g, m);
    uart_dbg_key_register("j_key", 'j', key_j, m);
    uart_dbg_key_register("k_key", 'k', key_k, m);
    uart_dbg_key_register("l_key", 'l', key_l, m);

    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    periphery_init();

    HAL_TIM_Base_Start_IT(&htim4); // TODO - panel encoder
    printlog("Machine ver: %s\r\n", BUILD_VERSION);

    ui_main_start();

    if (is_switch_on(m->switch_high_speed))
        set_high_speed();
    else
        set_low_speed();

    ui_status_show();

    thread_register("monitoring_thread", 1500,
                    monitoring_thread, NULL);

    u32 prev_val = 0;
    for(;;) {
        yield();

        if (!is_button_held_down(m->switch_run)) {
            program_idle_handler();

            u32 val = __HAL_TIM_GET_COUNTER(&htim12);
            if (prev_val != val) {
                printf("enc: %lu\r\n", val * 5);
            }
            prev_val = val;
        }

    }
}

void machine_init(void)
{
    struct machine *m = &machine;
    memset(m, 0, sizeof *m);

    m->machine_tid = thread_register("main_thread", 2048,
                                     main_thread, NULL);
}








