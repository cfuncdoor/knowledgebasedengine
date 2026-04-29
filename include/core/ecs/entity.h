#pragma once

#include "core/types/types.h"
#include <stdlib.h>

typedef u32 entity_t;
typedef u32 component_id_t;

#define ENTITY_NULL ((entity_t)0)
#define MAX_ENTITIES 4096

typedef struct entity_manager_s {
    entity_t next_entity;
    b8* active;
} entity_manager;

void entity_manager_init(entity_manager* em);
void entity_manager_destroy(entity_manager* em);
entity_t entity_create(entity_manager* em);
void entity_destroy(entity_manager* em, entity_t e);
b8 entity_valid(entity_manager* em, entity_t e);