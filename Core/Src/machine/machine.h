#ifndef MACHINE_H_
#define MACHINE_H_
#include "disp_mipi_dcs.h"
#include "touch_xpt2046.h"
#include "stepper_motor.h"
#include "stm32_lib/types.h"
#include "stm32_lib/types.h"
#include "timestamp.h"

#define BUILD_VERSION "0.1"

extern struct machine machine;



#define printlog(format, ...) do { \
    printf("%ld: " format, now() ? now() : uptime(), ##__VA_ARGS__); \
} while (0)


struct machine {
    struct cmsis_thread *tid;
    struct button *btn_k0;
    struct disp *disp;
    struct touch_xpt2046 *touch;
    struct stepper_motor *sm_longitudial_feed;
    struct stepper_motor *sm_cross_feed;
};

extern struct machine machine;

void panic(char *reason);
void machine_init(void);

#endif  // MACHINE_H_
