#pragma once

#include <raylib.h>
#include <raymath.h>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

constexpr int ENTITY_AMOUNT_X = 4096;
constexpr int ENTITY_AMOUNT_Y = 4096;
constexpr int ENTITY_RADIUS = 2;
constexpr int PLAYER_RADIUS = 64;
constexpr float PLAYER_SPEED = 256.0f;

constexpr int ENTITY_COUNT = ENTITY_AMOUNT_X * ENTITY_AMOUNT_Y;

// TEMP //
#include "rlgl.h"

constexpr int CIRCLE_SEGMENTS = 16; // tweak for speed vs quality

inline void draw_circle_outline(float cx, float cy, float r, Color c) {
    rlColor4ub(c.r, c.g, c.b, c.a);

    for (int i = 0; i < CIRCLE_SEGMENTS; i++) {
        float a0 = (2 * PI * i) / CIRCLE_SEGMENTS;
        float a1 = (2 * PI * (i + 1)) / CIRCLE_SEGMENTS;

        rlVertex2f(cx + cosf(a0) * r, cy + sinf(a0) * r);
        rlVertex2f(cx + cosf(a1) * r, cy + sinf(a1) * r);
    }
}
//////////

class benchmark {
public:
    virtual ~benchmark() = default;
    virtual int run() = 0;
};