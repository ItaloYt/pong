#ifndef PONG_MEMORY_H
#define PONG_MEMORY_H

typedef void (*PONG_CLEANUP)(void *data);

void pong_add_cleanup(PONG_CLEANUP cleanup, void *data);

void pong_cleanup();
void pong_finish();

#endif
