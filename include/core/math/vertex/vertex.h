#pragma once
#include "core/types/types.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

typedef struct vertex_s {
    float x, y, z;   
    u32 color;       
} vertex_t;
