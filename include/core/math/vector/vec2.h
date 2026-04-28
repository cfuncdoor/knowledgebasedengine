#pragma once
#include "core/types/types.h"
typedef struct vec2_s {
    f32 x, y;
} vec2_t;

f32 vec2_dot(vec2_t a, vec2_t b);
