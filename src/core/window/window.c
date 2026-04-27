#include "window.h"
#include "../logger/logger.h"
#include "../renderer/renderer.h"
#include <wingdi.h>

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void window_create(window_t* window, i32 width, i32 height, string_view title) {
    window->win.hinstance = GetModuleHandle(NULL);
    window->details.width = width;
    window->details.height = height;
    window->details.title = title;

    WNDCLASSA wc = {0};
    wc.lpfnWndProc   = window_callback;
    wc.hInstance     = window->win.hinstance;
    wc.lpszClassName = "KnowledgeBasedClass001";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    window->win.hwnd = CreateWindowExA(
        0,
        wc.lpszClassName,
        title.data,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        width, height,
        NULL, NULL, 
        window->win.hinstance, 
        NULL
    );

    log_msg(LOG_LEVEL_INFO, "created window '%s' : %s ", window->details.title.data, wc.lpszClassName);

    window->win.context.hdc = GetDC(window->win.hwnd);
    setup_pixel_format(window->win.context.hdc);
    dx9_create(&window->dx, window->win.hwnd, window->details.width, window->details.height);
}

void window_destroy(window_t* window) 
{
    ReleaseDC(window->win.hwnd, window->win.context.hdc);
}