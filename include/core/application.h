#pragma once
#include "types/types.h"
#include "window/window.h"

typedef struct {
    b8 is_running;
    b8 should_close;
} application_flags_t;

typedef struct {
    i32 id;
} application_data_t;

typedef struct {
    u8 padding[2];
    application_flags_t flags;
    application_data_t data;
} application_details_t;

typedef struct {
    application_details_t details;
    window_t window;
} application_t;

void application_create(application_t* app);
void application_run(application_t* app);
void application_render(application_t* app);
b8 application_should_close(application_t* app);
void application_destroy(application_t* app);
