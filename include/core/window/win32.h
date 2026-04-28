#pragma once
#include <windows.h>

typedef struct win32_context_s {
    HDC hdc;
} win32_context_t;

typedef struct win32_s {
    HWND hwnd;
    HINSTANCE hinstance;
    win32_context_t context;
} win32_t;