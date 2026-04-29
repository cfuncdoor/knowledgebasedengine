#pragma once

#include "core/ecs/entity.h"
#include "core/ecs/component.h"
#include "core/ecs/system.h"
#include "core/ecs/components.h"
#include "core/ecs/render_system.h"
#include "core/renderer/renderer.h"

typedef struct world_s {
    entity_manager em;
    component_manager cm;
    system_manager sm;
    mesh_component_id mesh_id;
    transform_component_id transform_id;
} world;

void world_init(world* w);
void world_destroy(world* w);
void world_update(world* w, f32 dt, renderer_t* renderer);
void world_register_components(world* w);
entity_t world_create_entity(world* w);
void world_set_transform(world* w, entity_t e, vec3_t pos, vec3_t rot, vec3_t scale);
transform_t* world_get_transform(world* w, entity_t e);
void world_set_mesh(world* w, entity_t e, vertex_t* verts, u32 count);
mesh_t* world_get_mesh(world* w, entity_t e);
void movement_system_update(world* w, f32 dt);