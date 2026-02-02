// THIS WAS AI GENERATED FOR TESTING PURPOSES, AND DOES NOT REPRESENT MY CODING SKILLS
// IT WAS JUST BECAUSE I DIDNT HAD THE TIME TO LINK A PROPER GL LIKE RAYLIB OR MAKE MY OWN

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace tgl {

static HWND   hwnd;
static HDC    back_dc;
static HBITMAP back_bmp;
static int    win_w, win_h;
static bool   is_running;

LRESULT CALLBACK wndproc(HWND h, UINT msg, WPARAM w, LPARAM l) {
    if (msg == WM_DESTROY) {
        is_running = false;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(h, msg, w, l);
}

inline bool init(int width, int height, const wchar_t* title) {
    win_w = width;
    win_h = height;

    WNDCLASS wc{};
    wc.lpfnWndProc = wndproc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = L"TINYGL_WINDOW";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        0, 0, wc.hInstance, 0
    );

    if (!hwnd) return false;

    HDC dc = GetDC(hwnd);
    back_dc = CreateCompatibleDC(dc);
    back_bmp = CreateCompatibleBitmap(dc, width, height);
    SelectObject(back_dc, back_bmp);
    ReleaseDC(hwnd, dc);

    is_running = true;
    return true;
}

inline bool running() {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return is_running;
}

inline void begin_frame(unsigned int rgb) {
    HBRUSH brush = CreateSolidBrush(
        RGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF)
    );
    RECT r{0, 0, win_w, win_h};
    FillRect(back_dc, &r, brush);
    DeleteObject(brush);
}

inline void draw_square(int x, int y, int size, unsigned int rgb) {
    HBRUSH brush = CreateSolidBrush(
        RGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF)
    );
    RECT r{ x, y, x + size, y + size };
    FillRect(back_dc, &r, brush);
    DeleteObject(brush);
}

inline void end_frame() {
    HDC dc = GetDC(hwnd);
    BitBlt(dc, 0, 0, win_w, win_h, back_dc, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, dc);
}

inline void shutdown() {
    DeleteObject(back_bmp);
    DeleteDC(back_dc);
    DestroyWindow(hwnd);
}

} // namespace tgl
