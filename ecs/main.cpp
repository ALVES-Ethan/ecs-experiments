#include "ecs.h"
#include "tinygl.h"

/// TEMP : FOR DEBUGGING PURPOSES ONLY ///
struct PositionComponent {
	float x;
	float y;
	
	PositionComponent() = default;
	PositionComponent(float _x, float _y){ x=_x; y=_y; }
};
struct SpriteComponent {
	unsigned int color;
	
	SpriteComponent() = default;
	SpriteComponent(unsigned int _color){ color=_color; }
};
//////////////////////////////////////////

int main() {
	constexpr int width = 960;
	constexpr int height = 540;
	tgl::init(width, height, L"ecs");
	
	ecs world; // idk i just like the idea of the ecs beeing an actual representation of a "physical world" or universe, just like in Hytale
	
	world.register_component<PositionComponent>();
	world.register_component<SpriteComponent>();
	
	ecs::Entity entity = world.create_entity();
	world.add_component<PositionComponent>(entity, 250, 250);
	world.add_component<SpriteComponent>(entity, 0xFF0000);

	while (tgl::running()) {
		// to do : make proper systems
		
		tgl::begin_frame(0x202020); // background
		
		// temporary placeholder store "view" system made by ChatGPT
		world.for_each<PositionComponent, SpriteComponent>([](ecs::Entity entity, PositionComponent& pos, SpriteComponent& spr) {
			pos.x += 0.01f;
			tgl::draw_square(pos.x, pos.y, 32, spr.color);
		});
		
		tgl::end_frame();
	}
	
	
	tgl::shutdown();
}