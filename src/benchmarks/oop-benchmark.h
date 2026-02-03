#pragma once

#include <vector>
#include <raylib.h>

class oop_entity {
protected:
	float x, y;

	virtual void on_spawn() = 0;
	virtual void on_update(float _dt) = 0;
	virtual void on_destroy() = 0;
};

class oop_graphic_entity : public oop_entity {
protected:
	Color color;
	float radius;

	inline void on_update(float _dt) override {
		DrawCircleLines(x, y, radius, color);
	}
};

class oop_graphic_collision_entity : public oop_graphic_entity{
	inline void on_update(float _dt) override {
		oop_graphic_entity::on_update(_dt);

	}
};

class oop_benchmark {
public:
	oop_benchmark();

private:
	void spawn_player();
	void spawn_entities();
	void compute_collisions();
	void handle_player_inputs();

	void draw_player();
	void draw_entities();

	std::vector<oop_entity> m_entities;
}; 