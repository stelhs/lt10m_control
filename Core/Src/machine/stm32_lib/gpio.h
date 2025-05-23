/*
 * gpio.h
 *

 */
#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"
#include "types.h"
#include "timers.h"

struct gpio {
    GPIO_TypeDef *port;
    u32 pin;
};

struct gpio_exti_filter {
    int pulse_min, pulse_max;
    int silent_interval;
    int def_state;
    struct gpio gpio;
    struct timeout t_silent;
    struct timeout t_pulse_down_min;
    struct timeout t_pulse_down_max;
    struct timeout t_pulse_up;
    int cnt;
    void (*cb)(void *);
    void *priv;
};

struct gpio_exti_filter *
    gpio_exti_filter_create(struct gpio gpio,
                             int pulse_min, int pulse_max,
                             int pulse_interval, int def_state,
                             void (*cb)(void *), void *priv);
bool is_gpio_exti_triggered(struct gpio_exti_filter *gif);
void gpio_exti_irq(struct gpio_exti_filter *gif);

static inline void gpio_up(struct gpio *gpio)
{
    gpio->port->BSRR = gpio->pin;
}

static inline void gpio_down(struct gpio *gpio)
{
    gpio->port->BSRR = (u32)gpio->pin << 16u;
}

static inline int gpio_read(struct gpio *gpio)
{
    return (gpio->port->IDR & gpio->pin) != 0;
}

#endif // GPIO_H
