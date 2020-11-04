/***************************************************************************//**
 * @file    rtt_wasm.c
 * @brief   RT-Thread WebAssembly main
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#include "rtt_wasm.h"

#define _NUM_TO_STR(n)      #n
#define _DEF_TO_STR(ch)     _NUM_TO_STR(ch)

#if (CONFIG_USING_CONSOLE)
# define CONSOLE_NAME       "console"
#endif


/* === User Override Functions === */

void rt_setup(void) __attribute__((weak));
void loop(void) __attribute__((weak));

void rt_setup(void) { }
void loop(void) { rt_thread_sleep(RT_TICK_PER_SECOND); }



#if CONFIG_USING_CONSOLE

static struct rt_device console_dev;
static void *_dst_buf = RT_NULL;

static rt_err_t console_close(rt_device_t dev) {
    (void)dev;
    return RT_EOK;
}

static rt_size_t console_read(rt_device_t dev, rt_off_t pos,
    void *buffer, rt_size_t sz) {
    rt_size_t ret;
    (void)dev;
    (void)pos;
    _dst_buf = buffer;
    ret = wasm_console_read_buffer(sz);
    _dst_buf = RT_NULL;
    *(((rt_uint8_t *)buffer) + sz) = 0;
    return ret;
}

static rt_size_t console_write(rt_device_t dev, rt_off_t pos,
    const void *buffer, rt_size_t sz) {
    (void)dev;
    (void)pos;
    printf(buffer);
    return sz;
}

EMSCRIPTEN_KEEPALIVE
void wasm_console_rx_indicate(rt_size_t sz) {
    if (console_dev.rx_indicate != RT_NULL) {
        console_dev.rx_indicate(&console_dev, sz);
    }
}

EMSCRIPTEN_KEEPALIVE
void wasm_console_rx_data(char *buf, rt_size_t sz) {
    if ((_dst_buf != RT_NULL) && (buf != RT_NULL) && (sz != 0))
        rt_strncpy(_dst_buf, buf, sz);
}

static rt_err_t wasm_console_init(void) {
    rt_uint32_t flag = RT_DEVICE_FLAG_RDWR | \
                       RT_DEVICE_FLAG_STREAM | \
                       RT_DEVICE_FLAG_INT_RX;

    console_dev.type         = RT_Device_Class_Char;
    console_dev.rx_indicate  = RT_NULL;
    console_dev.tx_complete  = RT_NULL;
    console_dev.init         = RT_NULL;
    console_dev.open         = RT_NULL;
    console_dev.close        = console_close;
    console_dev.read         = console_read;
    console_dev.write        = console_write;
    console_dev.control      = RT_NULL;
    // console_dev.user_data    = RT_NULL;

    wasm_console_setup_event();

    return rt_device_register(&console_dev, CONSOLE_NAME, flag);
}

# endif /* CONFIG_USING_CONSOLE */


/* === RT-Thread Required Functions === */

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
    rt_uint8_t *stack_addr, void *texit) {
    (void)tentry;
    (void)parameter;
    (void)texit;
    *((rt_uint32_t *)stack_addr) = 0;
    return stack_addr;
}

rt_base_t rt_hw_interrupt_disable(void) {
    rt_base_t original_level;

    original_level = 1;

    return original_level;
}

void rt_hw_interrupt_enable(rt_base_t level) {
    (void)level;
}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
    // printf("rt_hw_context_switch %p %p\n", from, to);
}

void rt_hw_context_switch_to(rt_ubase_t to) {
    // printf("rt_hw_context_switch_to %p\n", to);
}

void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to) {
    // printf("rt_hw_context_switch_interrupt %p %p\n", from, to);
}

// void rt_hw_console_output(const char *str) {
//     printf(str);
// }

EMSCRIPTEN_KEEPALIVE
void wasm_tick_increase(void) {
    struct rt_thread *thread;

    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();

    thread = rt_thread_self();
    ((void (*)(void *))thread->entry)(thread->parameter);
}


/* === Initialize Conponents === */

#include "components/drivers/include/rtdevice.h"
#if CONFIG_USING_FINSH
# include "components/finsh/shell.h"
#endif
#ifdef RT_USING_DFS
# include "components/dfs/include/dfs.h"
# include "components/dfs/include/dfs_fs.h"
# include "components/dfs/filesystems/elmfat/dfs_elm.h"
#endif
#ifdef RT_USING_ULOG
# define LOG_TAG "RTT"
# include "components/utilities/ulog/ulog.h"
#else
# define LOG_E(format, args...) rt_kprintf(format "\n", ##args)
# define LOG_I(format, args...) rt_kprintf(format "\n", ##args)
#endif
#ifdef ULOG_BACKEND_USING_CONSOLE
extern int ulog_console_backend_init(void);
#endif
#ifdef RT_USING_MODULE
# include "components/libc/libdl/dlmodule.h"
#endif

