#include "main.h"
#include "stm32_lib/types.h"
#include "stm32_lib/buf.h"
#include "stm32_lib/kref_alloc.h"
#include "stm32_lib/cmsis_thread.h"
#include "stm32_lib/timers.h"
#include "stm32_lib/uart_debug.h"
#include "stm32_lib/buttons.h"
#include "machine.h"
#include "wait.h"

#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "periphery.h"
#include "sm_butt_ctrl.h"

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
    int key_width = 65;
    int key_height = 65;
    int key_num = 0;
    int left_indent = 60;
    int top_indent = 15;
    int key_indent = 10;

    struct text_style key_text_style = {
            .bg_color = BLACK,
            .color = BLUE,
            .font = font_rus,
            .fontsize = 3
    };

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 4; y++) {
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

    disp_clear(m->disp);
    test_keypad();
}

extern ADC_HandleTypeDef hadc1;
void key_d(void *priv)
{
	struct machine *m = (struct machine *)priv;
    printf("key_d\r\n");

    printf("start\r\n");
  //  stepper_motor_enable(m->sm_cross_feed);
    stepper_motor_enable(m->sm_longitudial_feed);
	//stepper_motor_run(m->sm_cross_feed, 1, 1);
    stepper_motor_run(m->sm_longitudial_feed, 10, 1);
	return;

//	disp_init(m->disp);
//	disp_test();
	printf("htim4.cnt = %lu\r\n", htim4.Instance->CNT);


//	touch_read(m->touch);
/*    printf("cnt = %lu\r\n", stepper_motor_pos(m->sm_longitudial_feed));
	stepper_motor_set_speed(m->sm_longitudial_feed, 300);
	stepper_motor_reset_pos(m->sm_longitudial_feed);
	stepper_motor_set_autostop(m->sm_longitudial_feed, 1200);
	stepper_motor_run_backward(m->sm_longitudial_feed);*/
}

void key_f(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_f\r\n");

    printf("stop\r\n");
    stepper_motor_stop(m->sm_cross_feed);
    stepper_motor_stop(m->sm_longitudial_feed);
    stepper_motor_disable(m->sm_cross_feed);
    stepper_motor_disable(m->sm_longitudial_feed);
    return;

    htim4.Instance->CNT = 0;
//  disp_init(m->disp);
//  disp_test();
//  touch_read(m->touch);
    stepper_motor_stop(m->sm_longitudial_feed);
    printf("cnt = %lu\r\n", stepper_motor_pos(m->sm_longitudial_feed));
}

void key_g(void *priv)
{
    struct machine *m = (struct machine *)priv;
    printf("key_g\r\n");

    printf("cross_feed = %lu\r\n", stepper_motor_pos(m->sm_cross_feed));
    printf("longitudial_feed = %lu\r\n", stepper_motor_pos(m->sm_longitudial_feed));

    stepper_motor_reset_pos(m->sm_cross_feed);
    stepper_motor_reset_pos(m->sm_longitudial_feed);

//    stepper_motor_set_speed(m->sm_cross_feed,
  //                          m->sm_cross_feed->freq + 1);
    stepper_motor_set_speed(m->sm_longitudial_feed,
                            m->sm_longitudial_feed->freq + 1);
}

static void main_thread(void *priv)
{
    struct machine *m = (struct machine *)priv;
    memset(m, 0, sizeof *m);

    uart_debug_plug(&huart1);
    uart_dbg_key_register("d_key", 'd', key_d, m);
    uart_dbg_key_register("f_key", 'f', key_f, m);
    uart_dbg_key_register("g_key", 'g', key_g, m);

    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    periphery_init();

    HAL_TIM_Base_Start_IT(&htim4);
    printlog("Machine initializing success\r\n");

    disp_test();

    stepper_motor_enable(m->sm_cross_feed);
    stepper_motor_enable(m->sm_longitudial_feed);

    m->sbc_up = sm_butt_ctrl_register(kmem_ref(m->btn_up),
                                      kmem_ref(m->sm_cross_feed),
                                      0, 10, 50, 1);
    m->sbc_down = sm_butt_ctrl_register(kmem_ref(m->btn_down),
                                        kmem_ref(m->sm_cross_feed),
                                        1, 10, 50, 1);
    m->sbc_left = sm_butt_ctrl_register(kmem_ref(m->btn_left),
                                        kmem_ref(m->sm_longitudial_feed),
                                        0, 10, 50, 1);
    m->sbc_right = sm_butt_ctrl_register(kmem_ref(m->btn_right),
                                         kmem_ref(m->sm_longitudial_feed),
                                         1, 10, 50, 1);


    for(;;) {
        sm_butt_ctrl_do(m->sbc_up);
        sm_butt_ctrl_do(m->sbc_down);
        sm_butt_ctrl_do(m->sbc_left);
        sm_butt_ctrl_do(m->sbc_right);

        if (is_potentiometer_changed(m->pm_move_speed)) {
            u16 val = potentiometer_val(m->pm_move_speed);
            printf("potentiometer = %d\r\n", val);

            sm_butt_ctrl_set_max_freq(m->sbc_up, 10 + 12000 * val / 100);
            sm_butt_ctrl_set_max_freq(m->sbc_down, 10 + 12000 * val / 100);
            sm_butt_ctrl_set_max_freq(m->sbc_left, 10 + 10000 * val / 100);
            sm_butt_ctrl_set_max_freq(m->sbc_right, 10 + 10000 * val / 100);
        }
/*    	lde_d2_on();
    	sleep(500);
    	lde_d2_off();
    	sleep(500);
    	printlog("is pressed = %d\r\n", is_button_clicked(m->btn_k0));*/

        if (m->touch->is_touched) {
            int x = 480 - 480 * m->touch->x / 4096;
            int y = 320 - 320 * m->touch->y / 4096;
            disp_fill(m->disp, x-4, y-4, 8, 8, YELLOW);
            printf("%d %d\r\n", m->touch->x, m->touch->y);
            m->touch->is_touched = 0;
        }
        yield();
    }
}


void machine_init(void)
{
    machine.tid = thread_register("main_thread", 2048,
                                  main_thread, &machine);
}








