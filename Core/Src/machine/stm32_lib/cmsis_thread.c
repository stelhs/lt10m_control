/*
 * cmsis_thread.c
 *
 */
#include "kref_alloc.h"
#include "klist.h"
#include "buf.h"
#include "cmsis_thread.h"
#include "main.h"


static struct list threads_list = LIST_INIT;

static void cmsis_thread_destructor(void *mem)
{
    struct cmsis_thread *tid = (struct cmsis_thread *)mem;
    osThreadState_t state = osThreadGetState(tid->tid);

    list_unlink(&tid->le);

    if (state != osThreadInactive && state != osThreadTerminated)
        osThreadTerminate(tid->tid);

    if (tid->stack) {
        memset(tid->stack->d, 0, buf_len(tid->stack));
        kmem_deref(&tid->stack);
    }

    if (tid->cb_mem)
        kmem_deref(&tid->cb_mem);
}

struct cmsis_thread *thread_register(char *name, size_t stack_size,
                                     void (*thread)(void *priv), void *priv)
{
    struct cmsis_thread *tid;

    tid = kzref_alloc(name, sizeof *tid, cmsis_thread_destructor);
    if (!tid)
        return NULL;

    tid->name = name;
    tid->cb_mem = kzref_alloc("task_control_block",
                              sizeof (StaticTask_t), NULL);
    if (!tid->cb_mem)
        goto err;

    tid->stack = buf_alloc("stack", stack_size);
    if (!tid->stack)
        goto err;
    buf_put(tid->stack, stack_size);

    const osThreadAttr_t task_attrs = {
      .name = name,
      .stack_mem = tid->stack->d,
      .stack_size = stack_size,
      .cb_mem = tid->cb_mem,
      .cb_size = sizeof (StaticTask_t),
      .priority = osPriorityNormal,
    };

    tid->tid = osThreadNew(thread, priv, &task_attrs);
    list_append(&threads_list, &tid->le, tid);
    return tid;
err:
    kmem_deref(&tid);
    return NULL;
}

struct cmsis_thread *thread_current(void)
{
    struct le *le;
    osThreadId_t tid = osThreadGetId();
    LIST_FOREACH(&threads_list, le) {
        struct cmsis_thread *thread = (struct cmsis_thread *)list_ledata(le);
        if (thread->tid == tid)
            return thread;
    }
    return NULL;
}


static void thread_msg_destructor(void *mem)
{
    struct cmsis_thread_msg *ct_msg = (struct cmsis_thread_msg *)mem;
    list_unlink(&ct_msg->le);
}

void thread_send_msg(struct cmsis_thread *tid, int type, void *msg)
{
    struct cmsis_thread_msg *ct_msg;
    ct_msg = kzref_alloc("thread_message", sizeof *ct_msg,
                         thread_msg_destructor);
    ct_msg->type = type;
    ct_msg->msg = msg;
    list_append(&tid->messages, &ct_msg->le, ct_msg);
}

int thread_recv_msg(void **msg)
{
    struct cmsis_thread *thread = thread_current();
    struct le *le;
    int type;
    struct cmsis_thread_msg *ct_msg;

    if (!list_count(&thread->messages))
        return -1;

    le = list_head(&thread->messages);
    ct_msg = (struct cmsis_thread_msg *)list_ledata(le);
    type = ct_msg->type;
    *msg = ct_msg->msg;
    kmem_deref(&ct_msg);
    return type;
}

size_t thread_stack_max_used_size(struct cmsis_thread *tid)
{
    size_t i = 0;
    size_t stack_size = buf_len(tid->stack);
    u8 *stack = tid->stack->d;

    for (i = 0; i < stack_size; i++) {
        if (stack[i] != 0xA5)
            break;
    }
    return stack_size - i;
}

void threads_stat_print(void)
{
    struct le *le;
    printf("\r\nList of threads:\r\n");
    LIST_FOREACH(&threads_list, le) {
        struct cmsis_thread *tid = (struct cmsis_thread *)list_ledata(le);
        printf("\t%s: %d/%d\r\n", tid->name,
               thread_stack_max_used_size(tid), buf_len(tid->stack));
    }
    printf("\r\n");
}
