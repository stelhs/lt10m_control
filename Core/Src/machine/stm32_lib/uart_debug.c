/*
 * debug_uart.c
 *
 */

#include "kref_alloc.h"
#include "klist.h"
#include "uart_debug.h"
#include "cmsis_thread.h"

static struct debug_uart *uart_dbg = NULL;
struct list uart_dbg_keys_list = LIST_INIT;

struct uart_debug_key {
    struct le le;
    char *name;
    char key;
    void (*cb)(void *);
    void *cb_priv;
};

int _write(int file, char *str, int len)
{
    size_t i=0;
    if (!uart_dbg)
        return len;

    for(i = 0; i < len; i++)
        HAL_UART_Transmit(uart_dbg->huart, (u8 *)str + i, 1, 0xFFFF);
    return len;
}

static void print_help(void)
{
    struct le *le;
    printf("Debug UART registred keys:\r\n");
    LIST_FOREACH(&uart_dbg_keys_list, le) {
        struct uart_debug_key *udk = (struct uart_debug_key *)
                                     list_ledata(le);
        printf("\tkey '%c': call %s()\r\n", udk->key, udk->name);
    }
    printf("\r\n");
}

static void udbg_thread(void *priv)
{
    struct debug_uart *udbg = (struct debug_uart *)priv;
    struct le *le;
    HAL_StatusTypeDef rc;
    u8 key;

    for(;;) {
        bool triggered = FALSE;
        yield();
        rc = HAL_UART_Receive(udbg->huart, &key, 1, 0);
        if (rc != HAL_OK)
            continue;

        printf("\r\nPressed key: %c\r\n", key);

        if (key == 'h') {
            print_help();
            continue;
        }

        LIST_FOREACH(&uart_dbg_keys_list, le) {
            struct uart_debug_key *udk = (struct uart_debug_key *)
                                         list_ledata(le);
            if (udk->key != (char)key)
                continue;
            udk->cb(udk->cb_priv);
            triggered = TRUE;
        }
        if (!triggered)
            printf("Press 'h' to help\r\n");
    }
}


int uart_debug_plug(UART_HandleTypeDef *huart)
{
    uart_dbg = kzref_alloc("uart_debug", sizeof *uart_dbg, NULL);
    if (!uart_dbg)
        return -1;

    uart_dbg->huart = huart;
    uart_dbg->tid = thread_register("uart_dbg_thread", UART_DBG_THREAD_STACK_SZ,
                                    udbg_thread, uart_dbg);
    return 0;
}


int uart_dbg_key_register(char *name, char key,
                          void (*cb)(void *), void *priv)
{
    struct uart_debug_key *udk;

    if (key == 'h') {
        printf("UART debug error: key 'h' is rezerved\r\n");
        return -2;
    }

    udk = kzref_alloc("uart_debug_key", sizeof *udk, NULL);
    if (!udk)
        return -1;

    udk->name = name;
    udk->key = key;
    udk->cb = cb;
    udk->cb_priv = priv;
    list_append(&uart_dbg_keys_list, &udk->le, udk);
    return 0;
}

