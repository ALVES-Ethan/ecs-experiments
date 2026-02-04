#pragma once

#include <memory>
#include <array>
#include <cstdint>

constexpr uint8_t MAX_COMPONENT_TYPE_COUNT = 32;

class ecs {
public:
    using entity = uint32_t;
    
    struct store_base {
        virtual ~store_base() = default;
    };

    template<typename T, uint32_t MAX_STORE_CAPACITY = 4097*4097>
    struct component_store : store_base {
        std::array<entity, MAX_STORE_CAPACITY> entities;
        std::array<size_t, MAX_STORE_CAPACITY> sparse;
        std::array<T, MAX_STORE_CAPACITY> dense;
        size_t size = 0;
        
        static constexpr size_t invalid = MAX_STORE_CAPACITY;
        
        component_store() {
            sparse.fill(invalid);
        }
    };
    
    template<typename T>
    component_store<T>& get_store();

    entity create_entity();
    
    template<typename T>
    bool has_component(entity _entity);
    template<typename T, typename... Args>
    T& add_component(entity _entity, Args&&... _args);
    template<typename T>
    T& get_component(entity _entity);
    
    template<typename First, typename... Others, typename Func>
    void for_each(Func&& fn);
    
private:
    std::array<std::unique_ptr<store_base>, MAX_COMPONENT_TYPE_COUNT> m_pools;
};
#include "ecs.inl"