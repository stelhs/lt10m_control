/*
 * buttons.h
 *
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "klist.h"
#include "cmsis_thread.h"


struct button {
    struct le le;
    char *name;
    struct gpio gp;
    struct timeout t_bounce;
    struct timeout t_long;
    void (*cb)(void *);
    void *priv;
    u8 pressed_state: 1;
    u8 state: 1;
    u8 waiting_state: 1;
    u8 prev_state: 1;
    u8 long_pressed:1;
    u8 pressed:1;
    u8 changed:1;
};

bool is_button_clicked(struct button *btn);
bool is_button_long_pressing(struct button *btn);
bool is_button_being_pressed(struct button *btn);
bool is_button_changed(struct button *btn);
struct button *button_register(char *name, GPIO_TypeDef *gpio_port,
                               uint gpio_pin, int pressed_state,
                               void (*cb)(void *), void *priv);
void button_reset(struct button *btn);

#endif /* BUTTONS_H_ */
