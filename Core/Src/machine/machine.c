#include <stdlib.h>
#include "main.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/uart_debug.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "abs_position.h"
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "periphery.h"
#include "ui_main.h"
#include "ui_button.h"
#include "ui_move_to.h"
#include "msg_rus.h"
#include "mode_idle.h"
#include "mode_cut.h"
#include "mode_thread.h"


struct machine machine;
extern UART_HandleTypeDef huart1;


void halt(char *reason)
{
    panic(reason);
}


void panic(char *cause)
{
    struct machine *m = &machine;
    stepper_motor_stop(m->sm_cross);
    stepper_motor_stop(m->sm_longitudial);
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

#if MACHINE_DEBUG
void key_d(void *priv)
{
	struct machine *m = (struct machine *)priv;
    printf("key_d\r\n");

    printf("run 1 turn cross\r\n");
    stepper_motor_run(m->sm_cross, 0, 10000, MOVE_UP, 75000);
}

void key_f(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_f\r\n");

    spindle_power_on();
}

void key_g(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_g\r\n");
    spindle_power_off();
}

void key_j(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_j\r\n");

    stepper_motor_run(m->sm_cross, 18, 18, MOVE_UP, 1);
}

void key_k(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_kj\r\n");

    stepper_motor_run(m->sm_cross, 18, 18, MOVE_DOWN, 1);
}

void key_l(void *priv)
{
    struct machine *m = &machine;
    struct mode_thread *mt = &m->mt;
    struct mode_thread_settings *mt_settings = &mt->settings;
    int curr_pos;
    int error_distance;
    int distance;
    int entry_raw_angle;

    curr_pos = abs_longitudal_curr_tool(m->ap);
    error_distance = calc_longitudal_error(mt, mt->start_longitudal_pos, curr_pos);
    distance = mt_settings->length + error_distance;

    // вычисление угла входа
    int xa = curr_pos;
    int xb = mt_settings->longitudal_start + mt_settings->thread_offset;
    entry_raw_angle =
            ((((xa - xb) * 3000)/mt->step_size) + mt_settings->spindle_start) % 3000;
    if (entry_raw_angle < 0)
        entry_raw_angle = 3000 + entry_raw_angle;

    printf("xa = %d\r\n", xa);
    printf("xb = %d\r\n", xb);
    printf("mt_settings->length = %d\r\n", mt_settings->length);
    printf("mt_settings->longitudal_start = %d\r\n", mt_settings->longitudal_start);
    printf("mt_settings->thread_offset = %d\r\n", mt_settings->thread_offset);
    printf("mt_settings->spindle_start = %d\r\n", mt_settings->spindle_start);
    printf("spindle_raw_angle = %d\r\n", spindle_raw_angle());
    printf("error_distance = %d\r\n", error_distance);
    printf("curr_pos = %d\r\n", curr_pos);
    printf("distance = %d\r\n", distance);
    printf("entry_raw_angle = %d\r\n", entry_raw_angle);
    printf("mt->step_size = %d\r\n", mt->step_size);
    printf("((xa - xb)/mt->step_size) = %d\r\n", ((xa - xb)/mt->step_size));

}
#endif // MACHINE_DEBUG

// mm per minute
int cut_speed_calculate(int diameter, int rpm)
{
    int circumference = (3141 * diameter) / 1000; // 3141 это Pi 3.141*1000
    return (circumference * rpm) / 1000;
}


int calc_cross_to_target(int curr_pos, int target_pos, bool *dir)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_cross;

    int offset;
    int distance;

    if (target_pos % sm->resolution)
        target_pos = (target_pos / sm->resolution) * sm->resolution;

    if (curr_pos == target_pos)
        return 0;

    offset = target_pos - curr_pos;
    distance = abs(offset);
    if (dir) {
        *dir = MOVE_DOWN;
        if (target_pos > curr_pos)
            *dir = MOVE_UP;
        if (m->ap->is_cross_inc_down)
            *dir = !*dir;
    }
    return distance;
}

int calc_cross_position(int position, int distance, bool dir)
{
    struct machine *m = &machine;

    if (dir == MOVE_UP) {
        if (m->ap->is_cross_inc_down)
            return position - distance;
        return position + distance;
    }

    if (m->ap->is_cross_inc_down)
        return position + distance;
    return position - distance;
}

int calc_longitudal_position(int position, int distance, bool dir)
{
    struct machine *m = &machine;

    if (dir == MOVE_RIGHT) {
        if (m->ap->is_longitudal_inc_left)
            return position - distance;
        return position + distance;
    }

    if (m->ap->is_longitudal_inc_left)
        return position + distance;
    return position - distance;
}

