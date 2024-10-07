#ifndef _CORE_STRING_VIEW_H_
#define _CORE_STRING_VIEW_H_

#include "../build/build.h"
#include "../build/defines.h"
#include "string.h"

#include <stddef.h>

typedef struct StringView {
    size_t len;
    const char *data;
}StringView;

CORE_API StringView string_view_from(const char *data);
CORE_API StringView string_view_new(const char *data, size_t len);
CORE_API String string_view_into_string(StringView self);
CORE_API StringView string_view_copy(StringView self);
#define string_view_new_const(s) ((StringView){ .len = sizeof((s)) - 1, .data = (s) })

CORE_API bool string_view_contains(StringView self, StringView predicate);

#ifdef CORE_SHORT
#define sv stringstring_view_new_const
#define sv_from string_view_from
#define sv_new string_view_new
#define sv_into_string string_view_into_string
#define sv_contains stringstring_view_contains
#endif

typedef struct WString WString;
typedef struct WStringView {
    size_t len;
    const wchar *data;
}WStringView;

CORE_API WStringView wstring_view_from(const wchar *data);
CORE_API WStringView wstring_view_new(const wchar *data, size_t len);
CORE_API WString wstring_view_into_string(WStringView self);
CORE_API WStringView wstring_view_copy(WStringView self);
#define wstring_view_new_const(s) ((WStringView){ .len = sizeof((s)) - 1, .data = (s) })

CORE_API bool wstring_view_contains(WStringView self, WStringView predicate);

#endif
