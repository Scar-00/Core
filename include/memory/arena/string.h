#ifndef _CORE_ARENA_STRING_H_
#define _CORE_ARENA_STRING_H_

#include "arena.h"
#include "../../build/build.h"

#include <stddef.h>
#include <stdarg.h>

typedef struct AString {
    ArenaAllocator arena;
    char *ptr;
    size_t cap;
    size_t len;
}AString;

CORE_API AString astring_new(ArenaAllocator arena);
CORE_API AString astring_new_size(ArenaAllocator arena, size_t size);
CORE_API AString astring_from(ArenaAllocator arena, const char *ptr);
CORE_API AString astring_from_parts(ArenaAllocator arena, const char *ptr, size_t len, size_t cap);
CORE_API AString astring_format(ArenaAllocator arena, const char *format, ...);
CORE_API AString astring_vformat(ArenaAllocator arena, const char *fmt, va_list args);

CORE_API const char *astring_cstr(AString const *self);
CORE_API size_t astring_cap(AString const *self);
CORE_API size_t astring_len(AString const *self);

CORE_API void astring_push(AString *self, char c);
CORE_API void astring_push_str(AString *self, AString other);
CORE_API void astring_push_ptr(AString *self, const char *ptr);
CORE_API void astring_pop(AString *self);

CORE_API void astring_dump(AString const *self);

CORE_API AString astring_copy(AString const *self);

#endif // _CORE_ARENA_STRING_H_