#if CONFIG_USING_GUI
# include <rttgui.h>
#endif

EM_JS(void, wasm_tick_init, (), {
    console.log("wasm_tick_init called");
    setInterval(function() {
        Module.ccall('wasm_tick_increase', null, [], []);
    }, 50); // CONFIG_TICK_PER_SECOND == 20
});

/* Driver init */
void rt_driver_init(void) {
    rt_err_t ret;

    wasm_tick_init();

    #if CONFIG_USING_CONSOLE
        ret = wasm_console_init();
        RT_ASSERT(RT_EOK == ret);
        rt_console_set_device(CONSOLE_NAME);
    #endif
}

/* High level driver init */
void rt_high_driver_init(void) {
    #if CONFIG_USING_GUI
        RTT_GUI.begin();
    #endif
}

/* Component init */
void rt_components_init(void) {
    #ifdef ULOG_BACKEND_USING_CONSOLE
        (void)ulog_console_backend_init();
    #elif defined(RT_USING_ULOG)
        (void)ulog_init();
    #endif

    /* INIT_BOARD_EXPORT */
    rt_high_driver_init();

    /* INIT_PREV_EXPORT */
    #ifdef RT_USING_DFS
        (void)dfs_init();
    #endif

    /* INIT_DEVICE_EXPORT */

    /* INIT_COMPONENT_EXPORT */
    #ifdef RT_USING_DFS_ELMFAT
        (void)elm_init();
    #endif
    #ifdef RT_USING_MODULE
        // (void)rt_system_dlmodule_init();
    #endif

    /* INIT_ENV_EXPORT */
    #ifdef RT_USING_DFS_MNTTABLE
        // dfs_mount_table
    #elif defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
        if (dfs_mount(SD_NAME, "/", "elm", 0, 0)) {
            LOG_E("[E] Mount " SD_NAME " failed!");
        } else {
            LOG_I("Mount " SD_NAME " to \"/\"");
        }
    #endif

    /* INIT_APP_EXPORT */
    #if CONFIG_USING_FINSH
        finsh_system_init();
    #endif
}


/* startup thread */
void startup_thread_entry(void *param) {
    CONTEX_START
    CONTEX_END
    (void)param;

    THREAD_START

    THREAD_STATE_START(0)
        /* init components */
        rt_components_init();
        /* reset priority */
        rt_thread_control(_self, RT_THREAD_CTRL_CHANGE_PRIORITY,
            &_self->init_priority);
    THREAD_STATE_END_GO(1)

    THREAD_STATE_START(1)
        /* run thread loop here */
        loop();
        rt_thread_sleep(1);
    THREAD_STATE_END_GO(1)

    THREAD_END
}

static rt_uint8_t arduino_stack[CONFIG_STARTUP_STACK_SIZE];
static struct rt_thread startup_thread;

/* Application init */
void rt_application_init(void) {
    rt_uint32_t tmp_prio = 1;
    rt_err_t ret = rt_thread_init(
        &startup_thread, "startup",
        startup_thread_entry, RT_NULL,
        arduino_stack, sizeof(arduino_stack),
        CONFIG_STARTUP_PRIORITY, CONFIG_STARTUP_TICK);
    RT_ASSERT(RT_EOK == ret);

    /* raise priority */
    ret = rt_thread_startup(&startup_thread);
    RT_ASSERT(RT_EOK == ret);
    ret = rt_thread_control(&startup_thread, RT_THREAD_CTRL_CHANGE_PRIORITY,
        &tmp_prio);
    RT_ASSERT(RT_EOK == ret);

    (void)ret;
}


/* === Entry Point === */

static rt_uint8_t rtt_heap[CONFIG_HEAP_SIZE];

int main(void) {
    /* disable interrupt*/
    rt_hw_interrupt_disable();
    #ifdef RT_USING_HEAP
        /* init heap */
        rt_system_heap_init((void *)&rtt_heap,
            (void *)&rtt_heap[CONFIG_HEAP_SIZE-1]);
    #endif
    /* init tick */
    rt_system_tick_init();
    /* init kernel object */
    rt_system_object_init();
    /* init timer */
    rt_system_timer_init();
    /* init scheduler */
    rt_system_scheduler_init();
    #ifdef RT_USING_SIGNALS
        /* init signal */
        // rt_system_signal_init();
    #endif
    /* init driver */
    rt_driver_init();
    /* show version */
    rt_show_version();
    /* init timer thread */
    rt_system_timer_thread_init();
    /* init idle thread */
    rt_thread_idle_init();
    /* init application */
    rt_application_init();
    /* user defined init function */
    rt_setup();
    /* start scheduler */
    rt_system_scheduler_start();

    return 0;
}
