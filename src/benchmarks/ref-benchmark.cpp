#include "ref-benchmark.h"

#define ENABLE_PROFILER
#include "../utils/profiler.h"

ref_benchmark::ref_benchmark() : m_player(), m_entities() {}

int ref_benchmark::run() {
    spawn_entities();
    spawn_player();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ecs");
    Camera2D cam = { 0 }; cam.zoom = 1.0f;

    while (!WindowShouldClose()) {
        Profiler::beginFrame();

        handle_player_inputs();
        compute_collisions();

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(cam);

        draw_entities();
        draw_player();

        EndMode2D();
        DrawFPS(10, 10);
        EndDrawing();
    }
    Profiler::dump("logs/ref.log");
    CloseWindow();
    return 0;
}

void ref_benchmark::spawn_player() {
    PROFILE_FUNCTION();
    m_player = { PLAYER_RADIUS, PLAYER_RADIUS, PLAYER_RADIUS, BLUE };
}

void ref_benchmark::spawn_entities() {
    PROFILE_FUNCTION();

    m_entities.reserve(ENTITY_COUNT);

    const float sx = (float)WINDOW_WIDTH / ENTITY_AMOUNT_X;
    const float sy = (float)WINDOW_HEIGHT / ENTITY_AMOUNT_Y;

    int index = 0;
    for (int y = 0; y < ENTITY_AMOUNT_Y; ++y) {
        for (int x = 0; x < ENTITY_AMOUNT_X; ++x) {
            m_entities.push_back({
                ENTITY_RADIUS + x * sx,
                ENTITY_RADIUS + y * sy,
                ENTITY_RADIUS,
                RED
            });
            index++;
        }
    }
}

void ref_benchmark::compute_collisions() {
    PROFILE_FUNCTION();
    for (entity& e : m_entities) {
        const float dx = e.x - m_player.x;
        const float dy = e.y - m_player.y;
        const float r = e.radius + m_player.radius;

        e.color = (dx * dx + dy * dy < r * r) ? GREEN : RED;
    }
}

void ref_benchmark::handle_player_inputs() {
    PROFILE_FUNCTION();
    const float dt = GetFrameTime();
    const float move = PLAYER_SPEED * dt;

    if (IsKeyDown(KEY_W)) m_player.y -= move;
    if (IsKeyDown(KEY_S)) m_player.y += move;
    if (IsKeyDown(KEY_A)) m_player.x -= move;
    if (IsKeyDown(KEY_D)) m_player.x += move;
}

void ref_benchmark::draw_player() {
    PROFILE_FUNCTION();
    DrawCircleLines(m_player.x, m_player.y, m_player.radius, BLUE);
}

void ref_benchmark::draw_entities() {
    PROFILE_FUNCTION();
    for (const entity& e : m_entities) {
        if (e.x + e.radius < 0 || e.x - e.radius > WINDOW_WIDTH ||
            e.y + e.radius < 0 || e.y - e.radius > WINDOW_HEIGHT)
            continue;

        DrawCircleLines(e.x, e.y, e.radius, e.color);
    }
}