#include "core/ecs/components.h"

void ecs_register_components(component_manager* cm, mesh_component_id* mesh_id, transform_component_id* transform_id) {
    *mesh_id = component_register(cm, MESH_COMPONENT_NAME, sizeof(mesh_t));
    *transform_id = component_register(cm, TRANSFORM_COMPONENT_NAME, sizeof(transform_t));
}

void ecs_set_transform(component_manager* cm, transform_component_id id, entity_t e, vec3_t pos, vec3_t rot, vec3_t scale) {
    u32 offset = e * sizeof(transform_t);
    transform_t* t = (transform_t*)(cm->pools[id].data + offset);
    t->position = pos;
    t->rotation = rot;
    t->scale = scale;
}

transform_t* ecs_get_transform(component_manager* cm, transform_component_id id, entity_t e) {
    u32 offset = e * sizeof(transform_t);
    return (transform_t*)(cm->pools[id].data + offset);
}

void ecs_set_mesh(component_manager* cm, mesh_component_id id, entity_t e, vertex_t* verts, u32 count) {
    u32 offset = e * sizeof(mesh_t);
    mesh_t* m = (mesh_t*)(cm->pools[id].data + offset);
    m->vertices = verts;
    m->vertex_count = count;
}

mesh_t* ecs_get_mesh(component_manager* cm, mesh_component_id id, entity_t e) {
    u32 offset = e * sizeof(mesh_t);
    return (mesh_t*)(cm->pools[id].data + offset);
}