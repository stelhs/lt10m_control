/*
 * timers.h
 *
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include "cmsis_thread.h"

struct timer_worker {
    struct le le;
    void (*cb)(void *);
    void *ctx;
    bool periodic_flag;
    char *name;
    struct timeout timeout;
};

struct timer_worker *set_timeout(char *name, u32 interval,
                                 void (*cb)(void *), void *ctx, size_t ctx_sz);

struct timer_worker *set_periodic(char *name, u32 interval,
                                  void (*cb)(void *), void *ctx, size_t ctx_sz);

void timer_worker_reset_timeout(struct timer_worker *wrk);
void timers_print(void);

#endif /* TIMERS_H_ */
