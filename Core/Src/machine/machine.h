#ifndef MACHINE_H_
#define MACHINE_H_
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "stepper_motor.h"
#include "potentiometer.h"
#include "stm32_lib/types.h"
#include "stm32_lib/types.h"
#include "timestamp.h"
#include "ui_status.h"
#include "ui_move_to.h"
#include "mode_cut.h"
#include "mode_thread.h"

#define BUILD_VERSION "0.4"

extern struct machine machine;

#define MACHINE_MSG_MOVETO_CROSS 1
#define MACHINE_MSG_MOVETO_LONGITUDAL 2

#define printlog(format, ...) do { \
    printf("%ld: " format, now() ? now() : uptime(), ##__VA_ARGS__); \
} while (0)

enum progs {
    PROG_FEED_LEFT,
    PROG_FEED_RIGHT,
    PROG_FEED_LEFT_UP,
    PROG_FEED_RIGHT_UP,
    PROG_FEED_LEFT_DOWN,
    PROG_FEED_RIGHT_DOWN,
    PROG_FEED_UP_LEFT,
    PROG_FEED_UP_RIGHT,
    PROG_FEED_DOWN_LEFT,
    PROG_FEED_DOWN_RIGHT,
    PROG_FEED_UP,
    PROG_FEED_DOWN,
    PROG_THREAD_LEFT,
    PROG_THREAD_RIGHT
};

struct machine {
    enum progs prog;
    struct cmsis_thread *ui_tid;
    struct cmsis_thread *machine_tid;
    struct button *btn_left;
    struct button *btn_right;
    struct button *btn_up;
    struct button *btn_down;
    struct button *btn_enc;
    struct button *btn_ok;
    struct button *switch_run;
    struct button *switch_touch_lock;
    struct button *switch_high_speed;
    struct button *switch_move_to;
    struct disp *disp2;
    struct disp *disp1;

    struct stepper_motor *sm_longitudial;
    struct stepper_motor *sm_cross;

    struct potentiometer *pm_move_speed;

    struct abs_position *ap;
    struct ui_status ui_stat;
    struct ui_main *ui_main;
    struct ui_move_to ui_move_to;

    struct mode_cut mc;
    struct mode_thread mt;

    bool is_busy;
    bool is_disp2_needs_redraw;
};


extern struct machine machine;

void panic(char *reason);
void machine_init(void);
int cross_move_to(int target_pos, bool is_accurate,
                  void (*periodic_cb)(void *), void *priv);
int longitudal_move_to(int target_pos, bool is_accurate, int max_freq,
                       void (*periodic_cb)(void *), void *priv);
void buttons_reset(void);
int cross_up_new_position(int distance);
int cross_down_new_position(int distance);
int longitudal_left_new_position(int distance);
int longitudal_right_new_position(int distance);
int calc_cross_to_target(int curr_pos, int target_pos, bool *dir);
int calc_cross_position(int position, int distance, bool dir);
int calc_longitudal_position(int position, int distance, bool dir);
int calc_longitudal_to_target(int curr_pos, int target_pos, bool *dir);
void set_normal_acceleration(void);
void set_high_acceleration(void);
int cut_speed_calculate(int diameter, int rpm);

// IRQ context
void sm_cross_normal_acceleration_changer(struct stepper_motor *sm, bool is_init);
void sm_longitudal_normal_acceleration_changer(struct stepper_motor *sm, bool is_init);
// IRQ context
void sm_longitudial_high_acceleration_changer(struct stepper_motor *sm,
                                              bool is_init);

#endif  // MACHINE_H_
