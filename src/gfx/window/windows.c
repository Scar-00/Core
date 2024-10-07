#include "../../internal.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>


typedef struct Callbacks {
    ResizeCallback resize_callback;
    MouseCallback mouse_callback;
    ErrorCallback error_callback;
    KeyCallback key_callback;
    CursorCallback cursor_callback;
}Callbacks;

static void resize_callback(WindowHandle window, size_t width, size_t height) {
    (void)window;
    (void)width;
    (void)height;
}

static void mouse_callback(WindowHandle window, int button, KeyAction action, int mods) {
    (void)window;
    (void)button;
    (void)action;
    (void)mods;
}

static void errro_callback(GfxError_ error) {
    (void)error;
}

static void key_callback(WindowHandle window, int button, int scancode, KeyAction action, int mods) {
    (void)window;
    (void)button;
    (void)action;
    (void)mods;
}

static void cursor_callback(WindowHandle window, double dx, double dy) {
    (void)window;
    (void)dx;
    (void)dy;
}

static Callbacks default_callbacks = {
    .resize_callback = resize_callback,
    .mouse_callback = mouse_callback,
    .error_callback = errro_callback,
    .key_callback = key_callback,
    .cursor_callback = cursor_callback,
};

typedef struct Window {
    WString name;
    size_t width, height;
    Callbacks callbacks;
    HINSTANCE instance;
    WNDCLASSEXW wnd_class;
    HWND hwnd;
    HDC hdc;
    HGLRC gl_context;
    bool should_close;
}Window;

static char scancode(LPARAM lparam) {
    char scancode = (lparam >> 16);
    return scancode;
}


// Stolen from GLFW
static int get_key_mods(void)
{
    int mods = 0;

    if (GetKeyState(VK_SHIFT) & 0x8000)
        mods |= KEY_MOD_SHIFT;
    if (GetKeyState(VK_CONTROL) & 0x8000)
        mods |= KEY_MOD_CONTROL;
    if (GetKeyState(VK_MENU) & 0x8000)
        mods |= KEY_MOD_ALT;
    if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
        mods |= KEY_MOD_SUPER;
    if (GetKeyState(VK_CAPITAL) & 1)
        mods |= KEY_MOD_CAPS_LOCK;
    if (GetKeyState(VK_NUMLOCK) & 1)
        mods |= KEY_MOD_NUM_LOCK;

    return mods;
}
static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowHandle window = GetPropW(hwnd, L"CORE");
    switch (msg) {
        case WM_KEYUP:
        case WM_KEYDOWN: {
            const KeyAction action = (HIWORD(lParam) & KF_UP) ? KEY_UP : KEY_DOWN;
            window->callbacks.key_callback(window, wParam, scancode(lParam), action, get_key_mods());
        }break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP: {
            int button, action;

            if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
                button = MOUSE_BUTTON_LEFT;
            else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
                button = MOUSE_BUTTON_RIGHT;
            else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
                button = MOUSE_BUTTON_MIDDLE;
            else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
                button = MOUSE_BUTTON_4;
            else
                button = MOUSE_BUTTON_5;

            if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN ||
                msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN)
            {
                action = KEY_DOWN;
            }
            else
                action = KEY_UP;

            window->callbacks.mouse_callback(window, button, action, get_key_mods());
        }break;
        case WM_CLOSE: {
            window->should_close = true;
            DestroyWindow(hwnd);
        }break;
        case WM_QUIT: {
            window->should_close = true;
        }break;
        case WM_DESTROY: {
            PostQuitMessage(0);
        }break;
        default: {
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }break;
    }
    return 0;
}

CORE_API WindowHandle window_init(size_t width, size_t height, StringView name, WindowFlags_ flags) {
    WString wnd_name = wstring_from_view(name);
    HINSTANCE instance = GetModuleHandle(NULL);

    WNDCLASSEXW wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = window_proc,
        .hInstance = instance,
        .hIcon = LoadIconW(NULL, IDI_APPLICATION),
        .hCursor = LoadCursorW(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = L"CORE_WINDOW",
        .hIconSm = LoadIconW(NULL, IDI_APPLICATION),
    };

    if(!RegisterClassExW(&wc)) {
        return NULL;
    }

    HWND hwnd = CreateWindowExW(0, wc.lpszClassName, wstring_cstr(&wnd_name), 0, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, instance, NULL);
    if(!hwnd) {
        return NULL;
    }

    HDC hdc = GetDC(hwnd);
    if(!hdc) {
        return NULL;
    }

    HGLRC gl_context = wglCreateContext(hdc);
    if(!gl_context) {
        return NULL;
    }

    WindowHandle self = global_allocator.alloc(sizeof(Window));
    *self = (Window) {
        .name = wnd_name,
        .width = width,
        .height = height,
        .instance = instance,
        .wnd_class = wc,
        .hwnd = hwnd,
        .hdc = hdc,
        .gl_context = gl_context,
        .callbacks = default_callbacks,
    };

    SetPropW(hwnd, L"CORE", self);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    return self;
}

CORE_API void window_destroy(WindowHandle self) {
    ReleaseDC(self->hwnd, self->hdc);
    wstring_destroy(&self->name);
    global_allocator.free(self);
}

CORE_API bool window_make_current(WindowHandle self) {
    return wglMakeCurrent(self->hdc, self->gl_context);
}

CORE_API void window_swap_buffers(WindowHandle self) {
    SwapBuffers(self->hdc);
}

CORE_API bool window_should_close(WindowHandle self) {
    return self->should_close;
}

CORE_API void window_poll_events(WindowHandle self) {
    MSG msg;
    while(GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

CORE_API void window_set_resize_callback(WindowHandle self, ResizeCallback callback) {
    self->callbacks.resize_callback = callback;
}

CORE_API void window_set_cursor_callback(WindowHandle self, CursorCallback callback) {
    self->callbacks.cursor_callback = callback;
}

CORE_API void window_set_mouse_callback(WindowHandle self, MouseCallback callback) {
    self->callbacks.mouse_callback = callback;
}

CORE_API void window_set_error_callback(WindowHandle self, ErrorCallback callback) {
    self->callbacks.error_callback = callback;
}

CORE_API void window_set_key_callback(WindowHandle self, KeyCallback callback) {
    self->callbacks.key_callback = callback;
}

CORE_API ProcAddr get_proc_addr(void *module, const char *name) {
    return (ProcAddr)GetProcAddress((HMODULE)module, name);
}
