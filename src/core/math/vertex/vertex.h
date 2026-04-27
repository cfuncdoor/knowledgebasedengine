#pragma once
#include "../../types/types.h"

typedef struct {
    f32 x, y, z, rhw;
    u32 color;
} vertex_t;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
