#include "oop-benchmark.h"

#define ENABLE_PROFILER
#include "../utils/profiler.h"

oop_benchmark::oop_benchmark() {}

oop_benchmark::~oop_benchmark() {
    for (auto entity : m_entities) delete entity;
    m_entities.clear();
}

int oop_benchmark::run() {
    spawn_entities();
    spawn_player();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "oop");
    Camera2D cam = { 0 };
    cam.zoom = 1.0f;

    while (!WindowShouldClose()) {
        Profiler::beginFrame();
        PROFILE_SCOPE("frame_time");

        compute_collisions();

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(cam);

        update_entities();

        EndMode2D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    Profiler::dump("logs/oop.log");
    CloseWindow();
    return 0;
}

void oop_benchmark::spawn_player() {
    PROFILE_FUNCTION();
    m_player = new oop_player_entity();
    m_player->on_spawn();
    m_entities.push_back(m_player);
}

void oop_benchmark::spawn_entities() {
    PROFILE_FUNCTION();
    m_entities.reserve(ENTITY_COUNT);

    const float sx = (float)WINDOW_WIDTH / ENTITY_AMOUNT_X;
    const float sy = (float)WINDOW_HEIGHT / ENTITY_AMOUNT_Y;

    for (int y = 0; y < ENTITY_AMOUNT_Y; ++y) {
        for (int x = 0; x < ENTITY_AMOUNT_X; ++x) {
            auto e = new oop_graphic_collision_entity();
            e->on_spawn();
            e->set_position(ENTITY_RADIUS + x * sx, ENTITY_RADIUS + y * sy);
            m_entities.push_back(e);
        }
    }
}

void oop_benchmark::compute_collisions() {
    PROFILE_FUNCTION();
    for (auto entity : m_entities) {
        if (auto collision_entity = dynamic_cast<oop_graphic_collision_entity*>(entity)) {
            if (collision_entity != m_player)
                collision_entity->check_collision(m_player);
        }
    }
}

void oop_benchmark::update_entities() {
    PROFILE_FUNCTION();
    float dt = GetFrameTime();
    for (auto e : m_entities)
        e->on_update(dt);
}