int calc_longitudal_to_target(int curr_pos, int target_pos, bool *dir)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_longitudial;

    int offset;
    int distance;

    if (target_pos % sm->resolution)
        target_pos = (target_pos / sm->resolution) * sm->resolution;

    if (curr_pos == target_pos)
        return 0;

    offset = target_pos - curr_pos;
    distance = abs(offset);
    if (dir) {
        *dir = MOVE_LEFT;
        if (target_pos > curr_pos)
            *dir = MOVE_RIGHT;
        if (m->ap->is_longitudal_inc_left)
            *dir = !*dir;
    }
    return distance;
}



int cross_move_to(int target_pos, bool is_accurate,
                  void (*periodic_cb)(void *), void *priv)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_cross;
    int curr_pos;

    int distance;
    bool dir;
    m->is_busy = TRUE;

    while (1) {
        yield();
        curr_pos = abs_cross_curr_tool(m->ap);
        distance = calc_cross_to_target(curr_pos, target_pos, &dir);
        if (!distance) {
            m->is_busy = FALSE;
            return 0;
        }
        stepper_motor_run(sm, 0, 0, dir, distance);
        while (is_stepper_motor_run(sm)) {
            yield();
            if (periodic_cb)
                periodic_cb(priv);

            if (is_button_clicked(m->btn_enc)) {
                stepper_motor_stop(sm);
                m->is_busy = FALSE;
                return -1;
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

int longitudal_move_to(int target_pos, bool is_accurate, int max_freq,
                       void (*periodic_cb)(void *), void *priv)
{
    struct machine *m = &machine;
    struct stepper_motor *sm = m->sm_longitudial;

    int distance;
    bool dir;
    if (!max_freq)
        max_freq = sm->max_freq;
    int freq = max_freq;
    int attempts = 0;
    m->is_busy = TRUE;

    while (1) {
        yield();
        int curr_pos = abs_longitudal_curr_tool(m->ap);
        distance = calc_longitudal_to_target(curr_pos, target_pos, &dir);
        if (distance <= 5) {
            m->is_busy = FALSE;
            return 0;
        }

        if (attempts == 1)
            freq /= 4;

        if (attempts == 2)
            freq /= 4;

        if (attempts == 3)
            freq /= 4;

        if (attempts == 4)
            freq /= 4;

        if (attempts == 5)
            freq /= 4;

        if (attempts >= 7) {
            attempts = 0;
            freq = max_freq;
        }
        attempts ++;

        if (freq < sm->min_freq)
            freq = sm->min_freq;

        printf("freq = %d\r\n", freq);

        stepper_motor_run(sm, 500, freq, dir, distance);
        while (is_stepper_motor_run(sm)) {
            yield();
            if (periodic_cb)
                periodic_cb(priv);

            if (is_button_clicked(m->btn_enc)) {
                stepper_motor_stop(sm);
                m->is_busy = FALSE;
                return -1;
            }
        }
        if (!is_accurate) {
            m->is_busy = FALSE;
            return 0;
        }
        sleep(300);
    }
    m->is_busy = FALSE;
    return -1;
}



// IRQ context
void sm_cross_normal_acceleration_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    int pos;
    static int cnt = 0;

    if (is_init) {
        cnt = 0;
        // Length of acceleration section
        sm->start_acceleration = 1;
        u32 accel_distance = (((sm->target_freq - sm->start_freq) *
                               (sm->target_freq - sm->start_freq))) /
                                      (sm->start_acceleration * 50 * 1000);
        sm->acceleration = sm->start_acceleration;

        if (accel_distance > sm->distance_um / 2)
            accel_distance = sm->distance_um / 2;
        // braking start point
        sm->start_braking_point = sm->distance_um - accel_distance;
        sm->accel_prescaller_cnt = 0;
        return;
    }

    pos = stepper_motor_pos(sm);

    if (cnt == 500) {
        if (pos < sm->start_braking_point) {
            sm->start_acceleration = 10;
            sm->acceleration = 10;
        }
    }
    cnt++;

    if (pos >= sm->start_braking_point) { // braking
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 1) {
            sm->accel_prescaller_cnt = 0;
            if ((sm->distance_um - pos) > 0)
                sm->acceleration = ((freq * freq) /
                                        (sm->distance_um - pos)) / 1000;
            else
                sm->acceleration = sm->start_acceleration * 5;
            if (sm->acceleration > sm->start_acceleration * 5)
                sm->acceleration = sm->start_acceleration * 5;
        }

        freq -= sm->acceleration * 5;
        if (freq <= sm->start_freq)
            freq = sm->start_freq;
        if (freq != sm->freq) {
            stepper_motor_set_freq(sm, freq);
        }
        return;
    }

    if (sm->freq < sm->target_freq) { // acceleration
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 10) {
            sm->accel_prescaller_cnt = 0;
            sm->acceleration = (sm->start_acceleration /
                                (sm->max_freq / (sm->target_freq - freq)));
            if (!sm->acceleration)
                sm->acceleration = 1;
        }


        freq += sm->acceleration;
        if (freq >= sm->target_freq) {
            freq = sm->target_freq;
            sm->accel_prescaller_cnt = 0;
        }
    }

    if (freq > sm->target_freq)
        freq = sm->target_freq;

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}

