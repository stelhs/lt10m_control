#ifndef MACHINE_H_
#define MACHINE_H_
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "stepper_motor.h"
#include "potentiometer.h"
#include "stm32_lib/types.h"
#include "stm32_lib/types.h"
#include "timestamp.h"

#define BUILD_VERSION "0.2"

extern struct machine machine;



#define printlog(format, ...) do { \
    printf("%ld: " format, now() ? now() : uptime(), ##__VA_ARGS__); \
} while (0)


struct machine {
    struct cmsis_thread *tid;
    struct cmsis_thread *ui_tid;
    struct button *btn_left;
    struct button *btn_right;
    struct button *btn_up;
    struct button *btn_down;
    struct button *btn_enc;
    struct button *switch_run;
    struct button *switch_touch_lock;
    struct button *switch_high_speed;
    struct button *switch_gap_compensation;
    struct disp *disp1;
    struct disp *disp2;

    struct stepper_motor *sm_longitudial_feed;
    struct stepper_motor *sm_cross_feed;

    struct potentiometer *pm_move_speed;

    bool is_last_move_up;
    bool is_last_move_left;

};

extern struct machine machine;

void panic(char *reason);
void machine_init(void);

#endif  // MACHINE_H_
