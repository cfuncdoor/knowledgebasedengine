#pragma once
#include "dx9.h"
#include "core/window/window.h"

typedef struct renderer_state_s {
    b8 is_initialized;
    b8 is_rendering;
} renderer_state_t;

typedef struct renderer_s {
    window_t* window; // window context of renderer
    dx9_t dx; //dx context of renderer
    renderer_state_t state; // state of renderer;
} renderer_t;

void renderer_initialize(renderer_t* renderer, window_t* window);
void renderer_beginframe(renderer_t* renderer);
void renderer_endframe(renderer_t* renderer);
void renderer_on_resize(renderer_t *renderer, u32 width, u32 height);
void renderer_destroy(renderer_t* renderer);