#ifndef THREAD_TABLE_H_
#define THREAD_TABLE_H_

#include "stm32_lib/types.h"

struct __attribute__((packed)) dimensions {
    s32 min;
    s32 max;
};

struct __attribute__((packed)) thread_metric {
    s16 diameter;
    s16 step;
    u8 is_default;
    struct dimensions bolt_major;
    struct dimensions bolt_minor;
    struct dimensions internal_major;
    struct dimensions internal_minor;
    s32 drill_size;
};

extern const struct thread_metric thread_metric_table[];

#endif /* THREAD_TABLE_H_ */
