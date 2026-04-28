#pragma once
#include <windows.h>
#include <d3d9.h>
#include "core/types/types.h"

typedef struct dx9_device_s {
    IDirect3DDevice9* handle; 
    D3DPRESENT_PARAMETERS params; 
} dx9_device_t;

typedef struct dx9_vertex_buffer_s {
    u32 size;
    u32 vertex_count;
    IDirect3DVertexBuffer9* buffer;
} dx9_vertex_buffer_t;

typedef struct dx9_buffer_s{
    dx9_vertex_buffer_t v_buffer;
} dx9_buffer_t;

typedef struct dx9_resources_s {
    dx9_buffer_t buffer;
} dx9_resources_t;

typedef struct dx9_context_s {
    IDirect3D9* instance;      
    dx9_device_t device;
} dx9_context_t;

typedef struct dx9_s {
    dx9_context_t context;
    dx9_resources_t resource;
} dx9_t;

void dx9_render(dx9_t* dx);

void dx9_clearscreen(LPDIRECT3DDEVICE9 device);
void dx9_present(LPDIRECT3DDEVICE9 device);
void dx9_endscene(LPDIRECT3DDEVICE9 device);
void dx9_beginscene(LPDIRECT3DDEVICE9 device, dx9_vertex_buffer_t* vbo);
void dx9_setup_for_drawing(LPDIRECT3DDEVICE9 device, IDirect3DVertexBuffer9* buffer);

void dx9_destroy(dx9_t* dx);
void dx9_destroy_buffer(dx9_t* dx);
void dx9_device_create(dx9_t* dx, HWND hwnd, u32 width, u32 height);
void dx9_create_buffer(dx9_t* dx, void* data, u32 vertex_count);
void dx9_device_reset(dx9_t *dx, u32 width, u32 height);
