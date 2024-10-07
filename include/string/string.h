#ifndef _CORE_STRING_H_
#define _CORE_STRING_H_

#include "../build/build.h"
#include "../build/defines.h"

#include <stddef.h>
#include <stdarg.h>

typedef struct String {
    enum {
        STRING_SHORT,
        STRING_LONG,
    }type;
    union {
        struct {
            char data[24];
        }s;
        struct {
            char *ptr;
            size_t cap;
            size_t len;
        }l;
    }data;
}String;

typedef struct StringView StringView;

CORE_API String string_new();
CORE_API String string_new_size(size_t size);
CORE_API String string_from(const char *ptr);
CORE_API String string_from_parts(const char *ptr, size_t len, size_t cap);
CORE_API String string_format(const char *format, ...);
CORE_API String string_vformat(const char *fmt, va_list args);
CORE_API void string_destroy(String *self);

CORE_API const char *string_cstr(String const *self);
CORE_API size_t string_cap(String const *self);
CORE_API size_t string_len(String const *self);

CORE_API void string_push(String *self, char c);
CORE_API void string_push_str(String *self, String other);
CORE_API void string_push_ptr(String *self, const char *ptr);
CORE_API void string_pop(String *self);
CORE_API bool string_cmp(String const *self, String const *other);
CORE_API bool string_cmp_sv(String const *self, StringView other);
CORE_API bool string_contains(String *self, StringView predicate);

CORE_API void string_dump(String const *self);

CORE_API String string_copy(String const *self);

CORE_API StringView string_into_view(String const *self);

#endif // _CORE_STRING_H_
