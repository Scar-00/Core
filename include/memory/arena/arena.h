#ifndef _CORE_ARENA_H_
#define _CORE_ARENA_H_

#include "../../build/build.h"
#include "../../containers/vector.h"

#include <stddef.h>

typedef struct ArenaAllocator ArenaAllocator;

struct ArenaAllocator {
    Vec(char) buffer;
    char *current_alloc;
    ArenaAllocator *next;
};

CORE_API ArenaAllocator arena_new(size_t size);
CORE_API void arena_dealloc(ArenaAllocator *arena);

CORE_API void *arena_alloc(ArenaAllocator alloc, size_t size);
CORE_API void *arena_realloc(ArenaAllocator alloc, void *src, size_t size);
CORE_API void arena_clear(ArenaAllocator alloc);

#endif // _CORE_ARENA_H_
