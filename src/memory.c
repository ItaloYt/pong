#include <stdlib.h>

#include <syslog.h>

#include "pong/memory.h"

struct Cleanup {
    void *data;
    PONG_CLEANUP cleanup;

    struct Cleanup *previous;
};

struct Cleanup *tail;

void pong_add_cleanup(PONG_CLEANUP cleanup, void *data) {
    if (cleanup == NULL) return;

    struct Cleanup *last = malloc(sizeof(struct Cleanup));

    if (last == NULL) {
        syslog(LOG_ERR, "error: failed to add cleanup\n");

        pong_finish();
    }

    last->data = data;
    last->cleanup = cleanup;
    last->previous = tail;

    tail = last;
}

void pong_cleanup() {
    struct Cleanup *current = tail;

    syslog(LOG_INFO, "info: cleaning\n");

    while (tail) {
        tail->cleanup(tail->data);
    
        tail = tail->previous;
        
        free(current);

        current = tail;
    }
}

void pong_finish() {
    pong_cleanup();

    exit(1);
}
