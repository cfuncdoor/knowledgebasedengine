#pragma once
#include <windows.h>
#include "../types/types.h"
#include "../renderer/renderer.h"

typedef struct {
    HDC hdc;
} win32_context_t;

typedef struct {
    HWND hwnd;
    HINSTANCE hinstance;
    win32_context_t context;
} win32_t;

typedef struct {
    i32 width;
    i32 height;
    string_view title;
} window_details_t;

typedef struct {
    win32_t win;
    dx9_t dx;
    window_details_t details;
} window_t;

void window_create(window_t* window, i32 width, i32 height, string_view title);
void window_destroy(window_t* window);