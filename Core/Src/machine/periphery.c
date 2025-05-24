/*
 * periphery.c
 *
 */
#include <stdlib.h>
#include "main.h"
#include "machine.h"
#include "periphery.h"

#include "disp_mipi_dcs.h"
#include "stm32_lib/gpio.h"
#include "stm32_lib/buttons.h"

extern struct machine machine;
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;

static struct gpio gpio_led_d2 = {LED_D2_GPIO_Port, LED_D2_Pin};
static struct gpio gpio_disp_spi_cs = {SPI_TFT_CS_GPIO_Port, SPI_TFT_CS_Pin};
static struct gpio gpio_disp_reset = {SPI_TFT_RESET_GPIO_Port, SPI_TFT_RESET_Pin};
static struct gpio gpio_disp_dc_rs = {SPI_TFT_DC_RS_GPIO_Port, SPI_TFT_DC_RS_Pin};

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

    m->disp = disp_register("main", &gpio_disp_spi_cs,
    								&gpio_disp_reset,
									&gpio_disp_dc_rs,
									&hspi1);
}

// IRQ context
void HAL_GPIO_EXTI_Callback(u16 pin)
{
    struct machine *m = &machine;

    switch (pin) {
    case TOUCH_IRQ_Pin:
        printf("EXTI %d\r\n", pin);
        return;
    }

}

