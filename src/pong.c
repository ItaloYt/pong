#include <stdlib.h>
#include <stdbool.h>

#include <syslog.h>

#include "pong/pong.h"

#include "pong/vulkan.h"
#include "pong/math.h"

// [0] -> Right
// [1] -> Ball
// [2] -> Left
static struct PongVkObject *objects;

struct {
    bool up;
    bool down;
} right, left;

static fvec2 ball_speed = { 700, 700 };

static unsigned width;
static unsigned height;

static struct PongTimer timer;

static void pong_setup_objects() {
    objects[0] = (struct PongVkObject) {
        .position = { width - 25, height / 2.0 - 50 },
        .size = { 25, 100 },
    };

    objects[1] = (struct PongVkObject) {
        .position = { width / 2.0 - 12.5, height / 2.0 - 12.5 },
        .size = { 25, 25 },
    };

    objects[2] = (struct PongVkObject) {
        .position = { 0, height / 2.0 - 50 },
        .size = { 25, 100 },
    };
    
    ball_speed[0] *= (rand() % 2 == 0 ? 1 : -1);
    ball_speed[1] *= (rand() % 2 == 0 ? 1 : -1);

    pong_timer_set(&timer, 1);
}

void pong_start() {
    width = pong_vulkan_get_width();
    height = pong_vulkan_get_height();

    pong_vulkan_create_objects(&objects, 3);

    pong_setup_objects();
}

void pong_update(float delta) {
    if (pong_timer_check(&timer)) {
        objects[1].position[0] += ball_speed[0] * delta;
        objects[1].position[1] += ball_speed[1] * delta;
    }

    if (objects[1].position[0] <= 0) {
        syslog(LOG_INFO, "info: right player scored!\n");

        pong_setup_objects();

        return;
    }

    if (objects[1].position[0] >= width - objects[1].size[0]) {
        syslog(LOG_INFO, "info: left player scored!");

        pong_setup_objects();

        return;
    }

    // Collision and ball-player
    if (pong_math_is_colliding((struct PongMathBox *) (objects + 1), (struct PongMathBox *) (objects + 0))) {
        objects[1].position[0] = objects[0].position[0] - objects[1].size[0];

        ball_speed[0] *= -1;
    }

    if (pong_math_is_colliding((struct PongMathBox *) (objects + 1), (struct PongMathBox *) (objects + 2))) {
        objects[1].position[0] = objects[2].position[0] + objects[2].size[0];

        ball_speed[0] *= -1;
    }

    if (objects[1].position[1] <= 0 || objects[1].position[1] >= height - objects[1].size[1]) {
        objects[1].position[1] = pong_math_fclamp(objects[1].position[1], 0, height - objects[1].size[1]);

        ball_speed[1] *= -1;
    }

    // Collision players-wall
    if (right.up ^ right.down)
        objects[0].position[1] = pong_math_fclamp(objects[0].position[1] + (right.up ? -1 : 1) * 500 * delta, 0, height - objects[0].size[1]);

    if (left.up ^ left.down)
        objects[2].position[1] = pong_math_fclamp(objects[2].position[1] + (left.up ? -1 : 1) * 500 * delta, 0, height - objects[2].size[1]);
}

void pong_right_action_up(bool value) {
    right.up = value;
}

void pong_right_action_down(bool value) {
    right.down = value;
}

void pong_left_action_up(bool value) {
    left.up = value;
}

void pong_left_action_down(bool value) {
    left.down = value;
}
