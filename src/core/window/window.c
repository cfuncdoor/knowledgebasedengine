#include "core/window/window.h"
#include "core/logger/logger.h"

static LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            return 0;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            return 0;

        case WM_SIZE:
            return 0;

        case WM_MOUSEMOVE:
            return 0;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            return 0;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            return 0;

        case WM_MOUSEWHEEL:
            return 0;

        default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void window_create(window_t* window, i32 width, i32 height, string_view title) {
    WNDCLASSA wc = {0};
    window->win.hinstance = GetModuleHandle(NULL);
    window->details.width = (u32)width; //casting this integer back to signed int makes my brain tingle.
    window->details.height = (u32)height;
    window->details.title = title;

    wc.lpfnWndProc   = window_callback;
    wc.hInstance     = window->win.hinstance;
    wc.lpszClassName = window->details.class_name.data;
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
}

void window_destroy(window_t* window) 
{
    log_msg(LOG_LEVEL_INFO, "destroying the window...");
    ReleaseDC(window->win.hwnd, window->win.context.hdc);
}
