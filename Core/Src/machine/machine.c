#include "main.h"
#include "stm32_lib/types.h"
#include "stm32_lib/buf.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/timers.h"
#include "stm32_lib/uart_debug.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "wait.h"

#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "periphery.h"
#include "ui_main.h"

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


static void test_keypad(struct disp *disp)
{
    int x, y;
    int key_width = 80;
    int key_height = 80;
    int key_num = 0;
    int left_indent = 20;
    int top_indent = 80;
    int key_indent = 15;

    struct text_style key_text_style = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3
    };

    for (x = 0; x < 3; x++) {
        for (y = 0; y < 4; y++) {
            key_num ++;
            char buf[3];
            disp_rect(disp, left_indent + x * (key_width + key_indent),
                      top_indent + y * (key_height + key_indent),
                      key_width, key_height,
                      1, GREEN);
            snprintf(buf, 3, "%d", key_num);
            disp_text(disp, buf,
                    left_indent + x * (key_width + key_indent) + key_width / 2 - 10,
                    top_indent + y * (key_height + key_indent) + key_height / 2 - 10,
                    &key_text_style);
        }
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

    printf("run 1 turn cross\r\n");
    stepper_motor_run(m->sm_cross_feed, 16, 100, MOVE_DOWN, 400 * 10 * 5);
}

void key_f(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_f\r\n");

    printf("run 1 turn cross\r\n");

/*    __HAL_TIM_SET_PRESCALER(&htim5, 10 - 1);
    htim5.Instance->EGR |= TIM_EGR_UG;
    __HAL_TIM_SET_COUNTER(&htim5, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim5, 1000);
    HAL_TIM_Base_Start_IT(&htim5);*/

    stepper_motor_run(m->sm_cross_feed, 16, 4000, MOVE_DOWN, 400 * 10 * 10);
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

    m->sm_cross_feed->gap --;
    printf("gap = %d\r\n", m->sm_longitudial_feed->gap);
}

void key_k(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_j\r\n");
    m->sm_cross_feed->gap ++;
    printf("gap = %d\r\n", m->sm_longitudial_feed->gap);
}

void key_l(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_l\r\n");
    printf("pos = %lu\r\n", stepper_motor_pos(m->sm_longitudial_feed));
}


// IRQ context
static void
sm_cross_high_speed_freq_changer(struct stepper_motor *sm, bool is_init)
{
    int freq = sm->freq;
    if (sm->freq < sm->target_freq) {
        if (sm->freq < 1000)
            freq += 20;
        else if (sm->freq < 2000)
            freq += 100;
        else if (sm->freq < 5000)
            freq += 200;
        else if (sm->freq < 8000)
            freq += 50;
        else if (sm->freq < 10000)
            freq += 20;
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
            freq += 100;
        else if (sm->freq < 2000)
            freq += 800;
        else if (sm->freq < 6000)
            freq += 500;
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
    int freq = sm->freq; // Текущая частота
    u32 pos;

    if (is_init) {
        // Длина участка разгона
        u32 accel_distance = (((sm->target_freq - sm->start_freq) *
                               (sm->target_freq - sm->start_freq))) /
                                      (sm->start_acceleration * 1000);
        sm->acceleration = sm->start_acceleration;
        printf("sm->acceleration = %lu\r\n", sm->acceleration);
        printf("sm->distance = %lu\r\n", sm->distance);
        printf("1 accel_distance = %lu\r\n", accel_distance);
        if (accel_distance > sm->distance / 2)
            accel_distance = sm->distance / 2;
        printf("2 accel_distance = %lu\r\n", accel_distance);
        sm->start_braking_point = sm->distance - accel_distance; // Точка начала торможения
        printf("sm->start_braking_point = %lu\r\n", sm->start_braking_point);
        sm->accel_prescaller_cnt = 0;
        return;
    }

    pos = stepper_motor_pos(sm); // Текущее положение двигателя

    if (pos >= sm->start_braking_point) { // Торможение
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 10) {
            sm->accel_prescaller_cnt = 0;
            sm->acceleration = ((freq * freq) / (sm->distance - pos)) / 1000;
            if (sm->acceleration > sm->start_acceleration)
                sm->acceleration = sm->start_acceleration;
        }

        freq -= sm->acceleration;
        if (freq <= sm->start_freq)
            freq = sm->start_freq;
        if (freq != sm->freq)
            stepper_motor_set_freq(sm, freq);
        return;
    }

    if (sm->freq < sm->target_freq) { // Разгон
        sm->accel_prescaller_cnt++;
        if (sm->accel_prescaller_cnt >= 10) {
            sm->accel_prescaller_cnt = 0;
            sm->acceleration = (sm->start_acceleration /
                                (sm->max_freq / (sm->target_freq - freq))) * 2;
        }

        freq += sm->acceleration; // Увеличиваем частоту линейно
        if (freq >= sm->target_freq) {
            freq = sm->target_freq;
            sm->accel_prescaller_cnt = 0;
        }
    }

    if (freq != sm->freq) {
        stepper_motor_set_freq(sm, freq);
    }
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
            stepper_motor_run(m->sm_cross_feed, 200, 10000,
                              MOVE_UP, NO_AUTO_STOP);
            m->is_last_move_up = TRUE;
        }
        else {
            stepper_motor_disable(m->sm_cross_feed);
            stepper_motor_stop(m->sm_cross_feed);
        }
    }

    if (is_button_changed(m->btn_down)) {
        if (is_button_held_down(m->btn_down)) {
            stepper_motor_enable(m->sm_cross_feed);
            stepper_motor_run(m->sm_cross_feed, 200, 10000,
                              MOVE_DOWN, NO_AUTO_STOP);
            m->is_last_move_up = FALSE;
        }
        else {
            stepper_motor_disable(m->sm_cross_feed);
            stepper_motor_stop(m->sm_cross_feed);
        }
    }

    if (is_button_changed(m->btn_left)) {
        if (is_button_held_down(m->btn_left)) {
            stepper_motor_enable(m->sm_longitudial_feed);
            stepper_motor_run(m->sm_longitudial_feed, 300, 15000,
                              MOVE_LEFT, NO_AUTO_STOP);
            m->is_last_move_left = TRUE;
        }
        else {
            stepper_motor_disable(m->sm_longitudial_feed);
            stepper_motor_stop(m->sm_longitudial_feed);
        }
    }

    if (is_button_changed(m->btn_right)) {
        if (is_button_held_down(m->btn_right)) {
            stepper_motor_enable(m->sm_longitudial_feed);
            stepper_motor_run(m->sm_longitudial_feed, 300, 15000,
                              MOVE_RIGHT, NO_AUTO_STOP);
            m->is_last_move_left = FALSE;
        }
        else {
            stepper_motor_disable(m->sm_longitudial_feed);
            stepper_motor_stop(m->sm_longitudial_feed);
        }
    }
}

