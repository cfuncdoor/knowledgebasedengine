#include "core/application.h"
#include "core/logger/logger.h"
#include "core/renderer/renderer.h"
#include "core/types/types.h"
#include "core/window/window.h"
#include "core/types/types.h"
#include <stdio.h>

void application_create(application_t *app) 
{
    i32 window_counter = 0;
    string_view name;
    {
        name.data = "Knowledge Based App";
        name.len = (i32)sizeof(name.data) - 1;
    }   

    app->details.flags.is_running = true;
    app->details.flags.should_close = false;
    app->details.data.id = window_counter + 1;

    {
        char buffer[32]; //32 should be enough
        snprintf(buffer, sizeof(buffer), "KnowledgeBasedWindowClass%02d", window_counter);
        app->window.details.class_name.data = _strdup(buffer);
    }


    window_create(&app->window, 800, 600, name);
    log_msg(LOG_LEVEL_INFO, "created application '%s': %d ", app->window.details.title.data, app->details.data.id);
    window_counter += 1;
}

void application_run(application_t *app) 
{
    MSG msg = {0};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT) app->details.flags.should_close = true;
    }

    application_render(app);
}

void application_render(application_t *app) 
{
    dx9_render(&app->window.dx);
}

b8 application_should_close(application_t *app) 
{
    return app->details.flags.should_close;
}

void application_destroy(application_t *app) 
{
    window_destroy(&app->window);
}
