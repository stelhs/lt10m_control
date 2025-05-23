/*
 * gpio.c
 *
 */
#include "gpio.h"
#include "kref_alloc.h"

    ;
struct gpio_exti_filter *
    gpio_exti_filter_create(struct gpio gpio,
                             int pulse_min, int pulse_max,
                             int silent_interval, int def_state,
                             void (*cb)(void *), void *priv)
{
    struct gpio_exti_filter *gef;
    gef = kzref_alloc("gpio_exti_filter", sizeof *gef, NULL);
    gef->gpio = gpio;
    gef->pulse_min = pulse_min;
    gef->pulse_max = pulse_max;
    gef->silent_interval = silent_interval;
    gef->def_state = def_state;
    gef->cb = cb;
    gef->priv = priv;
    return gef;
}

// IRQ context
void gpio_exti_irq(struct gpio_exti_filter *gef)
{
    int st;

    st = gpio_read(&gef->gpio);
    if (st == !gef->def_state) {
        if (is_timeout_started(&gef->t_pulse_up) &&
                !is_timeout_elapsed(&gef->t_pulse_up)) {
            return;
        }

        if (is_timeout_started(&gef->t_pulse_up) &&
                is_timeout_elapsed(&gef->t_pulse_up)) {
            timeout_stop(&gef->t_pulse_up);
        }

        if (!is_timeout_started(&gef->t_pulse_up)) {
            timeout_start(&gef->t_pulse_down_min, gef->pulse_min);
            timeout_start(&gef->t_pulse_down_max, gef->pulse_max);
            return;
        }
    }

    if (st == gef->def_state) {
        if (!is_timeout_started(&gef->t_pulse_down_min))
            return;

        if (is_timeout_elapsed(&gef->t_pulse_down_min) &&
                !is_timeout_elapsed(&gef->t_pulse_down_max)) {
            gef->cnt ++;
            if (gef->cb)
                gef->cb(gef->priv);
            timeout_stop(&gef->t_pulse_down_min);
            timeout_stop(&gef->t_pulse_down_max);
            timeout_start(&gef->t_pulse_up, gef->pulse_min);
            timeout_start(&gef->t_silent, gef->silent_interval);
            return;
        }

        if (is_timeout_elapsed(&gef->t_pulse_down_min) &&
                is_timeout_elapsed(&gef->t_pulse_down_max)) {
            timeout_stop(&gef->t_pulse_down_min);
            timeout_stop(&gef->t_pulse_down_max);
        }
        return;
    }
}

bool is_gpio_exti_triggered(struct gpio_exti_filter *gef)
{
    irq_disable();
    if (!is_timeout_started(&gef->t_silent)) {
        irq_enable();
        return FALSE;
    }

    if (!is_timeout_elapsed(&gef->t_silent)) {
        irq_enable();
        return FALSE;
    }
    timeout_stop(&gef->t_silent);
    irq_enable();
    return TRUE;
}


