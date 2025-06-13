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
#define tick() osKernelGetTickCount()

struct cmsis_thread {
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
    u32 curr = tick();
    u32 end = curr + duration - 1;
    timeout->duration = duration;
    timeout->end = end ? end : 1;
}

static inline void timeout_reset(struct timeout *timeout)
{
    u32 curr = tick();
    u32 end = curr + timeout->duration - 1;
    timeout->end = end ? end : 1;
}

static inline u32 timeout_left(struct timeout *timeout)
{
    u32 curr = tick();
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
    u16 end = __HAL_TIM_GET_COUNTER(&htim1) + us;
//    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < end);
}

struct stopwatch {
    u32 start;
};

static inline void stopwatch_start(struct stopwatch *sw)
{
    sw->start = tick();
}

static inline u32 stopwatch_counter(struct stopwatch *sw)
{
    u32 now = tick();
    if (now >= sw->start)
        return (sw->start - now) / 1000;
    return (0xFFFFFFFF - sw->start + now) / 1000;
}

#define thread_lock(lock) do { \
    while (lock) \
        yield(); \
    lock = TRUE; \
} while (0)

#define thread_unlock(lock) do { \
    lock = FALSE; \
} while (0)


#endif /* CMSIS_THREAD_H_ */
