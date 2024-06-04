#ifndef _CORE_STRING_VIEW_H_
#define _CORE_STRING_VIEW_H_

#include "../build/build.h"
#include "string.h"

#include <stddef.h>

typedef struct StringView {
    size_t len;
    const char *data;
}StringView;

CORE_API StringView string_view_from(const char *data);
CORE_API StringView string_view_new(const char *data, size_t len);
CORE_API String string_view_into_string(StringView self);
#define string_view_new_const(s) ((StringView){ .len = sizeof((s)) - 1, .data = (s) })

#ifdef CORE_SHORT
#define sv_from string_view_from
#define sv_new string_view_new
#define sv_into_string string_view_into_string
#endif

#endif
