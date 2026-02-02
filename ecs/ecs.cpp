#include "ecs.h"

ecs::Entity ecs::create_entity() {
    if (!m_free_entities.empty()) {
        Entity entity = m_free_entities.back();
        m_free_entities.pop_back();
        return entity;
    }
    return m_next_entity++;
}

void ecs::destroy_entity(Entity _entity) { // to-do : remove components
    m_free_entities.push_back(_entity);
}