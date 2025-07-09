#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <xdg-decoration-unstable-v1-client-protocol.h>

#include <string.h>

#include <syslog.h>

#include "./window.h"

#include "pong/memory.h"

// static void error_event(void *null, struct wl_display *display, void *object, unsigned error, const char *message);
// static void delete_event(void *null, struct wl_display *display, unsigned name);

static void global_event(void *null, struct wl_registry *registry, unsigned name, const char *interface, unsigned version);
static void remove_event(void *null, struct wl_registry *registry, unsigned name);

// static const struct wl_display_listener display_listener = {
//     .error = error_event,
//     .delete_id = delete_event,
// };

static const struct wl_registry_listener listener = {
    .global = global_event,
    .global_remove = remove_event,
};

void pong_connect_display() {
    syslog(LOG_INFO, "info: connecting display\n");

    wayland.display = wl_display_connect(NULL);

    // Since display isn't connected yet, we can't use pong_wayland_check
    if (wayland.display == NULL) {
        syslog(LOG_ERR, "error: failed\n");

        pong_finish();
    }

    pong_add_cleanup((PONG_CLEANUP) wl_display_disconnect, wayland.display);

    // pong_wayland_check(wl_display_add_listener(wayland.display, &display_listener, NULL) != -1);

    syslog(LOG_INFO, "info: getting registry\n");

    wayland.registry = wl_display_get_registry(wayland.display);

    pong_wayland_check(wayland.registry != NULL);

    pong_wayland_check(wl_registry_add_listener(wayland.registry, &listener, NULL) != -1);

    pong_wayland_check(wl_display_roundtrip(wayland.display) > -1);

    if (wayland.compositor == NULL) {
        syslog(LOG_ERR, "error: compositor not found\n");

        pong_finish();
    }

    if (wayland.xdg.base == NULL) {
        syslog(LOG_ERR, "error: xdg base not found\n");

        pong_finish();
    }

    if (wayland.xdg.manager == NULL) {
        syslog(LOG_ERR, "error: xdg decoration manager not found\n");

        pong_finish();
    }

    syslog(LOG_INFO, "info: creating surface\n");

    wayland.surface = wl_compositor_create_surface(wayland.compositor);

    pong_wayland_check(wayland.surface != NULL);
}

// IMPORTANT: for every interface binded here, we need to check in pong_connect_display if it was found or not
static void global_event(void *null, struct wl_registry *registry, unsigned name, const char *interface, unsigned version) {
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        syslog(LOG_INFO, "info: binding compositor\n");

        wayland.compositor = wl_registry_bind(registry, name, &wl_compositor_interface, version);

        pong_wayland_check(wayland.compositor != NULL);

        return;
    }

    if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        syslog(LOG_INFO, "info: binding xdg base\n");

        wayland.xdg.base = wl_registry_bind(registry, name, &xdg_wm_base_interface, version);

        pong_wayland_check(wayland.xdg.base != NULL);

        pong_setup_xdg_base();

        return;
    }

    if (strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
        syslog(LOG_INFO, "info: binding xdg decoration manager\n");

        wayland.xdg.manager = wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, version);

        pong_wayland_check(wayland.xdg.manager != NULL);

        pong_setup_xdg_manager();

        return;
    }

    if (strcmp(interface, wl_seat_interface.name) == 0) {
        syslog(LOG_INFO, "info: binding seat\n");

        wayland.keyboard.seat = wl_registry_bind(registry, name, &wl_seat_interface, version);

        pong_wayland_check(wayland.keyboard.seat != NULL);

        pong_setup_seat();

        return;
    }
}

static void remove_event(void *null, struct wl_registry *registry, unsigned name) {

}

// static void error_event(void *null, struct wl_display *display, void *object, unsigned error, const char *message) {
//     syslog(LOG_ERR, "error: wayland: error=%u, message=%s\n", error, message);
// }
//
// static void delete_event(void *null, struct wl_display *display, unsigned name) {
//
// }
