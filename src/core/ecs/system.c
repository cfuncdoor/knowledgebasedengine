#include "core/ecs/system.h"

void system_manager_init(system_manager* sm) {
    sm->system_count = 0;
}

void system_register(system_manager* sm, const char* name, system_update_fn update, component_id_t* required, u32 count) {
    if (sm->system_count >= 32) return;
    system_t* sys = &sm->systems[sm->system_count++];
    sys->name = name;
    sys->update = update;
    for (u32 i = 0; i < count && i < 8; i++) {
        sys->required_components[i] = required[i];
    }
    sys->required_count = count;
}

void system_manager_update(system_manager* sm, world* w, f32 dt) {
    for (u32 i = 0; i < sm->system_count; i++) {
        system_t* sys = &sm->systems[i];
        if (sys->update) {
            sys->update(w, dt);
        }
    }
}