#include "ecs.h"

/// TEMP : FOR DEBUGGING PURPOSES ONLY ///
struct PositionComponent {
	float x;
	float y;
	
	PositionComponent() = default;
	PositionComponent(float _x, float _y){ x=_x; y=_y; }
};
struct SpriteComponent {
	void* img_data;
};
//////////////////////////////////////////

int main() {
	ecs world; // idk i just like the idea of the ecs beeing an actual representation of a "physical world" or universe, just like in Hytale
	
	world.register_component<PositionComponent>();
	world.register_component<SpriteComponent>();
	
	ecs::Entity entity = world.create_entity();
	world.add_component<PositionComponent>(entity, 250, 250);
	world.add_component<SpriteComponent>(entity);

	while (true) {
		// to do : make proper systems
		
		// temporary placeholder store "view" system made by ChatGPT
		world.for_each<PositionComponent, SpriteComponent>([](ecs::Entity entity, PositionComponent& pos, SpriteComponent& spr) {
			void* some_img = spr.img_data;
			
			pos.x += 0.01f;
			pos.y += 0.01f;
		});
	}
}