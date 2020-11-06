/***************************************************************************//**
 * @file    rtt_main.c
 * @brief   RT-Thread WebAssembly main
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#include <string.h>
#include "rtt_wasm.h"

static worker_handle worker;
static rt_size_t input_sz = 0;
static char input_buf[CONFIG_CONSOLE_BUFFER_SIZE + 1] = { 0 };


void wasm_concole_output_cb(char *buf, int sz, void *arg) {
    (void)arg;
    if ((buf != NULL) && (sz != 0)) {
        printf("%s", buf);
    }
}

void wasm_concole_update(void) {
    if (input_sz != 0) {
        emscripten_call_worker(worker, "wasm_concole_input", input_buf, input_sz, NULL, NULL);
        input_sz = 0;
    }
    emscripten_call_worker(worker, "wasm_concole_output", NULL, 0, wasm_concole_output_cb, NULL);
}

EMSCRIPTEN_KEEPALIVE
void wasm_user_input(char *buf, rt_size_t sz) {
    if ((buf != RT_NULL) && (sz != 0)) {
        input_sz = (sz > CONFIG_CONSOLE_BUFFER_SIZE) ? CONFIG_CONSOLE_BUFFER_SIZE : sz;
        memcpy(input_buf, buf, input_sz + 1);
    }
}

int main(void) {
    worker = emscripten_create_worker("rtt_worker.js");
    wasm_console_setup_event();
    emscripten_set_main_loop(wasm_concole_update, CONFIG_TICK_PER_SECOND, 0); 

    return 0;
}
