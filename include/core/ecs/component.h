#pragma once

#include "core/types/types.h"
#include <stdlib.h>

typedef u32 component_id_t;

#define INVALID_COMPONENT_ID ((component_id_t)0)
#define MAX_COMPONENTS 64
#define MAX_COMPONENT_SIZE 128
#define MAX_ENTITIES 4096

typedef struct component_type_s {
    u32 size;
    const char* name;
} component_type;

typedef struct component_pool_s {
    u8* data;
    b8* occupied;
} component_pool;

typedef struct component_manager_s {
    component_type types[MAX_COMPONENTS];
    component_pool pools[MAX_COMPONENTS];
    u32 type_count;
} component_manager;

void component_manager_init(component_manager* cm);
void component_manager_destroy(component_manager* cm);
component_id_t component_register(component_manager* cm, const char* name, u32 size);