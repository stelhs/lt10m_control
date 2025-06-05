/*
 * wait.c
 */
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "wait.h"
#include "periphery.h"


extern struct machine machine;

int wait(u32 flags, uint timeout_ms)
{
    struct machine *m = &machine;

    struct timeout timeout;
    if (timeout_ms)
        timeout_start(&timeout, timeout_ms);

    while(1) {
        yield();

        if (timeout_ms && is_timeout_elapsed(&timeout))
            return 0;

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
}

