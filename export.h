/***************************************************************************//**
 * @file    export.h
 * @brief   RT-Thread WebAssembly export functions
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#ifndef __EXPORT_H__
#define __EXPORT_H__

// #include "rtdebug.h"
#include "include/rtdef.h"
// #include "rtservice.h"
// #include "rtm.h"

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE
rt_uint8_t *rt_hw_stack_init(void *entry, void *parameter,
    rt_uint8_t *stack_addr, void *exit);
#ifdef RT_USING_SMP
// #define rt_hw_interrupt_disable rt_cpus_lock
// #define rt_hw_interrupt_enable rt_cpus_unlock
// void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread);
// void rt_hw_context_switch_to(rt_ubase_t to, struct rt_thread *to_thread);
// void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from, rt_ubase_t to, struct rt_thread *to_thread);
#else /* RT_USING_SMP */
    EMSCRIPTEN_KEEPALIVE
    rt_base_t rt_hw_interrupt_disable(void);
    EMSCRIPTEN_KEEPALIVE
    void rt_hw_interrupt_enable(rt_base_t level);
    EMSCRIPTEN_KEEPALIVE
    void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to);
    EMSCRIPTEN_KEEPALIVE
    void rt_hw_context_switch_to(rt_ubase_t to);
    EMSCRIPTEN_KEEPALIVE
    void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to);
#endif /* RT_USING_SMP */
// EMSCRIPTEN_KEEPALIVE
// void rt_hw_console_output(const char *str);


EMSCRIPTEN_KEEPALIVE
void rt_system_object_init(void);
EMSCRIPTEN_KEEPALIVE
void rt_system_tick_init(void);
EMSCRIPTEN_KEEPALIVE
void rt_tick_increase(void);
EMSCRIPTEN_KEEPALIVE
void rt_system_timer_init(void);
EMSCRIPTEN_KEEPALIVE
void rt_system_timer_thread_init(void);
EMSCRIPTEN_KEEPALIVE
rt_err_t rt_thread_init(struct rt_thread *thread, const char *name,
    void (*entry)(void *parameter), void *parameter,
    void *stack_start, rt_uint32_t stack_size,
    rt_uint8_t priority, rt_uint32_t tick);
// rt_err_t rt_thread_detach(rt_thread_t thread);
// rt_thread_t rt_thread_create(const char *name,
//                              void (*entry)(void *parameter),
//                              void       *parameter,
//                              rt_uint32_t stack_size,
//                              rt_uint8_t  priority,
//                              rt_uint32_t tick);
EMSCRIPTEN_KEEPALIVE
rt_thread_t rt_thread_self(void);
EMSCRIPTEN_KEEPALIVE
rt_err_t rt_thread_startup(rt_thread_t thread);
EMSCRIPTEN_KEEPALIVE
rt_err_t rt_thread_control(rt_thread_t thread, int cmd, void *arg);
EMSCRIPTEN_KEEPALIVE
rt_err_t rt_thread_sleep(rt_tick_t tick);
EMSCRIPTEN_KEEPALIVE
void rt_thread_idle_init(void);
EMSCRIPTEN_KEEPALIVE
void rt_system_scheduler_init(void);
EMSCRIPTEN_KEEPALIVE
void rt_system_scheduler_start(void);
#ifdef RT_USING_HEAP
    EMSCRIPTEN_KEEPALIVE
    void rt_system_heap_init(void *begin_addr, void *end_addr);
    EMSCRIPTEN_KEEPALIVE
    void *rt_malloc(rt_size_t nbytes);
    EMSCRIPTEN_KEEPALIVE
    void rt_free(void *ptr);
#endif /* RT_USING_HEAP */
EMSCRIPTEN_KEEPALIVE
rt_device_t rt_device_find(const char *name);
EMSCRIPTEN_KEEPALIVE
rt_err_t rt_device_register(rt_device_t dev, const char *name,
    rt_uint16_t flags);
EMSCRIPTEN_KEEPALIVE
void rt_interrupt_enter(void);
EMSCRIPTEN_KEEPALIVE
void rt_interrupt_leave(void);
EMSCRIPTEN_KEEPALIVE
void rt_kprintf(const char *fmt, ...);
#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
    EMSCRIPTEN_KEEPALIVE
    rt_device_t rt_console_set_device(const char *name);
    // rt_device_t rt_console_get_device(void);
#endif
EMSCRIPTEN_KEEPALIVE
void *rt_memcpy(void *dest, const void *src, rt_ubase_t n);
EMSCRIPTEN_KEEPALIVE
char *rt_strncpy(char *dest, const char *src, rt_ubase_t n);
EMSCRIPTEN_KEEPALIVE
void rt_show_version(void);

#ifdef RT_USING_FINSH
EMSCRIPTEN_KEEPALIVE
int finsh_system_init(void);
#endif

#ifdef RT_USING_ULOG
    EMSCRIPTEN_KEEPALIVE
    void ulog_output(rt_uint32_t level, const char *tag, rt_bool_t newline, const char *format, ...);
    EMSCRIPTEN_KEEPALIVE
    int wasm_ulog_backend_init(void);
#endif

/* WebAssembly Patch */

#define CONTEX_START                    \
struct {                                \
    rt_uint16_t state;                  \
    rt_uint16_t yield;
#define CONTEX_END                      \
} *ctx;                                 \
rt_thread_t _self = rt_thread_self();   \
ctx = _self->stack_addr;
#define CONTEX(x)                       (ctx->x)

#define THREAD_START                    \
switch (ctx->state) {
#define THREAD_STATE_START(n)           \
case n: {
#define THREAD_STATE_END_GO(n)          \
    if (ctx->yield == 0)                \
        ctx->state = n;                 \
    else                                \
        ctx->yield = 0;                 \
    return;                             \
}
#define THREAD_END                      \
default:                                \
    rt_kprintf("thread[%s]: invalid state %d", _self->name, ctx->state); \
    return;                             \
}

#define THREAD_YIELD_IN_CALLEE_PREPARE  \
{                                       \
    rt_thread_t _self = rt_thread_self(); \
    rt_uint16_t *yield = ((rt_uint16_t *)_self->stack_addr) + 1;

#define THREAD_YIELD_IN_CALLEE_FROM_LOOP \
    *yield = 1;                         \
    break;                              \
}

#ifdef __cplusplus
}
#endif

#endif /* __EXPORT_H__ */
