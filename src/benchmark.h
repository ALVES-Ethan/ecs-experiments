#pragma once

#include <array>
#include <raylib.h>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 540;

constexpr int ENTITY_AMOUNT_X = 64;
constexpr int ENTITY_AMOUNT_Y = 64;
constexpr int ENTITY_RADIUS = 4;
constexpr int PLAYER_RADIUS = 64;
constexpr float PLAYER_SPEED = 256.0f;

constexpr int ENTITY_COUNT = ENTITY_AMOUNT_X * ENTITY_AMOUNT_Y;

struct entity {
    float x;
    float y;
    float radius;
    Color color;
};

class benchmark {
public:
    benchmark();

    int run();

private:
    void spawn_player();
    void spawn_entities();
    void compute_collisions();
    void handle_player_inputs();

    void draw_player();
    void draw_entities();

    entity m_player;
    std::array<entity, ENTITY_COUNT> m_entities;
};