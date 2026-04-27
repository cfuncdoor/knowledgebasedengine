#pragma once
#include "../../types/types.h"

typedef struct {
    f32 x, y;
} vec2_t;

f32 vec2_dot(vec2_t a, vec2_t b);