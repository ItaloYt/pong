#ifndef PONG_WINDOW_H
#define PONG_WINDOW_H

#include <stdbool.h>

struct Window {
    unsigned width;
    unsigned height;

    bool closed;
};

extern struct Window window;

void pong_window_create();
void pong_window_update();

#endif
