#include "core/renderer/renderer.h"
#include "core/math/vertex/vertex.h"
#include "core/logger/logger.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <winerror.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void dx9_create(dx9_t *dx, HWND hwnd, u32 width, u32 height) 
{
    dx->context.instance = Direct3DCreate9(D3D_SDK_VERSION);
    memset(&dx->context.device.params, 0, sizeof(dx->context.device.params));
    dx->context.device.params.Windowed = TRUE;
    dx->context.device.params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    dx->context.device.params.hDeviceWindow = hwnd;
    dx->context.device.params.BackBufferFormat = D3DFMT_X8R8G8B8;
    dx->context.device.params.BackBufferWidth = width;
    dx->context.device.params.BackBufferHeight = height;

    {
        HRESULT result = dx->context.instance->lpVtbl->CreateDevice(
            dx->context.instance,
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &dx->context.device.params,
            &dx->context.device.handle
        );

        if(SUCCEEDED(result)) {
            log_msg(LOG_LEVEL_INFO, "created directX device");
        } else if(FAILED(result)) {
            log_msg(LOG_LEVEL_ERROR, "failed to create the directx device: 0x%08x", (u32)result);
            return;
        }
    }

}

void dx9_destroy(dx9_t* dx) {
    if (dx->context.device.handle) {
        dx->context.device.handle->lpVtbl->Release(dx->context.device.handle);
    }
    if (dx->context.instance) {
        dx->context.instance->lpVtbl->Release(dx->context.instance);
    }
}

void dx9_render(dx9_t *dx) 
{
    if (!dx->context.device.handle) return;
   dx->context.device.handle->lpVtbl->Clear(
       dx->context.device.handle, 0, NULL, 
        D3DCLEAR_TARGET, D3DCOLOR_XRGB(100, 149, 237), 1.0f, 0
    );

    if (SUCCEEDED(dx->context.device.handle->lpVtbl->BeginScene(dx->context.device.handle))) {
        vertex_t vertices[] = {
            { 100.0f, 100.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0) },
            { 300.0f, 100.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }, 
            { 100.0f, 300.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255) }, 
            { 300.0f, 300.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255) }
        };

        dx->context.device.handle->lpVtbl->SetFVF(dx->context.device.handle, D3DFVF_CUSTOMVERTEX);
        
        dx->context.device.handle->lpVtbl->DrawPrimitiveUP(
            dx->context.device.handle, 
            D3DPT_TRIANGLESTRIP, 
            2,          
            vertices, 
            sizeof(vertex_t)
        );

       dx->context.device.handle->lpVtbl->EndScene(dx->context.device.handle);
    }

   dx->context.device.handle->lpVtbl->Present(dx->context.device.handle, NULL, NULL, NULL, NULL);
}
