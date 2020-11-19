/***************************************************************************//**
 * @file    rtconfig.h
 * @brief   RT-Thread WebAssembly config
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#ifndef __RTCONFIG_H__
#define __RTCONFIG_H__

#ifdef __EMSCRIPTEN__
# include <emscripten.h>
#endif


/* User Config */

/* WebAssembly Config */

#define CONFIG_CONSOLE_BUFFER_SIZE      (128)
#define CONFIG_TICK_PER_SECOND          (20)

#ifndef CONFIG_HEAP_SIZE
# define CONFIG_HEAP_SIZE              	(64 * 1024)
#endif


/* Default User Config */

#ifndef CONFIG_USING_DRIVER_RTC
# define CONFIG_USING_DRIVER_RTC        (0)
#endif

#ifndef CONFIG_USING_CONSOLE
# define CONFIG_USING_CONSOLE           (1)
#endif

#ifndef CONFIG_USING_FINSH
# define CONFIG_USING_FINSH             (CONFIG_USING_CONSOLE)
#endif

#ifndef CONFIG_USING_MSH
# define CONFIG_USING_MSH               (1)
#endif

#ifndef CONFIG_USING_LOG
# define CONFIG_USING_LOG               (1)
#endif

#ifndef CONFIG_USING_MODULE
# define CONFIG_USING_MODULE            (0)
#endif

#ifndef CONFIG_USING_GUI
# define CONFIG_USING_GUI               (0)
#endif

#ifndef CONFIG_USING_RAMFS
# define CONFIG_USING_RAMFS             (0)
#endif

#ifndef CONFIG_USING_MEMFS
# define CONFIG_USING_MEMFS             (0)
#endif

#ifndef CONFIG_USING_IDBFS
# define CONFIG_USING_IDBFS             (0)
#endif

#ifndef CONFIG_USING_EXFAT
# define CONFIG_USING_EXFAT             (0)
#endif


/* Config Check */

#if (CONFIG_USING_LOG)
# define RT_USING_ULOG
# define ULOG_OUTPUT_LVL                (LOG_LVL_DBG) // (LOG_LVL_INFO)
# define ULOG_ASSERT_ENABLE
// # define ULOG_USING_COLOR
// # define ULOG_USING_ISR_LOG
// # define ULOG_USING_SYSLOG
// # define ULOG_USING_FILTER
# define ULOG_OUTPUT_TIME
// # define ULOG_TIME_USING_TIMESTAMP
# define ULOG_OUTPUT_LEVEL
# define ULOG_OUTPUT_TAG
# define ULOG_OUTPUT_THREAD_NAME
# ifdef ULOG_BACKEND_USING_CONSOLE
#  undef ULOG_BACKEND_USING_CONSOLE
# endif 
#endif /* CONFIG_USING_LOG */

#if (CONFIG_USING_MODULE)
# define RT_USING_MODULE
# define MODULE_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX - 1)
# define MODULE_THREAD_STACK_SIZE       (4 * 512)
# define IDLE_THREAD_STACK_SIZE         (1 * 512)
#endif /* CONFIG_USING_MODULE */

#if (0)
# if (CONFIG_USING_DRIVER_SPI)
#  define RT_USING_SPI_MSD
# endif /* CONFIG_USING_DRIVER_SPI */
#endif /* 0 */

#if (CONFIG_USING_GUI)
# ifndef CONFIG_GUI_WIDTH
#  error "Please define CONFIG_GUI_WIDTH"
# endif
# ifndef CONFIG_GUI_HIGH
#  error "Please define CONFIG_GUI_HIGH"
# endif
#endif /* CONFIG_USING_GUI */


/* Debug Config */

// #define RT_DEBUG
// #define RT_USING_OVERFLOW_CHECK
// #define RT_USING_MEMTRACE
// #define RT_DEBUG_INIT                   (1)
// #define RT_DEBUG_MEM                    (1)
// #define RT_DEBUG_SCHEDULER              (1)
// #define RT_DEBUG_IPC                    (1)
// #define RT_DEBUG_TIMER                  (1)
// #define RT_DEBUG_THREAD                 (1)


/* System Config */

#define RT_NAME_MAX                     (16)
#define RT_ALIGN_SIZE                   (4)
#define RT_THREAD_PRIORITY_MAX          (32)
#define RT_TICK_PER_SECOND              (CONFIG_TICK_PER_SECOND)
#ifdef __EMSCRIPTEN__
# define ARCH_CPU_STACK_GROWS_UPWARD
#endif


/* Startup Thread Config */

#ifndef CONFIG_STARTUP_STACK_SIZE
# define CONFIG_STARTUP_STACK_SIZE      (128)
#endif
#ifndef CONFIG_STARTUP_PRIORITY
# define CONFIG_STARTUP_PRIORITY        (RT_THREAD_PRIORITY_MAX >> 1)
#endif
#ifndef CONFIG_STARTUP_TICK
# define CONFIG_STARTUP_TICK            (10)
#endif


