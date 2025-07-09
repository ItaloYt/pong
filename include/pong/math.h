#ifndef PONG_MATH_H
#define PONG_MATH_H

#include <stdbool.h>

typedef float fvec2[2];
typedef float fmat4[16];

struct PongMathBox {
    fvec2 position;
    fvec2 size;
};

static inline float pong_math_fmin(float a, float b);
static inline float pong_math_fmax(float a, float b);
static inline float pong_math_fclamp(float value, float min, float max);

static inline int pong_math_imin(int a, int b);
static inline int pong_math_imax(int a, int b);
static inline int pong_math_iclamp(int value, int min, int max);

static inline bool pong_math_is_colliding(const struct PongMathBox *a, const struct PongMathBox *b);

static inline float pong_math_fmin(float a, float b) {
    return (a < b ? a : b);
}

static inline float pong_math_fmax(float a, float b) {
    return (a > b ? a : b);
}

static inline float pong_math_fclamp(float value, float min, float max) {
    return pong_math_fmax(pong_math_fmin(value, max), min);
}

static inline int pong_math_imin(int a, int b) {
    return (a < b ? a : b);
}

static inline int pong_math_imax(int a, int b) {
    return (a > b ? a : b);
}

static inline int pong_math_iclamp(int value, int min, int max) {
    return pong_math_imax(pong_math_imin(value, max), min);
}

static inline bool pong_math_is_colliding(const struct PongMathBox *a, const struct PongMathBox *b) {
    return 
        (a->position[0] <= b->position[0] + b->size[0] && a->position[0] + a->size[0] >= b->position[0]) &&
        (a->position[1] <= b->position[1] + b->size[1] && a->position[1] + a->size[1] >= b->position[1]);
}

#endif