// IRQ context
void sm_longitudal_normal_acceleration_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    int pos;
    static int cnt = 0;

    if (is_init) {
        cnt = 0;
        // Length of acceleration section
        sm->start_acceleration = 1;
        u32 accel_distance = (((sm->target_freq - sm->start_freq) *
                               (sm->target_freq - sm->start_freq))) /
                                      (sm->start_acceleration * 10 * 1000);
        sm->acceleration = sm->start_acceleration;

        if (accel_distance > sm->distance_um / 2)
            accel_distance = sm->distance_um / 2;
        // braking start point
        sm->start_braking_point = sm->distance_um - accel_distance;
        sm->accel_prescaller_cnt = 0;
        return;
    }

    pos = stepper_motor_pos(sm);

    if (cnt == 500) {
        if (pos < sm->start_braking_point) {
            sm->start_acceleration = 10;
            sm->acceleration = 10;
        }
    }
    cnt++;

    if (pos >= sm->start_braking_point) { // braking
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 1) {
            sm->accel_prescaller_cnt = 0;
            if ((sm->distance_um - pos) > 0)
                sm->acceleration = ((freq * freq) /
                                        (sm->distance_um - pos)) / 1000;
            else
                sm->acceleration = sm->start_acceleration * 2;
            if (sm->acceleration > sm->start_acceleration * 2)
                sm->acceleration = sm->start_acceleration * 2;
        }

        freq -= sm->acceleration * 2;
        if (freq <= sm->start_freq)
            freq = sm->start_freq;
        if (freq != sm->freq) {
            stepper_motor_set_freq(sm, freq);
        }
        return;
    }

    if (sm->freq < sm->target_freq) { // acceleration
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 10) {
            sm->accel_prescaller_cnt = 0;
            sm->acceleration = (sm->start_acceleration /
                                (sm->max_freq / (sm->target_freq - freq)));
            if (!sm->acceleration)
                sm->acceleration = 1;
        }


        freq += sm->acceleration;
        if (freq >= sm->target_freq) {
            freq = sm->target_freq;
            sm->accel_prescaller_cnt = 0;
        }
    }

    if (freq > sm->target_freq)
        freq = sm->target_freq;

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}


void set_normal_acceleration(void)
{
    struct machine *m = &machine;
    stepper_motor_enable(m->sm_cross);
    stepper_motor_enable(m->sm_longitudial);
    stepper_motor_set_freq_changer_handler(m->sm_cross,
                                      sm_cross_normal_acceleration_changer);
    stepper_motor_set_freq_changer_handler(m->sm_longitudial,
                                      sm_longitudal_normal_acceleration_changer);
    m->sm_cross->is_allow_run_out = FALSE;
    m->sm_longitudial->is_allow_run_out = FALSE;
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
            freq += 25;
        else if (sm->freq < 5000)
            freq += 30;
        else if (sm->freq < 8000)
            freq += 20;
        else if (sm->freq < 10000)
            freq += 10;
        else
            freq ++;
        if (freq > sm->target_freq)
            freq = sm->target_freq;
    }

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}

// IRQ context
void
sm_longitudial_high_acceleration_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    if (sm->freq < sm->target_freq) {
        if (sm->freq < 1000)
            freq += 10;
        else if (sm->freq < 2000)
            freq += 60;
        else if (sm->freq < 6000)
            freq += 40;
        else if (sm->freq < 8000)
            freq += 20;
        else
            freq += 10;
        if (freq > sm->target_freq)
            freq = sm->target_freq;
    }

    if (freq != sm->freq)
        stepper_motor_set_freq(sm, freq);
}


