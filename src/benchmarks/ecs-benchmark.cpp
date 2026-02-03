#include "ecs-benchmark.h"

#define ENABLE_PROFILER
#include "../utils/profiler.h"

ecs_benchmark::ecs_benchmark() : m_ecs() {}

int ecs_benchmark::run() {
    spawn_entities();
    spawn_player();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ecs");
    Camera2D cam = { 0 }; cam.zoom = 1.0f;

    while (!WindowShouldClose()) {
        Profiler::beginFrame();
        PROFILE_SCOPE("frame_time");

        handle_player_inputs();
        compute_collisions();

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(cam);

        draw_entities();
        //draw_player();

        EndMode2D();
        DrawFPS(10, 10);
        EndDrawing();
    }
    Profiler::dump("logs/ecs.h");
    CloseWindow();
    return 0;
}

void ecs_benchmark::spawn_player() {
    PROFILE_FUNCTION();

    m_player = m_ecs.create_entity();
    m_ecs.add_component<position>(m_player, PLAYER_RADIUS, PLAYER_RADIUS);
    m_ecs.add_component<graphics>(m_player, PLAYER_RADIUS, BLUE);
    m_ecs.add_component<collider>(m_player, PLAYER_RADIUS);
    m_ecs.add_component<controller>(m_player, PLAYER_SPEED);
}

void ecs_benchmark::spawn_entities() {
    PROFILE_FUNCTION();

    const float sx = (float)WINDOW_WIDTH / ENTITY_AMOUNT_X;
    const float sy = (float)WINDOW_HEIGHT / ENTITY_AMOUNT_Y;

    int index = 0;
    for (int y = 0; y < ENTITY_AMOUNT_Y; ++y) {
        for (int x = 0; x < ENTITY_AMOUNT_X; ++x) {
            ecs::entity entity = m_ecs.create_entity();
            m_ecs.add_component<position>(entity, ENTITY_RADIUS + x * sx, ENTITY_RADIUS + y * sy);
            m_ecs.add_component<graphics>(entity, ENTITY_RADIUS, RED);
            m_ecs.add_component<collider>(entity, ENTITY_RADIUS);
            index++;
        }
    }
}

void ecs_benchmark::compute_collisions() {
    PROFILE_FUNCTION();

    position& plp = m_ecs.get_component<position>(m_player);
    collider& plc = m_ecs.get_component<collider>(m_player);
    
    m_ecs.for_each<collider, position, graphics>([plp, plc](ecs::entity _entity, collider& _col, position& _pos, graphics& _grp) {
            const float dx = _pos.x - plp.x;
            const float dy = _pos.y - plp.y;
            const float r = _col.radius + plc.radius;

            _grp.color = (dx * dx + dy * dy < r * r) ? GREEN : RED;
    });
}

void ecs_benchmark::handle_player_inputs() {
    PROFILE_FUNCTION();
    const float dt = GetFrameTime();
    const float move = PLAYER_SPEED * dt;

    m_ecs.for_each<position, controller>([move](ecs::entity _entity, position& _pos, controller& _ctr) {
        if (IsKeyDown(KEY_W)) _pos.y -= move;
        if (IsKeyDown(KEY_S)) _pos.y += move;
        if (IsKeyDown(KEY_A)) _pos.x -= move;
        if (IsKeyDown(KEY_D)) _pos.x += move;
    });
}

void ecs_benchmark::draw_entities() {
    PROFILE_FUNCTION();

    m_ecs.for_each<position, graphics>([](ecs::entity _entity, position& _pos, graphics& _grp) {
        if (_pos.x + _grp.radius < 0 || _pos.x - _grp.radius > WINDOW_WIDTH ||
            _pos.y + _grp.radius < 0 || _pos.y - _grp.radius > WINDOW_HEIGHT)
            return;

        DrawCircleLines(_pos.x, _pos.y, _grp.radius, _grp.color);
    });
}