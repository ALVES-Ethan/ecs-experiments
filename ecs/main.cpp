#include "ecs.h"
#include "tinygl.h"

/// TEMP : FOR DEBUGGING PURPOSES ONLY ///
struct position {
	float x;
	float y;
	
	position() = default;
	position(float _x, float _y) { this->x = _x; this->y = _y; }
};
struct graphics {
	unsigned int color;
	int size;
	
	graphics() = default;
	graphics(unsigned int _color, int _size) { color = _color; size = _size; }
};
struct velocity {
	float dir_x;
	float dir_y;
	float vel;
	
	velocity() { dir_x = 0.0f; dir_y = 0.0f; vel = 0.0f; }
};
struct controller {
	controller() = default;
};
//////////////////////////////////////////

int main() {
	constexpr int width = 960;
	constexpr int height = 540;
	tgl::init(width, height, L"ecs");
	
	ecs world; // idk i just like the idea of the ecs beeing an actual representation of a "physical world" or universe, just like in Hytale
	
	ecs::entity entity = world.create_entity();
	world.add_component<position>(entity, 250.0f, 250.0f);
	world.add_component<graphics>(entity, 0xFF0000, 25.0f);
	world.add_component<velocity>(entity);
	world.add_component<controller>(entity);

	while (tgl::running()) {
		// to do : make proper systems
		
		tgl::begin_frame(0x202020); // background
		
		// temporary placeholder store "view" system made by ChatGPT
		world.for_each<position, graphics>([](ecs::entity entity, position& pos, graphics& spr) { // draw system
			tgl::draw_square(static_cast<int>(pos.x), static_cast<int>(pos.y), spr.size, spr.color);
		});
		world.for_each<position, velocity>([](ecs::Entity entity, position& pos, velocity& vel) { // velocity system
			if (vel.vel >= 0.0f) { vel.vel -= 0.0001f; }
			else { vel.vel = 0.0f; }
			
			pos.x += vel.dir_x * vel.vel;
			pos.y += vel.dir_y * vel.vel;
		});
		world.for_each<controller, velocity>([](ecs::entity entity, controller& ctrl, velocity& vel) {
			const bool forward = tgl::is_pressed('Z');
			const bool backward = tgl::is_pressed('S');
			const bool left = tgl::is_pressed('Q');
			const bool right = tgl::is_pressed('D');
			
			const bool horizontal = left || right;
			const bool vertical = forward || backward;
			
			const bool moved = horizontal || vertical;
			
			vel.dir_y = forward ? -1.0f : backward ? 1.0f : vel.dir_y;
			vel.dir_x = right ? 1.0f : left ? -1.0f : vel.dir_x;
			
			if (vertical) {vel.dir_x = 0.0f; }
			if (horizontal) {vel.dir_y = 0.0f; }
			
			if (moved) {vel.vel = 0.1f;}
		});
		
		tgl::end_frame();
	}
	
	tgl::shutdown();
}