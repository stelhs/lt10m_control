/*
 * timers.c
 *
 */
#include "kref_alloc.h"
#include "klist.h"
#include "timers.h"
#include "cmsis_thread.h"


static struct list workers = LIST_INIT;
struct cmsis_thread *timers_tid = NULL;

static void process_periodic(struct timer_worker *wrk)
{
    if (!is_timeout_started(&wrk->timeout))
        return;

    if (!is_timeout_elapsed(&wrk->timeout))
        return;

    wrk->timeout.end += wrk->timeout.duration;
    wrk->cb(wrk->ctx);
}

static void process_timeout(struct timer_worker *wrk)
{
    if (!is_timeout_started(&wrk->timeout))
        return;

    if (!is_timeout_elapsed(&wrk->timeout))
        return;

    timeout_stop(&wrk->timeout);
    wrk->cb(wrk->ctx);
    kmem_deref(&wrk);
}

static void timers_thread(void *priv)
{
    struct le *le, *le_tmp;
    while (1) {
        yield();
        LIST_FOREACH_SAFE(&workers, le, le_tmp) {
            struct timer_worker *wrk = (struct timer_worker *)list_ledata(le);
            if (wrk->periodic_flag) {
                process_periodic(wrk);
                continue;
            }
            process_timeout(wrk);
        }
    }
}

static void timer_worker_destructor(void *mem)
{
    struct timer_worker *wrk = (struct timer_worker *)mem;
    list_unlink(&wrk->le);
}

static struct timer_worker *
        timer_worker_register(char *name, u32 duration_ms,
                              void (*cb)(void *), void *ctx, size_t ctx_sz,
                              bool periodic_flag)
{
    struct timer_worker *wrk;
    if (!timers_tid)
        timers_tid = thread_register("timers_thread", TIMERS_THREAD_STACK_SZ,
                                     timers_thread, NULL);

    wrk = kzref_alloc(name, sizeof *wrk + ctx_sz, timer_worker_destructor);
    if (!wrk)
        return NULL;

    wrk->cb = cb;
    wrk->ctx = ctx_sz ? wrk + 1 : ctx;
    if (ctx_sz)
        memcpy(wrk->ctx, ctx, ctx_sz);
    wrk->periodic_flag = periodic_flag;
    wrk->name = name;
    list_append(&workers, &wrk->le, wrk);
    timeout_start(&wrk->timeout, duration_ms);
    return wrk;
}

struct timer_worker *set_timeout(char *name, u32 interval_ms,
                                 void (*cb)(void *), void *ctx, size_t ctx_sz)
{
    return timer_worker_register(name, interval_ms, cb, ctx, ctx_sz, FALSE);
}

struct timer_worker *set_periodic(char *name, u32 interval_ms,
                                  void (*cb)(void *), void *ctx, size_t ctx_sz)
{
    return timer_worker_register(name, interval_ms, cb, ctx, ctx_sz, TRUE);
}

void timer_worker_reset_timeout(struct timer_worker *wrk)
{
    timeout_start(&wrk->timeout, wrk->timeout.duration);
}

void timers_print(void)
{
    struct le *le;

    printf("Registred timers list: \r\n");
    LIST_FOREACH(&workers, le) {
        struct timer_worker *wrk = (struct timer_worker *)list_ledata(le);
        printf("\t%s:%s: %lums\r\n", wrk->name,
               (wrk->periodic_flag ? "periodic" : "single"),
               wrk->timeout.duration);
    }
    printf("\r\n");
}
