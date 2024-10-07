#ifndef _CORE_WSTRING_H_
#define _CORE_WSTRING_H_

#include "../build/build.h"
#include "../build/defines.h"

#include <stddef.h>
#include <stdarg.h>

typedef struct WString {
    int type;
    union {
        struct {
            wchar data[24];
        }s;
        struct {
            wchar *ptr;
            size_t cap;
            size_t len;
        }l;
    }data;
}WString;

typedef struct WStringView WStringView;
typedef struct StringView StringView;
typedef struct String String;

CORE_API WString wstring_new();
CORE_API WString wstring_new_size(size_t size);
CORE_API WString wstring_from(const wchar *ptr);
CORE_API WString wstring_from_parts(const wchar *ptr, size_t len, size_t cap);
CORE_API WString wstring_format(const wchar *format, ...);
CORE_API WString wstring_vformat(const wchar *fmt, va_list args);
CORE_API WString wstring_from_string(String const *string);
CORE_API WString wstring_from_view(StringView view);
CORE_API void wstring_destroy(WString *self);

CORE_API const wchar *wstring_cstr(WString const *self);
CORE_API size_t wstring_cap(WString const *self);
CORE_API size_t wstring_len(WString const *self);

CORE_API void wstring_push(WString *self, wchar c);
CORE_API void wstring_push_str(WString *self, WString other);
CORE_API void wstring_push_ptr(WString *self, const wchar *ptr);
CORE_API void wstring_pop(WString *self);
CORE_API bool wstring_cmp(WString const *self, WString const *other);
CORE_API bool wstring_cmp_sv(WString const *self, WStringView other);
CORE_API bool wstring_contains(WString *self, WStringView predicate);

CORE_API void wstring_dump(WString const *self);

CORE_API WString wstring_copy(WString const *self);

CORE_API WStringView wstring_into_view(WString const *self);

#endif // _CORE_WSTRING_H_
