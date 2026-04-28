#include "core/renderer/renderer.h"
#include "core/logger/logger.h"
#include "core/renderer/dx9.h"

void renderer_initialize(renderer_t *renderer, window_t* window) 
{
    if (!window) {
        log_msg(LOG_LEVEL_ERROR, "renderer has no window attached");
        return;
    }
    log_msg(LOG_LEVEL_INFO, "initializing the renderer");
    log_msg(LOG_LEVEL_INFO, "creating the directX device");

    dx9_device_create(&renderer->dx, 
                    window->win.hwnd, 
                    window->details.width, 
                    window->details.height);

    log_msg(LOG_LEVEL_INFO, "initialized the renderer successfully!");
    renderer->window = window; // assign internal renderer_s::window_t
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
    dx9_endscene(renderer->dx.context.device.handle);
    dx9_present(renderer->dx.context.device.handle);
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