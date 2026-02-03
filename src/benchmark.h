#pragma once

#include <raylib.h>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 540;

constexpr int ENTITY_AMOUNT_X = 86;
constexpr int ENTITY_AMOUNT_Y = 58;
constexpr int ENTITY_RADIUS = 4;
constexpr int PLAYER_RADIUS = 64;
constexpr float PLAYER_SPEED = 256.0f;

constexpr int ENTITY_COUNT = ENTITY_AMOUNT_X * ENTITY_AMOUNT_Y;

class benchmark {
public:
    virtual ~benchmark() = default;
    virtual int run() = 0;
};