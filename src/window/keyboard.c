#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include <sys/mman.h>
#include <unistd.h>

#include <syslog.h>

#include "./window.h"

#include "pong/pong.h"
#include "pong/memory.h"

static void capabilities_event(void *null, struct wl_seat *seat, unsigned capabilities);
static void name_event(void *null, struct wl_seat *seat, const char *name);

static void keymap_event(void *null, struct wl_keyboard *keyboard, unsigned format, int fd, unsigned size);
static void enter_event(void *null, struct wl_keyboard *keyboard, unsigned serial, struct wl_surface *surface, struct wl_array *keys);
static void leave_event(void *null, struct wl_keyboard *keyboard, unsigned serial, struct wl_surface *surface);
static void key_event(void *null, struct wl_keyboard *keyboard, unsigned serial, unsigned time, unsigned scancode, unsigned state);
static void modifiers_event(void *null, struct wl_keyboard *keyboard, unsigned serial, unsigned depressed, unsigned latched, unsigned locked, unsigned group);
static void repeat_event(void *null, struct wl_keyboard *keyboard, int rate, int delay);

static const struct wl_seat_listener seat_listener = {
    .capabilities = capabilities_event,
    .name = name_event,
};

static const struct wl_keyboard_listener listener = {
    .keymap = keymap_event,
    .enter = enter_event,
    .leave = leave_event,
    .key = key_event,
    .modifiers = modifiers_event,
    .repeat_info = repeat_event,
};

void pong_setup_seat() {
    pong_add_cleanup((PONG_CLEANUP) wl_seat_release, wayland.keyboard.seat);

    pong_wayland_check(wl_seat_add_listener(wayland.keyboard.seat, &seat_listener, NULL) != -1);
}

void pong_create_keyboard() {
    syslog(LOG_INFO, "info: creating xkb context\n");

    wayland.keyboard.context = xkb_context_new(0);

    if (wayland.keyboard.context == NULL)
        pong_finish();

    pong_add_cleanup((PONG_CLEANUP) xkb_context_unref, wayland.keyboard.context);
}

// TODO: Unref everything
static void keymap_event(void *null, struct wl_keyboard *keyboard, unsigned format, int fd, unsigned size) {
    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) return;

    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

    close(fd);

    if (data == NULL)
        pong_finish();

    wayland.keyboard.keymap = xkb_keymap_new_from_string(wayland.keyboard.context, data, XKB_KEYMAP_FORMAT_TEXT_V1, 0);

    munmap(data, size);

    if (wayland.keyboard.keymap == NULL)
        pong_finish();

    wayland.keyboard.state = xkb_state_new(wayland.keyboard.keymap);

    if (wayland.keyboard.state == NULL)
        pong_finish();
}

static void enter_event(void *null, struct wl_keyboard *keyboard, unsigned serial, struct wl_surface *surface, struct wl_array *keys) {

}

static void leave_event(void *null, struct wl_keyboard *keyboard, unsigned serial, struct wl_surface *surface) {

}

static void key_event(void *null, struct wl_keyboard *keyboard, unsigned serial, unsigned time, unsigned scancode, unsigned state) {
    static xkb_keysym_t key = 0;

    if (wayland.keyboard.state == NULL) {
        syslog(LOG_ERR, "error: no supported keymap found\n");

        pong_finish();
    }

    key = xkb_state_key_get_one_sym(wayland.keyboard.state, scancode + 8);

    switch(key) {
        case XKB_KEY_Up:
            {
                pong_right_action_up(state != WL_KEYBOARD_KEY_STATE_RELEASED);
                break;
            }

        case XKB_KEY_Down:
            {
                pong_right_action_down(state != WL_KEYBOARD_KEY_STATE_RELEASED);
                break;
            }

        case XKB_KEY_w:
            {
                pong_left_action_up(state != WL_KEYBOARD_KEY_STATE_RELEASED);
                break;
            }

        case XKB_KEY_s:
            {
                pong_left_action_down(state != WL_KEYBOARD_KEY_STATE_RELEASED);
                break;
            }

        default:
            break;
    }
}

static void modifiers_event(void *null, struct wl_keyboard *keyboard, unsigned serial, unsigned depressed, unsigned latched, unsigned locked, unsigned group) {

}

static void repeat_event(void *null, struct wl_keyboard *keyboard, int rate, int delay) {

}

static void capabilities_event(void *null, struct wl_seat *seat, unsigned capabilities) {
    if (!(capabilities & WL_SEAT_CAPABILITY_KEYBOARD)) return;

    wayland.keyboard.handle = wl_seat_get_keyboard(seat);

    pong_wayland_check(wayland.keyboard.handle != NULL);

    pong_add_cleanup((PONG_CLEANUP) wl_keyboard_release, wayland.keyboard.handle);

    pong_wayland_check(wl_keyboard_add_listener(wayland.keyboard.handle, &listener, NULL) != -1);
}

static void name_event(void *null, struct wl_seat *seat, const char *name) {

}
