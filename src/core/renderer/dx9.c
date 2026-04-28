#include "core/renderer/dx9.h"
#include "core/logger/logger.h"
#include "core/math/vertex/vertex.h"

void dx9_device_create(dx9_t *dx, HWND hwnd, u32 width, u32 height) 
{
    dx->context.instance = Direct3DCreate9(D3D_SDK_VERSION);
    if (!dx->context.instance) {
        log_msg(LOG_LEVEL_ERROR, "failed to initialize Direct3D9 instance");
        return;
    }

    D3DPRESENT_PARAMETERS d3dpp = {0};
    d3dpp.Windowed         = TRUE;
    d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow    = hwnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth  = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16; 

    dx->context.device.params = d3dpp;

    HRESULT hr = dx->context.instance->lpVtbl->CreateDevice(
        dx->context.instance,
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &dx->context.device.params,
        &dx->context.device.handle
    );

    if (FAILED(hr)) {
        log_msg(LOG_LEVEL_ERROR, "failed to create the directx device: 0x%08x", (u32)hr);
        return;
    }

    log_msg(LOG_LEVEL_INFO, "created directX device successfully");
}

void dx9_vertex_buffer_update(dx9_t* dx, void* data, u32 vertex_count) {
    if (!dx->context.device.handle || !data || vertex_count == 0) return;
    
    dx9_vertex_buffer_t* vbo = &dx->resource.buffer.v_buffer;
    u32 required_size = vertex_count * sizeof(vertex_t);

    if (vbo->buffer == NULL || required_size > vbo->capacity_bytes) {
        if (vbo->buffer) {
            vbo->buffer->lpVtbl->Release(vbo->buffer);
            vbo->buffer = NULL;
        }

        u32 new_capacity = (vbo->capacity_bytes == 0) ? required_size : (u32)(required_size * 1.5f);
        
        HRESULT hr = dx->context.device.handle->lpVtbl->CreateVertexBuffer(
            dx->context.device.handle,
            new_capacity,
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_CUSTOMVERTEX,
            D3DPOOL_DEFAULT,                       
            &vbo->buffer,
            NULL
        );

        if (FAILED(hr)) {
            log_msg(LOG_LEVEL_ERROR, "failed to resize vertex buffer: 0x%08x", (u32)hr);
            vbo->capacity_bytes = 0; 
            return;
        }

        vbo->capacity_bytes = new_capacity;
        log_msg(LOG_LEVEL_INFO, "resized vertex buffer to %u bytes", new_capacity);
    }

    vbo->vertex_count = vertex_count;

    if (!vbo->buffer) {
        log_msg(LOG_LEVEL_ERROR, "attempted to lock a NULL buffer!");
        return;
    }

    void* p_void = NULL;
    HRESULT hr = vbo->buffer->lpVtbl->Lock(vbo->buffer, 0, required_size, (void**)&p_void, D3DLOCK_DISCARD);
    
    if (SUCCEEDED(hr) && p_void) {
        memcpy(p_void, data, required_size);
        vbo->buffer->lpVtbl->Unlock(vbo->buffer);
    }
}
void dx9_destroy_buffer(dx9_t *dx) 
{
    log_msg(LOG_LEVEL_INFO, "releasing the directX vertex buffer");
    if (dx->resource.buffer.v_buffer.buffer) {
        dx->resource.buffer.v_buffer.buffer->lpVtbl->Release(dx->resource.buffer.v_buffer.buffer);
        dx->resource.buffer.v_buffer.buffer = NULL;
    }
}

void dx9_destroy(dx9_t* dx) 
{
    log_msg(LOG_LEVEL_INFO, "releasing the directX context");
    if (dx->context.device.handle) {
        dx->context.device.handle->lpVtbl->Release(dx->context.device.handle);
        dx->context.device.handle = NULL;
    }
    if (dx->context.instance) {
        dx->context.instance->lpVtbl->Release(dx->context.instance);
        dx->context.instance = NULL;
    }
    dx9_destroy_buffer(dx);
}

void dx9_device_reset(dx9_t *dx, u32 width, u32 height)
{
    if (!dx->context.device.handle) return;

    if (dx->resource.buffer.v_buffer.buffer) {
        dx->resource.buffer.v_buffer.buffer->lpVtbl->Release(dx->resource.buffer.v_buffer.buffer);
        dx->resource.buffer.v_buffer.buffer = NULL;
        dx->resource.buffer.v_buffer.capacity_bytes = 0; 
    }

    dx->context.device.params.BackBufferWidth = width;
    dx->context.device.params.BackBufferHeight = height;

    HRESULT hr = dx->context.device.handle->lpVtbl->Reset(
        dx->context.device.handle, 
        &dx->context.device.params
    );

    if (FAILED(hr)) {
        log_msg(LOG_LEVEL_ERROR, "failed to reset directX device: 0x%08x", (u32)hr);
        return;
    }

    dx->context.device.handle->lpVtbl->SetFVF(dx->context.device.handle, D3DFVF_CUSTOMVERTEX);

    log_msg(LOG_LEVEL_INFO, "directX device reset to %ux%u", width, height);
}

void dx9_clearscreen(LPDIRECT3DDEVICE9 device) 
{
    if(!device) return;
    device->lpVtbl->Clear(device, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(20, 20, 20), 1.0f, 0);
}

void dx9_present(LPDIRECT3DDEVICE9 device) 
{
    if(!device) return;
    device->lpVtbl->Present(device, NULL, NULL, NULL, NULL);
}

void dx9_endscene(LPDIRECT3DDEVICE9 device) 
{
    if(!device) return;
    device->lpVtbl->EndScene(device);
}

void dx9_beginscene(LPDIRECT3DDEVICE9 device, dx9_vertex_buffer_t* vbo) 
{
    if (!device || !vbo->buffer) return;
    if (SUCCEEDED(device->lpVtbl->BeginScene(device))) {
        dx9_setup_for_drawing(device, vbo->buffer);
    }
}

void dx9_setup_for_drawing(LPDIRECT3DDEVICE9 device, IDirect3DVertexBuffer9* buffer)
{
    device->lpVtbl->SetFVF(device, D3DFVF_CUSTOMVERTEX);
    device->lpVtbl->SetStreamSource(device, 0, buffer, 0, sizeof(vertex_t));
}
