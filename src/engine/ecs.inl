#pragma once
#include "ecs.h"

#pragma once

#include <assert.h>

#include "component.h"

template <typename T>
ecs::component_store<T>& ecs::get_store() {
    auto id = component::get_id<T>(); // get the id of the component from its type (an id is an index in m_pools array)
    
    assert(id < m_pools.size()); // assert if there are more components type than the size of the array
    
    if (!m_pools[id]) { m_pools[id] = std::make_unique<component_store<T>>(); } // create a new pool if the component is not already registered

    return *static_cast<component_store<T>*>(m_pools[id].get()); // lookup the pool, and cast it before returning
}

template <typename T>
bool ecs::has_component(entity _entity) {
    auto& store = get_store<T>(); // get the store of the specified component
    
    return (store.sparse[_entity] != store.invalid); // if the sparse contains a valid index at _entity, than we have this component
}

template<typename T, typename... Args>
T& ecs::add_component(entity _entity, Args&&... _args) {
    auto& store = get_store<T>();

    assert(_entity < store.sparse.size()); // entity is not in bound (too many entities)
    assert(store.sparse[_entity] == store.invalid); // entity already have a component of type T associated with it (allow only one component type per entity)
    assert(store.size < store.dense.size()); // we already have too much data in the store (component overflow)

    size_t index = store.size++; // new element is about to get added in the store

    store.dense[index] = T(std::forward<Args>(_args)...); // construct a component with args
    store.entities[index] = _entity; // set the index to the entity for fast entity lookup
    store.sparse[_entity] = index; // set the sparse to the index for component lookup

    return store.dense[index]; // return the constructed data
}

template<typename T>
T& ecs::get_component(entity _entity) {
    auto& store = get_store<T>();

    assert(_entity < store.sparse.size()); // entity is not in bound (invalid entity id or smf)

    size_t index = store.sparse[_entity]; // get the index to lookup associated component
    
    assert(index != store.invalid); // this entity have no components in this store
    assert(store.entities[index] == _entity); // there is a relation problem with the store

    return store.dense[index];
}

template <typename First, typename... Others, typename Func>
void ecs::for_each(Func&& fn) {
    auto& primary_store = get_store<First>();

    for (size_t i = 0; i < primary_store.size; ++i) {
        entity entity = primary_store.entities[i];

        if ((has_component<Others>(entity) && ...)) {

            fn(entity,
                primary_store.dense[i],
                get_component<Others>(entity)...);
        }
    }
}