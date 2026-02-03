#include "component.h"

#pragma once

template <typename T>
inline component::id component::get_id() {
    static id id = get_next_id();
    return id;
}