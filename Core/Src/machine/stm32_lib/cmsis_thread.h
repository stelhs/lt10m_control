/*
 * cmsis_thread.h
 *
 */

#ifndef CMSIS_THREAD_H_
#define CMSIS_THREAD_H_
#include "klist.h"
#include "cmsis_os.h"
#include "tim.h"

extern TIM_HandleTypeDef htim1;

#define irq_enable() __asm volatile ("cpsie i" : : : "memory")
#define irq_disable() __asm volatile ("cpsid i" : : : "memory")

#define yield() osThreadYield()
#define sleep(msec) osDelay(msec)
#define now() osKernelGetTickCount()

struct cmsis_thread_msg {
    struct le le;
    int type;
    void *msg;
};

struct cmsis_thread {
    struct list messages;
    struct le le;
    char *name;
    osThreadId_t tid;
    void *cb_mem;
    struct buf *stack;
};

struct cmsis_thread *thread_register(char *name, size_t stack_size,
                                     void (*thread)(void *priv), void *priv);
void threads_stat_print(void);


struct timeout {
    u32 end;
    u32 duration;
};

static inline void timeout_start(struct timeout *timeout, u32 duration)
{
    u32 curr = now();
    u32 end = curr + duration - 1;
    timeout->duration = duration;
    timeout->end = end ? end : 1;
}

static inline void timeout_reset(struct timeout *timeout)
{
    u32 curr = now();
    u32 end = curr + timeout->duration - 1;
    timeout->end = end ? end : 1;
}

static inline u32 timeout_left(struct timeout *timeout)
{
    u32 curr = now();
    return timeout->end - curr;
}

static inline bool is_timeout_elapsed(struct timeout *timeout)
{
    bool elapsed = timeout_left(timeout) >= timeout->duration;
    if (!timeout->end)
        return FALSE;
    return elapsed;
}

static inline void timeout_stop(struct timeout *timeout)
{
    timeout->end = 0;
}

static inline bool is_timeout_started(struct timeout *timeout)
{
    return timeout->end != 0;
}

static inline void delay_us(u16 us)
{
    __HAL_TIM_SET_COUNTER(&DELAY_TIMER, 0);
    while (__HAL_TIM_GET_COUNTER(&DELAY_TIMER) < us);
}

struct stopwatch {
    u32 start;
};

#define thread_lock(lock) do { \
    while (lock) \
        yield(); \
    lock = TRUE; \
} while (0)

#define thread_unlock(lock) do { \
    lock = FALSE; \
} while (0)

struct cmsis_thread *thread_current(void);
void thread_send_msg(struct cmsis_thread *tid, int type, void *msg);
int thread_recv_msg(void **msg);

#endif /* CMSIS_THREAD_H_ */
