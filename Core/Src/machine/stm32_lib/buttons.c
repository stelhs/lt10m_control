/*
 * buttons.c
 *
 */
#include "kref_alloc.h"
#include "klist.h"
#include "cmsis_thread.h"
#include "buttons.h"

static struct list buttons = LIST_INIT;
static struct cmsis_thread *buttons_tid = NULL;


bool is_button_clicked(struct button *btn)
{
    bool pressed = btn->pressed;
    if (pressed)
        btn->pressed = FALSE;
    return pressed;
}

bool is_button_long_pressing(struct button *btn)
{
    return btn->long_pressed;
}

bool is_button_being_pressed(struct button *btn)
{
    return btn->state == 0;
}

bool is_button_changed(struct button *btn)
{
    bool changed = btn->changed;
    if (changed)
        btn->changed = FALSE;
    return changed;
}

void button_reset(struct button *btn)
{
    btn->pressed = FALSE;
}


static void processed_button(struct button *btn)
{
    btn->state = (btn->gp.port->IDR & btn->gp.pin) != 0;
    if (btn->prev_state < 0) {
        btn->prev_state = btn->state;
        return;
    }

    if (is_timeout_started(&btn->t_long) &&
            is_timeout_elapsed(&btn->t_long) &&
            btn->state == btn->pressed_state) {
        timeout_stop(&btn->t_long);
        btn->long_pressed = TRUE;
    }

    if (is_timeout_started(&btn->t_bounce) &&
            is_timeout_elapsed(&btn->t_bounce)) {
        timeout_stop(&btn->t_bounce);

        if (btn->state != btn->waiting_state)
            return;

        btn->changed = TRUE;
        if (btn->state != btn->pressed_state) {
            btn->prev_state = btn->state;

            if (!btn->long_pressed)
                btn->pressed = TRUE;

            btn->long_pressed = FALSE;
            timeout_stop(&btn->t_long);
            if (btn->cb)
                btn->cb(btn->priv);
            return;
        }
    }

    if (btn->state == btn->prev_state)
        return;
    btn->prev_state = btn->state;

    if (!is_timeout_started(&btn->t_bounce))
        btn->waiting_state = btn->state;
    timeout_start(&btn->t_bounce, 20);

    if (btn->state == btn->pressed_state)
        timeout_start(&btn->t_long, 1500);
}


static void buttons_thread(void *priv)
{
    struct le *le;
    while (1) {
        yield();
        LIST_FOREACH(&buttons, le) {
            struct button *btn = (struct button *)list_ledata(le);
            processed_button(btn);
        }
    }
}

static void button_destructor(void *mem)
{
    struct button *btn = (struct button *)mem;
    UNUSED(btn);
}

struct button *button_register(char *name, GPIO_TypeDef *gpio_port,
                               uint gpio_pin, int pressed_state,
                               void (*cb)(void *), void *priv)
{
    struct button *btn;
    if (!buttons_tid) {
        buttons_tid = thread_register("buttons_thread", BUTTONS_THREAD_STACK_SZ,
                                      buttons_thread, NULL);
        if (!buttons_tid)
            return NULL;
    }

    btn = kzref_alloc(name, sizeof *btn, button_destructor);
    if (!btn)
        return NULL;

    btn->name = name;
    btn->gp.port = gpio_port;
    btn->gp.pin = gpio_pin;
    btn->prev_state = -1;
    btn->state = -1;
    btn->cb = cb;
    btn->priv = priv;
    list_append(&buttons, &btn->le, btn);
    return btn;
}

