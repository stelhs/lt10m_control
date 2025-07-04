/*
 * periphery.c
 *
 */
#include <stdlib.h>
#include "main.h"
#include "machine.h"
#include "periphery.h"

#include "stm32_lib/gpio.h"
#include "stm32_lib/buttons.h"
#include "stm32_lib/kref_alloc.h"
#include "spi.h"
#include "usart.h"
#include "tim.h"
#include "disp_mipi_dcs.h"
#include "stepper_motor.h"
#include "potentiometer.h"
#include "touch_xpt2046.h"
#include "abs_position.h"

extern struct machine machine;

static struct gpio gpio_disp1_spi_cs = {SPI_TFT_CS_GPIO_Port, SPI_TFT_CS_Pin};
static struct gpio gpio_disp2_spi_cs = {SPI_TFT2_CS_GPIO_Port, SPI_TFT2_CS_Pin};
static struct gpio gpio_disp_reset = {TFT_RESET_GPIO_Port,
                                      TFT_RESET_Pin};
static struct gpio gpio_disp_dc_rs = {TFT_DC_RS_GPIO_Port,
                                      TFT_DC_RS_Pin};
static struct gpio gpio_touch_spi_cs = {SPI_TOUCH_CS_GPIO_Port,
                                        SPI_TOUCH_CS_Pin};
static struct gpio gpio_longitudal_feed_en = {LONGITUDAL_FEED_EN_GPIO_Port,
                                              LONGITUDAL_FEED_EN_Pin};
static struct gpio gpio_longitudal_feed_dir = {LONGITUDAL_FEED_DIR_GPIO_Port,
                                              LONGITUDAL_FEED_DIR_Pin};
static struct gpio gpio_cross_feed_en = {CROSS_FEED_EN_GPIO_Port,
                                         CROSS_FEED_EN_Pin};
static struct gpio gpio_cross_feed_dir = {CROSS_FEED_DIR_GPIO_Port,
                                          CROSS_FEED_DIR_Pin};
static struct gpio gpio_abs_pos_cs = {ABS_POS_CS_GPIO_Port,
                                          ABS_POS_CS_Pin};
static struct gpio gpio_speaker = {SPEAKER_GPIO_Port, SPEAKER_Pin};

struct beep_blinker {
    u32 cnt;
    u32 target;
};
static void beep_blinker_cb(void *priv)
{
    struct beep_blinker *bb = (struct beep_blinker *)priv;

    if (gpio_read(&gpio_speaker))
        gpio_down(&gpio_speaker);
    else
        gpio_up(&gpio_speaker);

    bb->cnt ++;
    if (bb->target && bb->cnt >= bb->target)
        beep_blink_stop();
}

static struct timer_worker *beep_blinker;
void beep_blink_start(int beep_duration, int period, int count)
{
    struct beep_blinker bb = {
            .cnt = 0,
            .target = count
    };
    beep_blinker = set_periodic("beep_blinker", period, beep_blinker_cb, &bb,
                                sizeof (struct beep_blinker));
}

void beep_blink_stop(void)
{
    kmem_deref(&beep_blinker);
    gpio_down(&gpio_speaker);
}

static void panel_encoder_init(void)
{
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    HAL_TIM_Base_Start_IT(&htim4);
}

