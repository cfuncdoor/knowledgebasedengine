#pragma once
#include <windows.h>
#include <d3d9.h>
#include "core/types/types.h"

typedef struct {
    IDirect3DDevice9* handle; 
    D3DPRESENT_PARAMETERS params; 
} dx9_device_t;

typedef struct {
    IDirect3D9* instance;      
    dx9_device_t device;
} dx9_context_t;

typedef struct {
    dx9_context_t context;
} dx9_t;

void dx9_create(dx9_t* dx, HWND hwnd, u32 width, u32 height);
void dx9_render(dx9_t* dx);
void dx9_destroy(dx9_t* dx);

void setup_pixel_format(HDC hdc);
