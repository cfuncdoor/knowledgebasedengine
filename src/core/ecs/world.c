#include "core/ecs/world.h"
#include "core/ecs/components.h"
#include "core/ecs/movement_system.h"
#include "core/ecs/render_system.h"

void world_init(world* w) {
    entity_manager_init(&w->em);
    component_manager_init(&w->cm);
    system_manager_init(&w->sm);
    world_register_components(w);
}

void world_destroy(world* w) {
    entity_manager_destroy(&w->em);
    component_manager_destroy(&w->cm);
}

void world_update(world* w, f32 dt, renderer_t* renderer) {
    (void)renderer;
    movement_system_update(w, dt);
}

void world_register_components(world* w) {
    ecs_register_components(&w->cm, &w->mesh_id, &w->transform_id);
}

entity_t world_create_entity(world* w) {
    return entity_create(&w->em);
}

void world_set_transform(world* w, entity_t e, vec3_t pos, vec3_t rot, vec3_t scale) {
    ecs_set_transform(&w->cm, w->transform_id, e, pos, rot, scale);
}

transform_t* world_get_transform(world* w, entity_t e) {
    return ecs_get_transform(&w->cm, w->transform_id, e);
}

void world_set_mesh(world* w, entity_t e, vertex_t* verts, u32 count) {
    ecs_set_mesh(&w->cm, w->mesh_id, e, verts, count);
}

mesh_t* world_get_mesh(world* w, entity_t e) {
    return ecs_get_mesh(&w->cm, w->mesh_id, e);
}