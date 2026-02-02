#include "ecs.h"
#include "tinygl.h"

/// TEMP : FOR DEBUGGING PURPOSES ONLY ///
struct Position {
	float x;
	float y;
	
	Position() = default;
	Position(float _x, float _y){ x=_x; y=_y; }
};
struct Graphics {
	unsigned int color;
	int size;
	
	Graphics() = default;
	Graphics(unsigned int _color, int _size){ color=_color; size=_size; }
};
struct Velocity {
	float dirX;
	float dirY;
	float vel;
	
	Velocity() = default;
	Velocity(float _dirX, float _dirY){ dirX=_dirX; dirY=_dirY; vel=0.0f; }
};
struct CharacterController {
	int arbitrary;
	
	CharacterController() = default;
	CharacterController(int _arbitrary){ arbitrary=_arbitrary; }
};
//////////////////////////////////////////

int main() {
	constexpr int width = 960;
	constexpr int height = 540;
	tgl::init(width, height, L"ecs");
	
	ecs world; // idk i just like the idea of the ecs beeing an actual representation of a "physical world" or universe, just like in Hytale
	
	world.register_component<Position>();
	world.register_component<Graphics>();
	world.register_component<Velocity>();
	world.register_component<CharacterController>();
	
	ecs::Entity entity = world.create_entity();
	world.add_component<Position>(entity, 250.0f, 250.0f);
	world.add_component<Graphics>(entity, 0xFF0000, 25.0f);
	world.add_component<Velocity>(entity, 0.0f, 0.0f);
	world.add_component<CharacterController>(entity, 0);

	while (tgl::running()) {
		// to do : make proper systems
		
		tgl::begin_frame(0x202020); // background
		
		// temporary placeholder store "view" system made by ChatGPT
		world.for_each<Position, Graphics>([](ecs::Entity entity, Position& pos, Graphics& spr) { // draw system
			tgl::draw_square(static_cast<int>(pos.x), static_cast<int>(pos.y), spr.size, spr.color);
		});
		world.for_each<Position, Velocity>([](ecs::Entity entity, Position& pos, Velocity& vel) { // velocity system
			if (vel.vel >= 0.0f) { vel.vel -= 0.0001f; }
			else { vel.vel = 0.0f; }
			
			pos.x += vel.dirX * vel.vel;
			pos.y += vel.dirY * vel.vel;
		});
		world.for_each<CharacterController, Velocity>([](ecs::Entity entity, CharacterController& ctrl, Velocity& vel) {
			const bool forward = tgl::is_pressed('Z');
			const bool backward = tgl::is_pressed('S');
			const bool left = tgl::is_pressed('Q');
			const bool right = tgl::is_pressed('D');
			
			const bool horizontal = left || right;
			const bool vertical = forward || backward;
			
			const bool moved = horizontal || vertical;
			
			vel.dirY = forward ? -1.0f : backward ? 1.0f : vel.dirY;
			vel.dirX = right ? 1.0f : left ? -1.0f : vel.dirX;
			
			if (vertical) {vel.dirX = 0.0f; }
			if (horizontal) {vel.dirY = 0.0f; }
			
			if (moved) {vel.vel = 0.1f;}
		});
		
		tgl::end_frame();
	}
	
	
	tgl::shutdown();
}