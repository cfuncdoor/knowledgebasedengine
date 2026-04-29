#include "core/ecs/movement_system.h"
#include "core/ecs/components.h"

static f32 global_time = 0.0f;

void movement_system_update(world* w, f32 dt) {
    global_time += dt;
    
    entity_manager* em = &w->em;
    component_manager* cm = &w->cm;
    
    for (entity_t e = 1; e < em->next_entity; e++) {
        if (!entity_valid(em, e)) continue;
        
        transform_t* t = ecs_get_transform(cm, w->transform_id, e);
        if (!t) continue;
        
        f32 speed = 2.0f;
        t->rotation.y = global_time * speed;
        t->rotation.x = global_time * speed * 0.5f;
    }
}