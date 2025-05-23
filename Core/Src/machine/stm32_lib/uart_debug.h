/*
 * debug_uart.h
 *
 */

#ifndef DEBUG_UART_H_
#define DEBUG_UART_H_
#include "stm32f4xx_hal.h"

struct debug_uart {
    UART_HandleTypeDef *huart;
    struct cmsis_thread *tid;
};

int uart_debug_plug(UART_HandleTypeDef *huart);
int uart_dbg_key_register(char *name, char key,
                          void (*cb)(void *), void *priv);


#endif /* DEBUG_UART_H_ */
