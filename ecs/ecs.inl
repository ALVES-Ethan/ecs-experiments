#include "ecs.h"

#pragma once

template <typename T>
void ecs::register_component() {
    std::type_index type = typeid(T);
    
    bool already_registered = false; // kinda funky, need to replace, why is map.contains only on std::20 :(
    for (auto it = m_pools.begin(); it != m_pools.end(); ++it)
        {if (it->first == type) { already_registered = true; break;} }

    assert(!already_registered);

    m_pools[type] = std::make_unique<ComponentPool<T>>();
}

template <typename T>
bool ecs::has_component(Entity _entity) {
    auto& pool = get_pool<T>();
    return pool.index.contains(_entity);
}

template <typename ... Components, typename Func>
void ecs::for_each(Func&& fn) {  // this was generated (alongside decl of course) by ChatGPT, need to replace
    // Use the first component as the driving pool
    auto& primary = get_pool<std::tuple_element_t<0, std::tuple<Components...>>>();

    for (size_t i = 0; i < primary.entities.size(); ++i) {
        Entity e = primary.entities[i];

        // Check if entity has all required components
        if ((has_component<Components>(e) && ...)) {
            fn(e, get_component<Components>(e)...);
        }
    }
}

template <typename T, typename... Args>
T& ecs::add_component(Entity _entity, Args&&... _args) {
    auto& pool = get_pool<T>();

    assert(!pool.index.contains(_entity)); // only allow one component per entity as of right now (IDK it's just simpler to deal with)

    size_t index = pool.components.size();

    pool.components.emplace_back(std::forward<Args>(_args)...);
    pool.entities.push_back(_entity);
    pool.index[_entity] = index;

    return pool.components.back();
}

template <typename T>
T& ecs::get_component(Entity _entity) {
    auto& pool = get_pool<T>();
    auto it = pool.index.find(_entity);

    assert(it != pool.index.end()); // entity does not have a component of this type

    return pool.components[it->second];
}

template <typename T>
void ecs::remove_component(Entity _entity) {
    auto& pool = get_pool<T>();
    auto it = pool.index.find(_entity);
    assert(it != pool.index.end());

    size_t index = it->second;
    size_t last = pool.components.size() - 1;

    // move last element into removed slot
    pool.components[index] = std::move(pool.components[last]);
    pool.entities[index] = pool.entities[last];
    pool.index[pool.entities[index]] = index;

    pool.components.pop_back();
    pool.entities.pop_back();
    pool.index.erase(_entity);
}

template <typename T>
ecs::ComponentPool<T>& ecs::get_pool() {
    auto key = std::type_index(typeid(T));

    auto it = m_pools.find(key);
    assert(it != m_pools.end()); // component not registered (no poll found)

    return *static_cast<ComponentPool<T>*>(it->second.get());
}