#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

#include <syslog.h>

#include "./window.h"
#include "pong/window.h"

#include "pong/memory.h"
#include "pong/math.h"

static void configure_event(void *null, struct xdg_toplevel *toplevel, int width, int height, struct wl_array *states);
static void close_event(void *null, struct xdg_toplevel *toplevel);
static void bounds_event(void *null, struct xdg_toplevel *toplevel, int width, int height);
static void capabilities_event(void *null, struct xdg_toplevel *toplevel, struct wl_array *capabilities);

static const struct xdg_toplevel_listener listener = {
    .configure = configure_event,
    .close = close_event,
    .configure_bounds = bounds_event,
    .wm_capabilities = capabilities_event,
};

void pong_create_xdg_toplevel() {
    syslog(LOG_INFO, "info: getting xdg toplevel\n");

    wayland.xdg.toplevel = xdg_surface_get_toplevel(wayland.xdg.surface);

    pong_wayland_check(wayland.xdg.toplevel != NULL);

    pong_add_cleanup((PONG_CLEANUP) xdg_toplevel_destroy, wayland.xdg.toplevel);

    pong_wayland_check(xdg_toplevel_add_listener(wayland.xdg.toplevel, &listener, NULL) != -1);

    xdg_toplevel_set_title(wayland.xdg.toplevel, "pong");
    xdg_toplevel_set_app_id(wayland.xdg.toplevel, "pong");
}

static void configure_event(void *null, struct xdg_toplevel *toplevel, int width, int height, struct wl_array *states) {
    window.width = (width == 0 ? pong_math_imin(1000, wayland.window.width_bound) : width);
    window.height = (height == 0 ? pong_math_imin(700, wayland.window.height_bound) : height);

    // No resizing
    xdg_toplevel_set_min_size(toplevel, window.width, window.height);
    xdg_toplevel_set_max_size(toplevel, window.width, window.height);
}

static void close_event(void *null, struct xdg_toplevel *toplevel) {
    window.closed = true;
}

static void bounds_event(void *null, struct xdg_toplevel *toplevel, int width, int height) {
    wayland.window.width_bound = (width == 0 ? -1 : width);
    wayland.window.height_bound = (height == 0 ? -1 : height);
}

static void capabilities_event(void *null, struct xdg_toplevel *toplevel, struct wl_array *capabilities) {

}
