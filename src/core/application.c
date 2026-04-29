#include "core/application.h"
#include "core/logger/logger.h"
#include "core/math/matrix/matrix.h"
#include "core/math/vertex/vertex.h"
#include "core/renderer/renderer.h"
#include "core/window/window.h"
#include "core/ecs/world.h"
#include "core/ecs/render_system.h"
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

    world_init(&app->ecs);

    log_msg(LOG_LEVEL_INFO, "world init done, mesh_id=%u transform_id=%u", app->ecs.mesh_id, app->ecs.transform_id);

    vertex_t cube_verts[] = {
        {-0.5f, -0.5f,  0.5f, 0xFFFF0000}, { 0.5f, -0.5f,  0.5f, 0xFFFF0000}, { 0.5f,  0.5f,  0.5f, 0xFFFF0000},
        {-0.5f, -0.5f,  0.5f, 0xFFFF0000}, { 0.5f,  0.5f,  0.5f, 0xFFFF0000}, {-0.5f,  0.5f,  0.5f, 0xFFFF0000},
        {-0.5f, -0.5f, -0.5f, 0xFF00FF00}, {-0.5f,  0.5f, -0.5f, 0xFF00FF00}, { 0.5f,  0.5f, -0.5f, 0xFF00FF00},
        {-0.5f, -0.5f, -0.5f, 0xFF00FF00}, { 0.5f,  0.5f, -0.5f, 0xFF00FF00}, { 0.5f, -0.5f, -0.5f, 0xFF00FF00},
        {-0.5f,  0.5f, -0.5f, 0xFF0000FF}, {-0.5f,  0.5f,  0.5f, 0xFF0000FF}, { 0.5f,  0.5f,  0.5f, 0xFF0000FF},
        {-0.5f,  0.5f, -0.5f, 0xFF0000FF}, { 0.5f,  0.5f,  0.5f, 0xFF0000FF}, { 0.5f,  0.5f, -0.5f, 0xFF0000FF},
        {-0.5f, -0.5f, -0.5f, 0xFFFFFF00}, { 0.5f, -0.5f, -0.5f, 0xFFFFFF00}, { 0.5f, -0.5f,  0.5f, 0xFFFFFF00},
        {-0.5f, -0.5f, -0.5f, 0xFFFFFF00}, { 0.5f, -0.5f,  0.5f, 0xFFFFFF00}, {-0.5f, -0.5f,  0.5f, 0xFFFFFF00},
        {-0.5f, -0.5f, -0.5f, 0xFF00FFFF}, {-0.5f, -0.5f,  0.5f, 0xFF00FFFF}, {-0.5f,  0.5f,  0.5f, 0xFF00FFFF},
        {-0.5f, -0.5f, -0.5f, 0xFF00FFFF}, {-0.5f,  0.5f,  0.5f, 0xFF00FFFF}, {-0.5f,  0.5f, -0.5f, 0xFF00FFFF},
        { 0.5f, -0.5f, -0.5f, 0xFFFF00FF}, { 0.5f,  0.5f, -0.5f, 0xFFFF00FF}, { 0.5f,  0.5f,  0.5f, 0xFFFF00FF},
        { 0.5f, -0.5f, -0.5f, 0xFFFF00FF}, { 0.5f,  0.5f,  0.5f, 0xFFFF00FF}, { 0.5f, -0.5f,  0.5f, 0xFFFF00FF},
    };

    entity_t e1 = world_create_entity(&app->ecs);
    world_set_mesh(&app->ecs, e1, cube_verts, 36);
    vec3_t pos1 = {-1.5f, 0.0f, 0.0f};
    vec3_t rot1 = {0.0f, 0.0f, 0.0f};
    vec3_t scale1 = {1.0f, 1.0f, 1.0f};
    world_set_transform(&app->ecs, e1, pos1, rot1, scale1);
    transform_t* t1 = world_get_transform(&app->ecs, e1);
    log_msg(LOG_LEVEL_INFO, "e1=%u pos=(%.2f,%.2f,%.2f) rot=(%.2f,%.2f,%.2f)", 
            e1, t1->position.x, t1->position.y, t1->position.z,
            t1->rotation.x, t1->rotation.y, t1->rotation.z);

    entity_t e2 = world_create_entity(&app->ecs);
    world_set_mesh(&app->ecs, e2, cube_verts, 36);
    vec3_t pos2 = {1.5f, 0.0f, 0.0f};
    vec3_t rot2 = {0.0f, 0.0f, 0.0f};
    vec3_t scale2 = {1.0f, 1.0f, 1.0f};
    world_set_transform(&app->ecs, e2, pos2, rot2, scale2);

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
        world_update(&app->ecs, 0.016f, &app->renderer);
        application_render(app);
    }
}

void application_render(application_t *app) 
{
    renderer_beginframe(&app->renderer);
    LPDIRECT3DDEVICE9 dev = app->renderer.dx.context.device.handle;

    render_system_update(&app->renderer, &app->ecs);

    mat4_t mat_view;
    vec3_t eye    = { 0.0f, 3.0f, -8.0f };
    vec3_t target = { 0.0f, 0.0f,  0.0f };
    vec3_t up     = { 0.0f, 1.0f,  0.0f };

    mat4_lookat(&mat_view, eye, target, up);
    dev->lpVtbl->SetTransform(dev, D3DTS_VIEW, (D3DMATRIX*)&mat_view);

    float aspect = (float)app->window.details.width / (float)app->window.details.height;
    mat4_t mat_proj;
    mat4_perspective(&mat_proj, 0.785398f, aspect, 1.0f, 100.0f);
    dev->lpVtbl->SetTransform(dev, D3DTS_PROJECTION, (D3DMATRIX*)&mat_proj);

    dev->lpVtbl->SetRenderState(dev, D3DRS_LIGHTING, FALSE); 
    dev->lpVtbl->SetRenderState(dev, D3DRS_CULLMODE, D3DCULL_NONE);

    renderer_endframe(&app->renderer);
}

b8 application_should_close(application_t *app) 
{
    return app->details.flags.should_close;
}

void application_destroy(application_t *app) 
{
    log_msg(LOG_LEVEL_INFO, "shutting down application...");
    
    world_destroy(&app->ecs);
    renderer_destroy(&app->renderer);
    window_destroy(&app->window);

    if (app->window.details.class_name.data) {
        free((void*)app->window.details.class_name.data);
    }
}