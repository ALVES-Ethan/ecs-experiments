#include "component.h"

component::id component::get_next_id() {
    static id id = 0;
    return id++;
}