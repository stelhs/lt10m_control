#include "main.h"
#include "stm32_lib/types.h"
#include "stm32_lib/buf.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/timers.h"
#include "stm32_lib/uart_debug.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "periphery.h"
#include "disp_ili9488.h"

struct machine machine;
extern UART_HandleTypeDef huart1;


void halt(char *reason)
{
    panic(reason);
}


void panic(char *cause)
{
    printlog("PANIC: %s\r\n", cause);
    for (;;) {
        printlog("PANIC: %s\r\n", cause);
        kmem_print_stat();
        HAL_Delay(3000);
    }
}


struct __attribute__((packed)) pixel {
	u8 b;
	u8 g;
	u8 r;
};

static void disp_test(void)
{
    struct machine *m = &machine;
    struct color c = {
            .r = 0,
            .g = 0,
            .b = 255
    };
    disp_ili9488_fill(m->disp, 100, 10, 6*4, 8*4, c);
}


void display_cb(void *priv)
{
	struct machine *m = (struct machine *)priv;

	printf("display_cb\r\n");
	disp_ili9488_init(m->disp);
	disp_test();
}


static void main_thread(void *priv)
{
    struct machine *m = (struct machine *)priv;
    memset(m, 0, sizeof *m);

    uart_debug_plug(&huart1);
    uart_dbg_key_register("display", 'd', display_cb, m);

    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    periphery_init();

    printlog("Machine initializing success\r\n");

    disp_test();
    for(;;) {
    	lde_d2_on();
    	sleep(100);
    	lde_d2_off();
    	sleep(100);
    	printlog("is pressed = %d\r\n", is_button_clicked(m->btn_k0));
    }
}


void machine_init(void)
{
    machine.tid = thread_register("main_thread", 2048,
                                  main_thread, &machine);
}








