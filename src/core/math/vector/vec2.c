#include "core/math/vector/vec2.h"

f32 vec2_dot(vec2_t a, vec2_t b) 
{
    return (a.x * b.x) + (b.y * a.y);
}
