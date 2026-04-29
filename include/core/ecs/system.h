#pragma once

#include "core/types/types.h"
#include "core/ecs/component.h"

typedef struct world_s world;

typedef void (*system_update_fn)(world* w, f32 dt);

typedef struct system_s {
    const char* name;
    system_update_fn update;
    component_id_t required_components[8];
    u32 required_count;
} system_t;

typedef struct system_manager_s {
    system_t systems[32];
    u32 system_count;
} system_manager;

void system_manager_init(system_manager* sm);
void system_register(system_manager* sm, const char* name, system_update_fn update, component_id_t* required, u32 count);
void system_manager_update(system_manager* sm, world* w, f32 dt);