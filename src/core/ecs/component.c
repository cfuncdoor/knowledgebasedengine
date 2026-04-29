#include "core/ecs/component.h"
#include <string.h>

void component_manager_init(component_manager* cm) {
    memset(cm, 0, sizeof(component_manager));
}

void component_manager_destroy(component_manager* cm) {
    for (u32 i = 0; i < cm->type_count; i++) {
        free(cm->pools[i].data);
        free(cm->pools[i].occupied);
        cm->pools[i].data = NULL;
        cm->pools[i].occupied = NULL;
    }
    cm->type_count = 0;
}

component_id_t component_register(component_manager* cm, const char* name, u32 size) {
    if (cm->type_count >= MAX_COMPONENTS) {
        return INVALID_COMPONENT_ID;
    }
    component_id_t id = cm->type_count++;
    cm->types[id].name = name;
    cm->types[id].size = size;
    cm->pools[id].data = calloc(MAX_ENTITIES * MAX_COMPONENT_SIZE, sizeof(u8));
    cm->pools[id].occupied = calloc(MAX_ENTITIES, sizeof(b8));
    return id;
}