/* Timer Config */

// #define RT_USING_TIMER_SOFT
// #define RT_TIMER_THREAD_PRIO            (4)
// #define RT_TIMER_THREAD_STACK_SIZE      (512)


/* Utility Config */

#define RT_USING_DEVICE                 /* Required by IPC, DRV */
#define RT_USING_MUTEX                  /* Required by DFS, DRV */
#define RT_USING_SEMAPHORE              /* Required by FINSH */
#define RT_USING_MAILBOX                /* Required by GUI */
// #define RT_USING_MESSAGEQUEUE           /* Required by ? */
// #define RT_USING_EVENT                  /* Required by ? */
// #define RT_USING_SIGNALS                /* Required by ? */
// #define RT_USING_HOOK
// #define RT_USING_IDLE_HOOK


/* Memory Management Config */

#define RT_USING_MEMPOOL                /* Required by SIG, GUI */
#if (CONFIG_USING_RAMFS)
# define RT_USING_MEMHEAP               /* Required by ramfs */
#endif
#define RT_USING_HEAP
#define RT_USING_SMALL_MEM


/* RTT Driver Config */

#if 0
# define RT_USING_DEVICE_IPC
# define RT_USING_SERIAL
# define RT_USING_SPI
# define RT_USING_RTC
# define RT_USING_ALARM
#endif


/* Console Config */

#if (CONFIG_USING_CONSOLE)
# define RT_USING_CONSOLE
# define RT_CONSOLEBUF_SIZE             (128)
#endif /* CONFIG_USING_CONSOLE */


/* FinSH Config */

#if (CONFIG_USING_FINSH)
#define RT_USING_FINSH
# if (CONFIG_USING_MSH)
#  define FINSH_USING_MSH
#  define FINSH_USING_MSH_ONLY
# endif
#define FINSH_USING_DESCRIPTION
#define FINSH_USING_HISTORY
#define FINSH_THREAD_PRIORITY           ((RT_THREAD_PRIORITY_MAX >> 1) + (RT_THREAD_PRIORITY_MAX >> 3))
#define FINSH_THREAD_STACK_SIZE         (128)
#endif /* CONFIG_USING_FINSH */


/* File System Config */

#if (CONFIG_USING_MEMFS || CONFIG_USING_RAMFS || CONFIG_USING_IDBFS)
# define RT_USING_DFS
// # define RT_USING_DFS_MNTTABLE          /* Mount table */
# define DFS_USING_WORKDIR
# define DFS_FILESYSTEMS_MAX            (4)     /* Max number of fs */
# define DFS_FD_MAX                     (4)     /* Max number of open file */
# if (CONFIG_USING_RAMFS)
#  define RT_USING_DFS_RAMFS
#  define CONFIG_RAMFS_SIZE             (1024)
# endif
# if (CONFIG_USING_MEMFS)
#  define RT_USING_DFS_MEMFS
# endif
# if (CONFIG_USING_IDBFS)
#  define RT_USING_DFS_IDBFS
# endif
// # define RT_USING_DFS_ELMFAT
// # define RT_DFS_ELM_CODE_PAGE           437     /* (xxx) is wrong -_-! */
// # if (CONFIG_USING_EXFAT)
// #  define RT_DFS_ELM_USE_EXFAT
// # endif
// # if defined(RT_DFS_ELM_USE_EXFAT) || (RT_DFS_ELM_CODE_PAGE >= 900)
// #  define RT_DFS_ELM_USE_LFN            (2)
// #  define RT_DFS_ELM_MAX_LFN            (255)
// // #  define RT_DFS_ELM_LFN_UNICODE
// # endif /* defined(RT_DFS_ELM_USE_EXFAT) || (RT_DFS_ELM_CODE_PAGE >= 900) */
#endif /* (CONFIG_USING_MEMFS || CONFIG_USING_RAMFS || CONFIG_USING_IDBFS) */


/* Unsupported Config */

#ifdef FINSH_USING_SYMTAB
# undef FINSH_USING_SYMTAB              /* Reason: no access to linker script */
#endif
#ifdef RT_USING_COMPONENTS_INIT
# undef RT_USING_COMPONENTS_INIT        /* Reason: no access to linker script */
#endif


/* Libary Function Replacement */

#define rt_strcpy(dst, src) rt_strncpy(dst, src, rt_strlen(src) + 1)
#define rt_strcat(dst, src) rt_strcpy((char *)(dst) + rt_strlen(dst), src)


#ifdef __EMSCRIPTEN__
# include "export.h"
#else
# define EMSCRIPTEN_KEEPALIVE
# define CONTEX_START
# define CONTEX_END
# define CONTEX(x) x
# define THREAD_START(n)
# define THREAD_STATE_END_GO(n)
# define THREAD_END
# define THREAD_YIELD_IN_CALLEE_PREPARE
# define THREAD_YIELD_IN_CALLEE_FROM_LOOP
#endif

#endif /* __RTCONFIG_H__ */
