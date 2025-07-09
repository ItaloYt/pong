#include <wayland-client.h>

#include <syslog.h>

#include "pong/window.h"
#include "./window.h"

#include "pong/memory.h"

struct Wayland wayland;
struct Window window;

void pong_window_create() {
    pong_connect_display();
    pong_create_xdg_surface();
    pong_create_xdg_toplevel();
    pong_create_xdg_decoration();
    pong_create_keyboard();

    wl_surface_commit(wayland.surface);

    pong_wayland_check(wl_display_roundtrip(wayland.display) != -1);

    wl_surface_commit(wayland.surface);
}

void pong_window_update() {
    pong_wayland_check(wl_display_dispatch(wayland.display) != -1);
}

void pong_wayland_check(bool condition) {
    if (condition) return;

    syslog(LOG_ERR, "error: failed with code %u\n", wl_display_get_error(wayland.display));

    pong_finish();
}
