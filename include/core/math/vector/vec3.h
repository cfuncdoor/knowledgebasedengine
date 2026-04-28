#pragma once
#include "core/types/types.h"

typedef struct vec3_s {
    f32 x, y, z;
} vec3_t;

vec3_t vec3_sub(vec3_t a, vec3_t b);
f32 vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);
vec3_t vec3_normalize(vec3_t v);