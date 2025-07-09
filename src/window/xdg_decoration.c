#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <xdg-decoration-unstable-v1-client-protocol.h>

#include <syslog.h>

#include "./window.h"

#include "pong/memory.h"

static void configure_event(void *null, struct zxdg_toplevel_decoration_v1 *decoration, unsigned mode);

static const struct zxdg_toplevel_decoration_v1_listener listener = {
    .configure = configure_event,
};

void pong_setup_xdg_manager() {
    pong_add_cleanup((PONG_CLEANUP) zxdg_decoration_manager_v1_destroy, wayland.xdg.manager);
}

void pong_create_xdg_decoration() {
    syslog(LOG_INFO, "info: getting xdg decoration\n");

    wayland.xdg.decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(wayland.xdg.manager, wayland.xdg.toplevel);

    pong_wayland_check(wayland.xdg.decoration != NULL);

    pong_add_cleanup((PONG_CLEANUP) zxdg_toplevel_decoration_v1_destroy, wayland.xdg.decoration);

    pong_wayland_check(zxdg_toplevel_decoration_v1_add_listener(wayland.xdg.decoration, &listener, NULL) != -1);

    zxdg_toplevel_decoration_v1_set_mode(wayland.xdg.decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
}

static void configure_event(void *null, struct zxdg_toplevel_decoration_v1 *decoration, unsigned mode) {
    if (mode != ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE) {
        syslog(LOG_ERR, "error: client-side decoration not supported\n");

        pong_finish();
    }
}
