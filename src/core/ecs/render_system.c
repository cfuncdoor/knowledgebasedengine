#include "core/ecs/render_system.h"
#include "core/renderer/renderer.h"
#include "core/logger/logger.h"

void render_system_update(renderer_t* renderer, world* w) {
    if (!renderer || !w) return;
    if (!w->mesh_id || !w->transform_id) return;
    
    entity_manager* em = &w->em;
    component_manager* cm = &w->cm;
    LPDIRECT3DDEVICE9 dev = renderer->dx.context.device.handle;
    
    log_msg(LOG_LEVEL_INFO, "render_system: entities=%u mesh_id=%u transform_id=%u", em->next_entity - 1, w->mesh_id, w->transform_id);
    
    for (entity_t e = 1; e < em->next_entity; e++) {
        if (!entity_valid(em, e)) continue;
        
        mesh_t* mesh = ecs_get_mesh(cm, w->mesh_id, e);
        transform_t* t = ecs_get_transform(cm, w->transform_id, e);
        
        if (!mesh || !mesh->vertices) continue;
        
        dx9_vertex_buffer_update(&renderer->dx, mesh->vertices, mesh->vertex_count);
        
        mat4_t mat_world;
        mat4_identity(&mat_world);
        
        if (t) {
            mat4_rotate_y(&mat_world, t->rotation.y);
            mat4_rotate_x(&mat_world, t->rotation.x);
            mat4_translate(&mat_world, t->position.x, t->position.y, t->position.z);
        }
        
        dev->lpVtbl->SetTransform(dev, D3DTS_WORLD, (D3DMATRIX*)&mat_world);
        
        dx9_vertex_buffer_t* vbo = &renderer->dx.resource.buffer.v_buffer;
        if (vbo->buffer && vbo->vertex_count >= 3) {
            dev->lpVtbl->DrawPrimitive(dev, D3DPT_TRIANGLELIST, 0, vbo->vertex_count / 3);
        }
    }
}