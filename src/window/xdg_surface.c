#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

#include <syslog.h>

#include "./window.h"

#include "pong/memory.h"

static void ping_event(void *null, struct xdg_wm_base *base, unsigned serial);

static void configure_event(void *null, struct xdg_surface *surface, unsigned serial);

static const struct xdg_wm_base_listener base_listener = {
    .ping = ping_event,
};

static const struct xdg_surface_listener listener = {
    .configure = configure_event,
};

void pong_setup_xdg_base() {
    pong_add_cleanup((PONG_CLEANUP) xdg_wm_base_destroy, wayland.xdg.base);

    pong_wayland_check(xdg_wm_base_add_listener(wayland.xdg.base, &base_listener, NULL) != -1);
}

void pong_create_xdg_surface() {
    syslog(LOG_INFO, "info: getting xdg surface\n");

    wayland.xdg.surface = xdg_wm_base_get_xdg_surface(wayland.xdg.base, wayland.surface);

    pong_wayland_check(wayland.xdg.surface != NULL);

    pong_add_cleanup((PONG_CLEANUP) xdg_surface_destroy, wayland.xdg.surface);

    pong_wayland_check(xdg_surface_add_listener(wayland.xdg.surface, &listener, NULL) != -1);
}

static void configure_event(void *null, struct xdg_surface *surface, unsigned serial) {
    xdg_surface_ack_configure(surface, serial);
}

static void ping_event(void *null, struct xdg_wm_base *base, unsigned serial) {
    xdg_wm_base_pong(base, serial);
}
