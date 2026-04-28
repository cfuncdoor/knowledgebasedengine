#include "core/application.h"
#include "core/logger/logger.h"
#include "core/renderer/renderer.h"
#include "core/window/window.h"
#include <stdio.h>
#include <string.h>

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

    app->renderer.window = &app->window;
    renderer_initialize(&app->renderer);

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

    if (app->window.details.width > 0 && app->window.details.height > 0) {
        application_render(app);
    }
}

void application_render(application_t *app) 
{
    renderer_beginframe(&app->renderer);
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