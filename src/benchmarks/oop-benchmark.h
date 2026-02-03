#pragma once

#include "../benchmark.h"

#include <vector>

struct entity {
    float x;
    float y;
    float radius;
    Color color;
};

class oop_benchmark : public benchmark {
public:
    oop_benchmark();

    int run() override;

private:
    void spawn_player();
    void spawn_entities();
    void compute_collisions();
    void handle_player_inputs();

    void draw_player();
    void draw_entities();

    entity m_player;
    std::vector<entity> m_entities;
};