#include "main.h"
#include "stm32_lib/types.h"
#include "stm32_lib/buf.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/timers.h"
#include "stm32_lib/uart_debug.h"
#include "stm32_lib/buttons.h"
#include "machine.h"

#include "disp_mipi_dcs.h"
#include "periphery.h"

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


static void test_keypad(void)
{
    struct machine *m = &machine;
    int x, y;
    int key_width = 90;
    int key_height = 90;
    int key_num = 0;
    int left_indent = 100;
    int top_indent = 15;
    int key_indent = 10;

    struct text_style key_text_style = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 4
    };

    for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
            key_num ++;
            char buf[3];
            disp_rect(m->disp, left_indent + x * (key_width + key_indent),
                      top_indent + y * (key_height + key_indent),
                      key_width, key_height,
                      1, GREEN);
            snprintf(buf, 3, "%d", key_num);
            disp_text(m->disp, buf,
                    left_indent + x * (key_width + key_indent) + key_width / 2 - 10,
                    top_indent + y * (key_height + key_indent) + key_height / 2 - 10,
                    &key_text_style);
        }
    }
}


static void disp_test(void)
{
    struct machine *m = &machine;
/*    struct img *img;
    struct color c = {
            .r = 0,
            .g = 0,
            .b = 255
    };
    struct color bgc = {
            .r = 0,
            .g = 0,
            .b = 0
    };

    u8 ch = 0;
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 20; x++) {
            img = font_symbol_img(ch, font_rus, 2, c, bgc);
            disp_fill_img(m->disp, x*6*2, y*10*2, img);
            kmem_deref(&img);
            ch ++;
        }
    }*/

    /*struct text_style style = {
            .font = font_rus,
            .fontsize = 4,
            .bg_color = {0, 0, 0},
            .color = {0, 255, 0}
    };
    disp_text(m->disp, "Вентилятор: ", 50, 100, &style);

    img = img_test1();
    disp_fill_img(m->disp, 320, 90, img);
    kmem_deref(&img);*/

    test_keypad();

}


void display_cb(void *priv)
{
	struct machine *m = (struct machine *)priv;

	printf("display_cb\r\n");
	disp_init(m->disp);
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








