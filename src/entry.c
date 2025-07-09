#include <stdbool.h>

#include <time.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>

#include "pong/pong.h"

#include "pong/window.h"
#include "pong/memory.h"
#include "pong/vulkan.h"

static void pong_close_log(void *null) {
    syslog(LOG_INFO, "info: closing log\n");

    closelog();
}

int main() {
    (void) srand(time(NULL));

    openlog("pong", LOG_CONS | LOG_NDELAY, LOG_USER);

    pong_add_cleanup(pong_close_log, NULL);

    pong_window_create();

    clock_t time0 = clock();

    pong_vulkan_create();

    pong_start();

    pong_vulkan_setup_wait_idle();

    clock_t time1 = clock();

    float delta = 0;

    while (!window.closed) {
        delta = (float) (time1 - time0) / CLOCKS_PER_SEC;

        time0 = clock();

        pong_window_update();

        pong_vulkan_draw();

        pong_update(delta);

        time1 = clock();
    }

    pong_cleanup();

    return 0;
}

void pong_check(bool condition) {
    if (condition) return;

    syslog(LOG_ERR, "error: errno: %d\n", errno);

    pong_finish();
}