void set_high_acceleration(void)
{
    struct machine *m = &machine;
    stepper_motor_disable(m->sm_cross);
    stepper_motor_disable(m->sm_longitudial);
    stepper_motor_set_freq_changer_handler(m->sm_cross,
                                      sm_cross_high_acceleration_changer);
    stepper_motor_set_freq_changer_handler(m->sm_longitudial,
                                sm_longitudial_high_acceleration_changer);
    m->sm_cross->is_allow_run_out = TRUE;
    m->sm_longitudial->is_allow_run_out = TRUE;
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




static void display_status_handler(void)
{
    struct machine *m = &machine;
    struct ui_status *us = &m->ui_stat;
    struct abs_position *ap = m->ap;

    if (abs_pos_tool(m->ap) != (int)us->tool_num->priv) {
        us->tool_num->priv = (void *)abs_pos_tool(m->ap);
        ui_item_show(us->tool_num);
    }

    ui_item_show(us->cross_pos);
    ui_item_show(us->longitudal_pos);
    ui_item_show(us->spindle_speed);
    ui_item_show(us->cross_speed);
    ui_item_show(us->longitudal_speed);
    ui_item_show(us->feed_rate);
    ui_item_show(us->cut_speed);

    if (ap->is_cross_inc_down != (bool)us->cross_pos_dir->priv) {
        us->cross_pos_dir->priv = (void *)ap->is_cross_inc_down;
        ui_item_update(us->cross_pos_dir);
    }

    if (ap->is_longitudal_inc_left != (bool)us->longitudal_pos_dir->priv) {
        us->longitudal_pos_dir->priv = (void *)ap->is_longitudal_inc_left;
        ui_item_update(us->longitudal_pos_dir);
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
    button_reset(m->btn_ok);
}


static void monitoring_thread(void *priv)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    for(;;) {
        int val;
        yield();
        touch_handler(m->disp1->touch);
        abs_position_update(m->ap);
        display_status_handler();

        if (is_button_changed(m->switch_touch_lock)) {
            if (is_switch_on(m->switch_touch_lock))
                touch_enable(m->disp1->touch);
            else
                touch_disable(m->disp1->touch);
        }

        val = panel_encoder_val();
        if (val) {
            if (mc_settings->feed_rate <= 20) {
                mc_settings->feed_rate += val;
                if (mc_settings->feed_rate <= 1)
                    mc_settings->feed_rate = 1;
            }
            else
                mc_settings->feed_rate += val * 10;

            if (mc_settings->feed_rate > 5000)
                mc_settings->feed_rate = 5000;
        }

        potentiometer_handler(m->pm_move_speed);
    }
}

static void program_run(void)
{
    struct machine *m = &machine;

    switch (m->prog) {
    case PROG_THREAD_LEFT:
    case PROG_THREAD_RIGHT:
        mode_thread_run();
        break;
    default:
        mode_cut_run();
    }
}

static void main_thread(void *priv)
{
    struct machine *m = &machine;
    struct mode_cut *mc = &m->mc;
    struct mode_cut_settings *mc_settings = &mc->settings;

    mc_settings->feed_rate = 100;
    mc_settings->longitudal_distance = 50 * 1000;
    mc_settings->target_diameter = 42 * 1000;
    mc_settings->cross_distance = 21 * 1000;
    mc_settings->cut_depth_step = 1500;

    uart_debug_plug(&huart1);
    uart_dbg_key_register("os_status", 's', dbg_os_stat, m);

#if MACHINE_DEBUG
    uart_dbg_key_register("d_key", 'd', key_d, m);
    uart_dbg_key_register("f_key", 'f', key_f, m);
    uart_dbg_key_register("g_key", 'g', key_g, m);
    uart_dbg_key_register("j_key", 'j', key_j, m);
    uart_dbg_key_register("k_key", 'k', key_k, m);
    uart_dbg_key_register("l_key", 'l', key_l, m);
#endif // MACHINE_DEBUG

    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    periphery_init();

    HAL_TIM_Base_Start_IT(&htim4); // TODO - panel encoder
    printlog("Machine ver: %s\r\n", BUILD_VERSION);

    ui_main_start();

    ui_status_init();

    thread_register("monitoring_thread", 1500,
                    monitoring_thread, NULL);

    if (is_switch_on(m->switch_run)) {
        ui_message_show(msg_switch_run_needs_off, MSG_NORM);
        while(is_switch_on(m->switch_run)) {
            yield();
        }
        ui_message_hide();
    }

    for(;;) {
        yield();

        if (is_switch_on(m->switch_run)) {
            ui_main_lock();
            program_run();
        } else {
            spindle_power_on();
            ui_main_unlock();
            mode_idle_run();
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








