#pragma once

#include "../benchmark.h"
#include "../engine/ecs.h"

struct position {
	float x, y;
};

struct graphics {
	float radius;
	Color color;
};

struct collider {
	float radius;
};

struct controller {
	float speed;
};

class ecs_benchmark : public benchmark {
public:
	ecs_benchmark();

	int run() override;

private:
	void spawn_player();
	void spawn_entities();
	void compute_collisions();
	void handle_player_inputs();

	void draw_entities();

	ecs m_ecs;
	ecs::entity m_player;
};