int panel_encoder_val(void)
{
    int val = (s16)__HAL_TIM_GET_COUNTER(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    return val;
}

void panel_encoder_reset(void)
{
    __HAL_TIM_SET_COUNTER(&htim4, 0);
}


void periphery_init(void)
{
    struct machine *m = &machine;
    HAL_TIM_Base_Start_IT(&htim6); // Run system timer 1kHz
    HAL_TIM_Base_Start_IT(&htim7); // Run delay timer

    m->btn_left = button_register("btn_left", BUTTON_LEFT_GPIO_Port,
                                  BUTTON_LEFT_Pin, 0, NULL, NULL);
    m->btn_right = button_register("btn_right", BUTTON_RIGHT_GPIO_Port,
                                   BUTTON_RIGHT_Pin, 0, NULL, NULL);
    m->btn_up = button_register("btn_up", BUTTON_UP_GPIO_Port,
                                BUTTON_UP_Pin, 0, NULL, NULL);
    m->btn_down = button_register("btn_down", BUTTON_DOWN_GPIO_Port,
                                  BUTTON_DOWN_Pin, 0, NULL, NULL);
    m->btn_enc = button_register("btn_encoder", PANEL_ENC_BUTTON_GPIO_Port,
                                 PANEL_ENC_BUTTON_Pin, 0, NULL, NULL);
    m->switch_run = button_register("switch_run", SWITCH_RUN_GPIO_Port,
                                    SWITCH_RUN_Pin, 0, NULL, NULL);
    m->switch_touch_lock = button_register("switch_touch_lock",
                                           SWITCH_TOUCH_LOCK_GPIO_Port,
                                           SWITCH_TOUCH_LOCK_Pin, 0, NULL, NULL);
    m->switch_high_speed = button_register("switch_high_speed",
                                           SWITCH_HIGH_SPEED_GPIO_Port,
                                           SWITCH_HIGH_SPEED_Pin, 0, NULL, NULL);
    m->switch_move_to = button_register("switch_gap_compensation",
                                                 SWITCH_GO_TO_MODE_GPIO_Port,
                                                 SWITCH_GO_TO_MODE_Pin, 0, NULL, NULL);

    gpio_up(&gpio_disp1_spi_cs);
    gpio_up(&gpio_disp2_spi_cs);
    gpio_up(&gpio_disp_reset);
    sleep(50);
    gpio_down(&gpio_disp_reset);
    sleep(50);
    gpio_up(&gpio_disp_reset);
    sleep(50);

    m->disp1 = disp_register("display_touch", &gpio_disp1_spi_cs,
                                   &gpio_disp_dc_rs, &hspi1,
                                   DISP_ORIENT_PORTRAIT);
    sleep(10);

    m->disp1->touch = touch_xpt2046_register("main_touch",
                                      &gpio_touch_spi_cs,
                                      &hspi2, DISP_ORIENT_PORTRAIT);

    m->disp2 = disp_register("display_2",
                             &gpio_disp2_spi_cs,
                             &gpio_disp_dc_rs,
                             &hspi1, DISP_ORIENT_LANDSCAPE);


    m->pm_move_speed = potentiometer_register("pm_move_speed", &hadc1, 50);

    m->sm_longitudial_feed =
            stepper_motor_register("longitudial_feed_motor",
                                   &htim2, &htim1, TIM_CHANNEL_1,
                                   &gpio_longitudal_feed_dir,
                                   &gpio_longitudal_feed_en,
                                   1000000, 18, 15000,
                                   LINEAR_LONGITUDAL_RESOLUTION,
                                   15000, 17600);

    m->sm_cross_feed =
            stepper_motor_register("cross_feed_motor",
                                   &htim5, &htim3, TIM_CHANNEL_1,
                                   &gpio_cross_feed_dir,
                                   &gpio_cross_feed_en,
                                   1000000, 18, 10000,
                                   LINEAR_CROSS_RESOLUTION,
                                   10000, 10000);

    m->ap = abs_position_dev_register("abs_position_dev",
                                           &hspi2, &gpio_abs_pos_cs);

    panel_encoder_init();
//    HAL_TIM_Base_Start_IT(&htim12);
    //HAL_TIM_IC_Start(&htim12, TIM_CHANNEL_1); // Spindle Encoder
}

// IRQ context
void HAL_GPIO_EXTI_Callback(u16 pin)
{
    struct machine *m = &machine;

    switch (pin) {
    case TOUCH_IRQ_Pin:
        touch_isr(m->disp1->touch);
        return;
    }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    struct machine *m = &machine;

    if (htim == &htim6) { // Period = 1ms (1000Hz)
        if (m->sm_longitudial_feed)
            stepper_motor_timer_isr(m->sm_longitudial_feed);
        if (m->sm_cross_feed)
            stepper_motor_timer_isr(m->sm_cross_feed);
        return;
    }

    if (htim == &htim4) { // Panel encoder
        printf("htim4.cnt = %lu\r\n", htim4.Instance->CNT);
        return;
    }

    if (htim == &htim2) { // longitudial feed counter
        stepper_motor_stop_isr(m->sm_longitudial_feed);
        return;
    }

    if (htim == &htim5) { // cross feed counter
        stepper_motor_stop_isr(m->sm_cross_feed);
        return;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    struct machine *m = &machine;

    if (hadc == &hadc1) {
        potentiometer_irq(m->pm_move_speed);
        return;
    }
}
