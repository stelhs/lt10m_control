/*
 * touch_xpt2046.h
 *
 *  Created on: May 24, 2025
 *      Author: stelhs
 */

#ifndef SRC_MACHINE_TOUCH_XPT2046_H_
#define SRC_MACHINE_TOUCH_XPT2046_H_

#include "spi_dev.h"
#include "stm32_lib/timers.h"

struct touch_xpt2046 {
    struct spi_dev *spi;
    struct timeout t;
    u16 x;
    u16 y;
    bool is_touched;
};

struct touch_xpt2046 *
touch_xpt2046_register(char *name, struct gpio *cs, SPI_HandleTypeDef *hspi);
void touch_read(struct touch_xpt2046 *touch);

// IRQ context
static inline void touch_isr(struct touch_xpt2046 *touch)
{
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

    if (is_timeout_elapsed(&touch->t)) {
        timeout_start(&touch->t, 100);
        touch_read(touch);
    }


    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

#endif /* SRC_MACHINE_TOUCH_XPT2046_H_ */
