#ifndef PONG_H
#define PONG_H

#include <stdbool.h>
#include <time.h>

struct PongTimer {
    time_t start;
    float time;
};

void pong_start();
void pong_update(float delta);

void pong_check(bool condition);

void pong_right_action_up(bool value);
void pong_right_action_down(bool value);
void pong_left_action_up(bool value);
void pong_left_action_down(bool value);

static inline void pong_timer_set(struct PongTimer *timer, float time);
static inline bool pong_timer_check(const struct PongTimer *timer);

static inline void pong_timer_set(struct PongTimer *timer, float time) {
    timer->start = clock();
    timer->time = time;
}

static inline bool pong_timer_check(const struct PongTimer *timer) {
    return (float) (clock() - timer->start) / CLOCKS_PER_SEC >= timer->time;
}

#endif
