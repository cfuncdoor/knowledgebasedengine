#include "core/ecs/entity.h"
#include <string.h>

void entity_manager_init(entity_manager* em) {
    memset(em, 0, sizeof(entity_manager));
    em->active = calloc(MAX_ENTITIES, sizeof(b8));
    em->next_entity = 1;
}

void entity_manager_destroy(entity_manager* em) {
    free(em->active);
    em->active = NULL;
}

entity_t entity_create(entity_manager* em) {
    if (!em->active || em->next_entity >= MAX_ENTITIES) {
        return ENTITY_NULL;
    }
    entity_t e = em->next_entity++;
    em->active[e] = true;
    return e;
}

void entity_destroy(entity_manager* em, entity_t e) {
    if (em->active && e < MAX_ENTITIES) {
        em->active[e] = false;
    }
}

b8 entity_valid(entity_manager* em, entity_t e) {
    return em->active && e < MAX_ENTITIES && em->active[e];
}