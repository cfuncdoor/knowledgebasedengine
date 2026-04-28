#include "core/application.h"
#include "core/logger/logger.h"
#include "core/math/matrix/matrix.h"
#include "core/math/vertex/vertex.h"
#include "core/renderer/renderer.h"
#include "core/window/window.h"
#include <minwindef.h>
#include <stdio.h>
#include <string.h>
#include <winuser.h>

void application_create(application_t *app) 
{
    static i32 window_counter = 0;

    string_view name;
    name.data = "Knowledge Based App";
    name.len = (i32)strlen(name.data);

    app->details.flags.is_running = true;
    app->details.flags.should_close = false;
    app->details.data.id = window_counter + 1;

    char buffer[32]; 
    snprintf(buffer, sizeof(buffer), "KnowledgeBasedWindowClass%02d", window_counter);
    app->window.details.class_name.data = _strdup(buffer);
    app->window.details.class_name.len = (i32)strlen(buffer);

    window_create(&app->window, 800, 600, name);

    renderer_initialize(&app->renderer, &app->window);

    log_msg(LOG_LEVEL_INFO, "created application '%s': %d ", name.data, app->details.data.id);
    window_counter++;
}

void application_run(application_t *app) 
{
    MSG msg = {0};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT) {
            app->details.flags.should_close = true;
        }
    }

    if (!app->details.flags.should_close && 
        app->window.details.width > 0 && 
        app->window.details.height > 0) 
    {
        application_render(app);
    }
}

void application_render(application_t *app) 
{
    static float rotation = 0.0f;
    rotation += 0.01f;

    vertex_t vertices[] = {
        {-1.0f, -1.0f,  1.0f, 0xFFFF0000}, { 1.0f, -1.0f,  1.0f, 0xFFFF0000}, { 1.0f,  1.0f,  1.0f, 0xFFFF0000},
        {-1.0f, -1.0f,  1.0f, 0xFFFF0000}, { 1.0f,  1.0f,  1.0f, 0xFFFF0000}, {-1.0f,  1.0f,  1.0f, 0xFFFF0000},
        {-1.0f, -1.0f, -1.0f, 0xFF00FF00}, {-1.0f,  1.0f, -1.0f, 0xFF00FF00}, { 1.0f,  1.0f, -1.0f, 0xFF00FF00},
        {-1.0f, -1.0f, -1.0f, 0xFF00FF00}, { 1.0f,  1.0f, -1.0f, 0xFF00FF00}, { 1.0f, -1.0f, -1.0f, 0xFF00FF00},
        {-1.0f,  1.0f, -1.0f, 0xFF0000FF}, {-1.0f,  1.0f,  1.0f, 0xFF0000FF}, { 1.0f,  1.0f,  1.0f, 0xFF0000FF},
        {-1.0f,  1.0f, -1.0f, 0xFF0000FF}, { 1.0f,  1.0f,  1.0f, 0xFF0000FF}, { 1.0f,  1.0f, -1.0f, 0xFF0000FF},
        {-1.0f, -1.0f, -1.0f, 0xFFFFFF00}, { 1.0f, -1.0f, -1.0f, 0xFFFFFF00}, { 1.0f, -1.0f,  1.0f, 0xFFFFFF00},
        {-1.0f, -1.0f, -1.0f, 0xFFFFFF00}, { 1.0f, -1.0f,  1.0f, 0xFFFFFF00}, {-1.0f, -1.0f,  1.0f, 0xFFFFFF00},
        {-1.0f, -1.0f, -1.0f, 0xFF00FFFF}, {-1.0f, -1.0f,  1.0f, 0xFF00FFFF}, {-1.0f,  1.0f,  1.0f, 0xFF00FFFF},
        {-1.0f, -1.0f, -1.0f, 0xFF00FFFF}, {-1.0f,  1.0f,  1.0f, 0xFF00FFFF}, {-1.0f,  1.0f, -1.0f, 0xFF00FFFF},
        { 1.0f, -1.0f, -1.0f, 0xFFFF00FF}, { 1.0f,  1.0f, -1.0f, 0xFFFF00FF}, { 1.0f,  1.0f,  1.0f, 0xFFFF00FF},
        { 1.0f, -1.0f, -1.0f, 0xFFFF00FF}, { 1.0f,  1.0f,  1.0f, 0xFFFF00FF}, { 1.0f, -1.0f,  1.0f, 0xFFFF00FF},
    };
    u32 v_count = sizeof(vertices) / sizeof(vertex_t);

    dx9_vertex_buffer_update(&app->renderer.dx, vertices, v_count);

    renderer_beginframe(&app->renderer);
    LPDIRECT3DDEVICE9 dev = app->renderer.dx.context.device.handle;

    mat4_t mat_world, mat_proj;
    
    mat4_rotate_y(&mat_world, rotation);
    dev->lpVtbl->SetTransform(dev, D3DTS_WORLD, (D3DMATRIX*)&mat_world);

    mat4_t mat_view;
    vec3_t eye    = { 0.0f, 3.0f, -5.0f };
    vec3_t target = { 0.0f, 0.0f,  0.0f };
    vec3_t up     = { 0.0f, 1.0f,  0.0f };

    mat4_lookat(&mat_view, eye, target, up);
    dev->lpVtbl->SetTransform(dev, D3DTS_VIEW, (D3DMATRIX*)&mat_view);

    float aspect = (float)app->window.details.width / (float)app->window.details.height;
    mat4_perspective(&mat_proj, 0.785398f, aspect, 1.0f, 100.0f);
    dev->lpVtbl->SetTransform(dev, D3DTS_PROJECTION, (D3DMATRIX*)&mat_proj);

    dev->lpVtbl->SetRenderState(dev, D3DRS_LIGHTING, FALSE); 
    dev->lpVtbl->SetRenderState(dev, D3DRS_CULLMODE, D3DCULL_CCW);

    dx9_vertex_buffer_t* vbo = &app->renderer.dx.resource.buffer.v_buffer;
    if (vbo->buffer && vbo->vertex_count >= 3) {
        dev->lpVtbl->DrawPrimitive(dev, D3DPT_TRIANGLELIST, 0, vbo->vertex_count / 3);
    }

    renderer_endframe(&app->renderer);
}

b8 application_should_close(application_t *app) 
{
    return app->details.flags.should_close;
}

void application_destroy(application_t *app) 
{
    log_msg(LOG_LEVEL_INFO, "shutting down application...");
    
    renderer_destroy(&app->renderer);
    window_destroy(&app->window);

    if (app->window.details.class_name.data) {
        free((void*)app->window.details.class_name.data);
    }
}