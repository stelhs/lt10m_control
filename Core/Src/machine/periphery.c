/*
 * periphery.c
 *
 */
#include <stdlib.h>
#include "main.h"
#include "machine.h"
#include "periphery.h"

#include "spi.h"
#include "usart.h"
#include "tim.h"
#include "disp_mipi_dcs.h"
#include "stepper_motor.h"
#include "touch_xpt2046.h"
#include "stm32_lib/gpio.h"
#include "stm32_lib/buttons.h"

extern struct machine machine;

static struct gpio gpio_led_d2 = {LED_D2_GPIO_Port, LED_D2_Pin};
static struct gpio gpio_disp_spi_cs = {SPI_TFT_CS_GPIO_Port, SPI_TFT_CS_Pin};
static struct gpio gpio_disp_reset = {SPI_TFT_RESET_GPIO_Port,
                                      SPI_TFT_RESET_Pin};
static struct gpio gpio_disp_dc_rs = {SPI_TFT_DC_RS_GPIO_Port,
                                      SPI_TFT_DC_RS_Pin};
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


void lde_d2_on(void)
{
    gpio_up(&gpio_led_d2);
}

void lde_d2_off(void)
{
    gpio_down(&gpio_led_d2);
}

void periphery_init(void)
{
    struct machine *m = &machine;

    m->btn_k0 = button_register("btn_k0", BUTTON_K0_GPIO_Port,
                                BUTTON_K0_Pin, 0, NULL, NULL);

    m->disp = disp_register("main_display", &gpio_disp_spi_cs,
                            &gpio_disp_reset, &gpio_disp_dc_rs,
                            &hspi1);

    m->touch = touch_xpt2046_register("main_touch", &gpio_touch_spi_cs,
                                      &hspi2);

    m->sm_longitudial_feed = stepper_motor_register("longitudial_feed_motor",
                                                    &htim2, TIM_CHANNEL_1,
                                                    &gpio_longitudal_feed_dir,
                                                    &gpio_longitudal_feed_en,
                                                    1000000);

    m->sm_cross_feed = stepper_motor_register("cross_feed_motor",
                                              &htim3, TIM_CHANNEL_1,
                                              &gpio_cross_feed_dir,
                                              &gpio_cross_feed_en,
                                              1000000);
}

// IRQ context
void HAL_GPIO_EXTI_Callback(u16 pin)
{
    struct machine *m = &machine;

    switch (pin) {
    case TOUCH_IRQ_Pin:
        touch_isr(m->touch);
        return;
    }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    struct machine *m = &machine;

    if (htim == &htim2) {
        stepper_motor_isr(m->sm_longitudial_feed);
        return;
    }

    if (htim == &htim3) {
        stepper_motor_isr(m->sm_cross_feed);
        return;
    }
}