static void move_button_low_speed_handler(void)
{
    struct machine *m = &machine;

    int freq = potentiometer_val(m->pm_move_speed) * 5;
    if (freq < 16)
        freq = 16;

    if (is_button_changed(m->btn_up)) {
        if (is_button_held_down(m->btn_up)) {
            if (is_switch_on(m->switch_gap_compensation) &&
                             (!m->is_last_move_up)) {
                stepper_motor_run(m->sm_cross_feed, 500, 1000,
                                  MOVE_UP, m->sm_cross_feed->gap);
                stepper_motor_wait_autostop(m->sm_cross_feed);
                m->is_last_move_up = TRUE;
                return;
            }

            m->is_last_move_up = TRUE;
            stepper_motor_run(m->sm_cross_feed, freq, freq,
                              MOVE_UP, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_cross_feed);
    }

    if (is_button_changed(m->btn_down)) {
        if (is_button_held_down(m->btn_down)) {
            if (is_switch_on(m->switch_gap_compensation) &&
                             m->is_last_move_up) {
                stepper_motor_run(m->sm_cross_feed, 500, 1000,
                                  MOVE_DOWN, m->sm_cross_feed->gap);
                stepper_motor_wait_autostop(m->sm_cross_feed);
                m->is_last_move_up = FALSE;
                return;
            }

            m->is_last_move_up = FALSE;
            stepper_motor_run(m->sm_cross_feed, freq, freq,
                              MOVE_DOWN, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_cross_feed);
    }

    if (is_button_changed(m->btn_left)) {
        if (is_button_held_down(m->btn_left)) {
            if (is_switch_on(m->switch_gap_compensation) &&
                             (!m->is_last_move_left)) {
                stepper_motor_run(m->sm_longitudial_feed, 2000, 5000,
                                  MOVE_LEFT, m->sm_longitudial_feed->gap);
                stepper_motor_wait_autostop(m->sm_longitudial_feed);
                m->is_last_move_left = TRUE;
                return;
            }

            m->is_last_move_left = TRUE;
            stepper_motor_run(m->sm_longitudial_feed, freq, freq,
                              MOVE_LEFT, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_longitudial_feed);
    }

    if (is_button_changed(m->btn_right)) {
        if (is_button_held_down(m->btn_right)) {
            if (is_switch_on(m->switch_gap_compensation) &&
                             m->is_last_move_left) {
                stepper_motor_run(m->sm_longitudial_feed, 2000, 5000,
                                  MOVE_RIGHT, m->sm_longitudial_feed->gap);
                stepper_motor_wait_autostop(m->sm_longitudial_feed);
                m->is_last_move_left = FALSE;
                return;
            }

            m->is_last_move_left = FALSE;
            stepper_motor_run(m->sm_longitudial_feed, freq, freq,
                              MOVE_RIGHT, NO_AUTO_STOP);
        }
        else
            stepper_motor_stop(m->sm_longitudial_feed);
    }
}

static void program_idle_handler(void)
{
    struct machine *m = &machine;

    if (is_button_changed(m->switch_high_speed)) {
        if (is_switch_on(m->switch_high_speed))
            set_high_speed();
        else
            set_low_speed();
    }

    if (is_switch_on(m->switch_high_speed))
        move_button_high_speed_handler();
    else
        move_button_low_speed_handler();

    if (is_potentiometer_changed(m->pm_move_speed)) {
        u16 val = potentiometer_val(m->pm_move_speed);
        printf("potentiometer = %d\r\n", val);
    }
}

static void main_thread(void *priv)
{
    struct machine *m = (struct machine *)priv;
    memset(m, 0, sizeof *m);

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

    for(;;) {
        yield();

        if (!is_button_held_down(m->switch_run)) {
            program_idle_handler();
        }

    }
}


void machine_init(void)
{
    machine.tid = thread_register("main_thread", 2048,
                                  main_thread, &machine);
}








