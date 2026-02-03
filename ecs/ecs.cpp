#include "ecs.h"

ecs::entity ecs::create_entity() {
    static entity next = 1;
    return next++;
}