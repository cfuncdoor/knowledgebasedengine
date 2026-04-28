#include "core/math/vector/vec3.h"
#include <math.h>

vec3_t vec3_sub(vec3_t a, vec3_t b) { return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z}; }
f32 vec3_dot(vec3_t a, vec3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return (vec3_t){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

vec3_t vec3_normalize(vec3_t v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return (vec3_t){v.x / len, v.y / len, v.z / len};
}
