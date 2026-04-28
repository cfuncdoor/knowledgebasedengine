#pragma once
#include "core/types/types.h"
#include "core/math/vector/vec3.h"
#include <math.h>

typedef struct mat4_s {
    f32 m[4][4];
} mat4_t;

void mat4_identity(mat4_t* mat);
void mat4_rotate_y(mat4_t* mat, f32 angle);
void mat4_rotate_x(mat4_t* mat, f32 angle);
void mat4_perspective(mat4_t* mat, f32 fov, f32 aspect, f32 near, f32 far);
void mat4_lookat(mat4_t* mat, vec3_t eye, vec3_t target, vec3_t up);