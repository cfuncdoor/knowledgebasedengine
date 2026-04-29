#pragma once

#include "core/types/types.h"
#include "core/ecs/component.h"
#include "core/ecs/entity.h"
#include "core/math/vector/vec3.h"
#include "core/math/vertex/vertex.h"

typedef component_id_t mesh_component_id;
typedef component_id_t transform_component_id;

#define MESH_COMPONENT_NAME "mesh"
#define TRANSFORM_COMPONENT_NAME "transform"

typedef struct transform_s {
    vec3_t position;
    vec3_t rotation;
    vec3_t scale;
} transform_t;

typedef struct mesh_s {
    vertex_t* vertices;
    u32 vertex_count;
} mesh_t;

void ecs_register_components(component_manager* cm, mesh_component_id* mesh_id, transform_component_id* transform_id);
void ecs_set_transform(component_manager* cm, transform_component_id id, entity_t e, vec3_t pos, vec3_t rot, vec3_t scale);
transform_t* ecs_get_transform(component_manager* cm, transform_component_id id, entity_t e);
void ecs_set_mesh(component_manager* cm, mesh_component_id id, entity_t e, vertex_t* verts, u32 count);
mesh_t* ecs_get_mesh(component_manager* cm, mesh_component_id id, entity_t e);