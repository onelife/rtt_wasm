/***************************************************************************//**
 * @file    console_be.c
 * @brief   RT-Thread WebAssembly ulog backend
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#include "include/rtthread.h"

#if defined(__EMSCRIPTEN__) && defined(RT_USING_ULOG)

#include "components/utilities/ulog/ulog.h"

/***************************************************************************//**
 * @addtogroup WebAssembly
 * @{
 ******************************************************************************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct ulog_backend console;

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void wasm_ulog_backend_output(struct ulog_backend *be, rt_uint32_t lvl,
    const char *tag, rt_bool_t is_raw, const char *log, rt_size_t len) {
    int flags = EM_LOG_CONSOLE;
    (void)be;
    (void)tag;
    (void)is_raw;
    (void)len;

    if (lvl <= LOG_LVL_ASSERT)
        flags |= EM_LOG_ERROR;
    else if (lvl <= LOG_LVL_ERROR)
        flags |= EM_LOG_ERROR;
    else if (lvl <= LOG_LVL_WARNING)
        flags |= EM_LOG_WARN;
    else if (lvl <= LOG_LVL_INFO)
        flags |= EM_LOG_INFO;
    else if (lvl <= LOG_LVL_DBG)
        flags |= EM_LOG_DEBUG;

    emscripten_log(flags, log);
}

int wasm_ulog_backend_init(void) {
    ulog_init();
    console.output = wasm_ulog_backend_output;
    ulog_backend_register(&console, "console", RT_FALSE);

    return 0;
}

/***************************************************************************//**
 * @}
 ******************************************************************************/

#endif /* defined(__EMSCRIPTEN__) && define(RT_USING_ULOG) */
