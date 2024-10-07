#ifndef _CORE_GFX_WINDOW_H_
#define _CORE_GFX_WINDOW_H_

#include "../build/build.h"
#include "../string/string_view.h"
#include "../build/defines.h"

typedef struct Window *WindowHandle;

typedef enum WindowFlags {
    WINDOW_NONE = 0,
    WINDOW_RESIZEABLE = CORE_BIT(1),
}WindowFlags;

typedef size_t WindowFlags_;
typedef size_t GfxError_;
typedef enum KeyAction {
    KEY_NONE = 0,
    KEY_DOWN,
    KEY_UP,
}KeyAction;

typedef void (*ProcAddr)(void);

typedef void (*ResizeCallback)(WindowHandle, size_t, size_t);
typedef void (*CursorCallback)(WindowHandle, double dx, double dy);
typedef void (*MouseCallback)(WindowHandle, int button, KeyAction, int mods);
typedef void (*ErrorCallback)(GfxError_);
typedef void (*KeyCallback)(WindowHandle, int key, int scancode, KeyAction action, int mod);

CORE_API WindowHandle window_init(size_t width, size_t height, StringView name, WindowFlags_ flags);
CORE_API void window_destroy(WindowHandle self);
CORE_API bool window_make_current(WindowHandle self);
CORE_API void window_swap_buffers(WindowHandle self);

CORE_API bool window_should_close(WindowHandle self);
CORE_API void window_poll_events(WindowHandle self);
CORE_API void window_set_resize_callback(WindowHandle self, ResizeCallback callback);
CORE_API void window_set_cursor_callback(WindowHandle self, CursorCallback callback);
CORE_API void window_set_mouse_callback(WindowHandle self, MouseCallback callback);
CORE_API void window_set_error_callback(WindowHandle self, ErrorCallback callback);
CORE_API void window_set_key_callback(WindowHandle self, KeyCallback callback);

CORE_API ProcAddr get_proc_addr(void *module, const char *name);

CORE_API void window_set_flag(WindowHandle self, WindowFlags_ flags);
CORE_API void window_clear_flag(WindowHandle self, WindowFlags_ flags);

#endif //_CORE_GFX_WINDOW_H_
