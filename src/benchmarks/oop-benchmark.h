#pragma once

#include "../benchmark.h"

#include <vector>
#include <raylib.h>

class oop_entity {
protected:
    float x = 0.f, y = 0.f;

public:
    virtual ~oop_entity() = default;

    virtual void on_spawn() = 0;
    virtual void on_update(float dt) = 0;
    virtual void on_destroy() = 0;

    void set_position(float _x, float _y) {
        x = _x;
        y = _y;
    }

    float get_x() const { return x; }
    float get_y() const { return y; }
};

class oop_graphic_entity : public oop_entity {
protected:
    Color color = WHITE;
    float radius = 5.f;

public:
    void on_spawn() override {}
    
    void on_update(float dt) override {
        //DrawCircleLines(x, y, radius, color);
    }

    void on_destroy() override {}
};

class oop_graphic_collision_entity : public oop_graphic_entity {
public:
    void on_spawn() override {
        radius = ENTITY_RADIUS;
        color = RED;
    }

    void on_update(float dt) override {
        oop_graphic_entity::on_update(dt);
    }

    void on_destroy() override {}

    void check_collision(const oop_entity* player) {
        float dx = x - player->get_x();
        float dy = y - player->get_y();
        float rsum = radius + PLAYER_RADIUS;
        color = (dx*dx + dy*dy < rsum*rsum) ? GREEN : RED;
    }
};

class oop_player_entity : public oop_graphic_collision_entity {
public:
    void on_spawn() override {
        radius = PLAYER_RADIUS;
        color = BLUE;
        x = radius;
        y = radius;
    }

    void on_update(float dt) override {
        oop_graphic_collision_entity::on_update(dt);
        handle_input(dt);
    }

    void on_destroy() override {}

private:
    void handle_input(float dt) {
        float move = PLAYER_SPEED * dt;
        if (IsKeyDown(KEY_W)) y -= move;
        if (IsKeyDown(KEY_S)) y += move;
        if (IsKeyDown(KEY_A)) x -= move;
        if (IsKeyDown(KEY_D)) x += move;
    }
};

class oop_benchmark : public benchmark {
public:
    oop_benchmark();
    ~oop_benchmark() override;

    int run() override;

private:
    void spawn_player();
    void spawn_entities();
    void compute_collisions();

    void update_entities();

    std::vector<oop_entity*> m_entities;
    oop_player_entity* m_player = nullptr;
};