#include "core/renderer/renderer.h"
#include "core/logger/logger.h"
#include "core/renderer/dx9.h"
#include "core/math/vertex/vertex.h"

void renderer_initialize(renderer_t *renderer) 
{
    if (!renderer->window) {
        log_msg(LOG_LEVEL_ERROR, "renderer has no window attached");
        return;
    }
    log_msg(LOG_LEVEL_INFO, "initializing the renderer");
    log_msg(LOG_LEVEL_INFO, "creating the directX device");

    dx9_device_create(&renderer->dx, 
                    renderer->window->win.hwnd, 
                    renderer->window->details.width, 
                    renderer->window->details.height);

    vertex_t vertices[] = {
        { 100.0f, 100.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0) },
        { 300.0f, 100.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }, 
        { 100.0f, 300.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255) }, 
        { 300.0f, 300.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255) }
    };
    u32 v_count = sizeof(vertices) / sizeof(vertex_t);

    log_msg(LOG_LEVEL_INFO, "creating the directX vertex buffer");
    dx9_create_buffer(&renderer->dx, vertices, v_count);

    log_msg(LOG_LEVEL_INFO, "initialized the renderer successfully!");
}

void renderer_beginframe(renderer_t *renderer) 
{
    renderer->state.is_rendering = true;
    LPDIRECT3DDEVICE9 dev = renderer->dx.context.device.handle;
    dx9_clearscreen(dev);
    dx9_beginscene(dev, &renderer->dx.resource.buffer.v_buffer);
}

void renderer_endframe(renderer_t *renderer) 
{
    LPDIRECT3DDEVICE9 dev = renderer->dx.context.device.handle;
    
    u32 prim_count = renderer->dx.resource.buffer.v_buffer.vertex_count - 2;
    if (prim_count > 0) {
        dev->lpVtbl->DrawPrimitive(dev, D3DPT_TRIANGLESTRIP, 0, prim_count);
    }

    dx9_endscene(dev);
    dx9_present(dev);
    renderer->state.is_rendering = false;
}

void renderer_on_resize(renderer_t *renderer, u32 width, u32 height) 
{
    if (width == 0 || height == 0) return;

    renderer->window->details.width = width;
    renderer->window->details.height = height;

    dx9_device_reset(&renderer->dx, width, height);
}

void renderer_destroy(renderer_t *renderer) 
{
    log_msg(LOG_LEVEL_INFO, "destroying the renderer...");
    dx9_destroy(&renderer->dx);
}