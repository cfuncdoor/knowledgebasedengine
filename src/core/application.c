#include "application.h"
#include "logger/logger.h"
#include "renderer/renderer.h"
#include "window/window.h"

static i32 window_counter = 0;

void application_create(application_t *app) 
{
    window_counter += 1;
    app->details.flags.is_running = true;
    app->details.flags.should_close = false;
    app->details.id = 0 + window_counter;
    
    window_create(&app->window, 800, 600, SV("Knowledge Based Engine"));
    log_msg(LOG_LEVEL_INFO, "created application '%s': %d ", app->window.details.title.data, app->details.id);
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