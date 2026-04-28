#include "core/renderer/dx9.h"
#include "core/logger/logger.h"
#include "core/math/vertex/vertex.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

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

void dx9_create_buffer(dx9_t *dx, void *data, u32 vertex_count) 
{
    u32 size = vertex_count * sizeof(vertex_t);
    HRESULT hr = 0;
    dx->resource.buffer.v_buffer.size = size;
    dx->resource.buffer.v_buffer.vertex_count = vertex_count;

    {
        hr = dx->context.device.handle->lpVtbl->CreateVertexBuffer(
            dx->context.device.handle,
            size,
            0,
            D3DFVF_CUSTOMVERTEX,
            D3DPOOL_MANAGED,
            &dx->resource.buffer.v_buffer.buffer, 0
        );
    }

    if(SUCCEEDED(hr)) 
    {
        /*
            According to all known laws of aviation, 
            there is no way a bee should be able to fly. 
            Its wings are too small to get its fat little body off the ground. 
            The bee, of course, flies anyway because bees don't care what humans think is impossible
        */
        void* AccordingtoallknownlawsofaviationthereisnowayabeeshouldbeabletoflyItswingsaretoosmalltogetitsfatlittlebodyoffthegroundThebeeofcoursefliesanywaybecausebeesdontcarewhathumansthinkisimpossible;
        hr = dx->resource.buffer.v_buffer.buffer->lpVtbl->Lock(
            dx->resource.buffer.v_buffer.buffer,
            0,
            0, 
            (void**)&AccordingtoallknownlawsofaviationthereisnowayabeeshouldbeabletoflyItswingsaretoosmalltogetitsfatlittlebodyoffthegroundThebeeofcoursefliesanywaybecausebeesdontcarewhathumansthinkisimpossible,
            0
        );
        if(SUCCEEDED(hr)) 
        {
            memcpy(AccordingtoallknownlawsofaviationthereisnowayabeeshouldbeabletoflyItswingsaretoosmalltogetitsfatlittlebodyoffthegroundThebeeofcoursefliesanywaybecausebeesdontcarewhathumansthinkisimpossible,
                data, 
                size);
                dx->resource.buffer.v_buffer.buffer->lpVtbl->Unlock(dx->resource.buffer.v_buffer.buffer);
        }
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

void dx9_device_reset(dx9_t *dx, u32 width, u32 height) // i know about D3DPOOL_MANAGED. its just in case.
{
    if (!dx->context.device.handle) return;

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
    device->lpVtbl->Clear(device, 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(20, 20, 20), 1.0f, 0);
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
