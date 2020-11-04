/***************************************************************************//**
 * @file    rtt_wasm.h
 * @brief   RT-Thread WebAssembly header
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#ifndef __RTT_WASM_H__
#define __RTT_WASM_H__

#ifdef __EMSCRIPTEN__
# include <stdlib.h>
# include <stdio.h>
#endif

#include "include/rtthread.h"
#include "include/rthw.h"
#ifdef RT_USING_DFS
# include "components/dfs/include/dfs_posix.h"
#endif

/* Will be imported from JavaScript */
extern void wasm_console_setup_event(void);
extern rt_size_t wasm_console_read_buffer(rt_size_t sz);

#endif /*__RTT_WASM_H__ */
