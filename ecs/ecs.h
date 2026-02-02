#pragma once

#include <cassert>

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

// you like my c-style convos :) ?

class ecs {
public:
    using Entity = uint32_t; // will probably change this later
    const Entity INVALID_ENTITY = 0;
    
    struct IComponentPool {
        virtual ~IComponentPool() = default;
    };

    template<typename T>
    struct ComponentPool : IComponentPool {
        std::vector<T> components;
        std::vector<Entity> entities;
        std::unordered_map<Entity, size_t> index;
    };
    
    template<typename T>
    void register_component(); // maybe replace with implicit registering when adding/removing components, or trying to get a pool ?
    template<typename T>
    ComponentPool<T>& get_pool();
    
    Entity create_entity();
    void destroy_entity(Entity _entity);
    template<typename T>
    bool has_component(Entity _entity); // to do : move in an "Entity" struct, so it makes more sence for gameplay devs
    
    template<typename... Components, typename Func>
    void for_each(Func&& fn); // this was generated (alongside impl of course) by ChatGPT, need to replace
    
    template<typename T, typename... Args>
    T& add_component(Entity _entity, Args&&... _args);
    template<typename T>
    T& get_component(Entity _entity);
    template<typename T>
    void remove_component(Entity _entity);

private:
    Entity m_next_entity = 1; // used for unique entity id and stuff
    std::vector<Entity> m_free_entities;
    
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pools;
};
#include "ecs.inl"