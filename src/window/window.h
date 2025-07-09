#ifndef LOCAL_PONG_WINDOW_H
#define LOCAL_PONG_WINDOW_H

#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <xdg-decoration-unstable-v1-client-protocol.h>
#include <xkbcommon/xkbcommon.h>

#include <stdbool.h>

struct Wayland {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_surface *surface;

    struct {
        struct xdg_wm_base *base;
        struct xdg_surface *surface;
        struct xdg_toplevel *toplevel;
        struct zxdg_decoration_manager_v1 *manager;
        struct zxdg_toplevel_decoration_v1 *decoration;
    } xdg;

    struct {
        unsigned width_bound;
        unsigned height_bound;
    } window;

    struct {
        struct wl_seat *seat;
        struct wl_keyboard *handle;
        struct xkb_context *context;
        struct xkb_keymap *keymap;
        struct xkb_state *state;
    } keyboard;
};

extern struct Wayland wayland;

void pong_connect_display();
void pong_setup_xdg_base();
void pong_setup_xdg_manager();
void pong_setup_seat();
void pong_create_xdg_surface();
void pong_create_xdg_toplevel();
void pong_create_xdg_decoration();
void pong_create_keyboard();

void pong_wayland_check(bool condition);

#endif
