#pragma once

#include "../benchmark.h"

#include <vector>

struct entity {
    float x;
    float y;
    float radius;
    Color color;
};

class ref_benchmark : public benchmark {
public:
    ref_benchmark();

    int run() override;

private:
    void spawn_player();
    void spawn_entities();
    void compute_collisions();
    void handle_player_inputs();

    void draw_entities();

    size_t m_player;
    std::vector<entity> m_entities;
};