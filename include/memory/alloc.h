#ifndef _CORE_ALLOC_H_
#define _CORE_ALLOC_H_

#include "../build/build.h"

#include <stddef.h>

typedef void *(*AllocFn)(size_t size);
typedef void *(*ReallocFn)(void *mem, size_t size);
typedef void (*FreeFn)(void *_block);
typedef struct Allocator{
    AllocFn alloc;
    ReallocFn realloc;
    FreeFn free;
}Allocator;

extern Allocator global_allocator;

CORE_API void global_allocator_set(Allocator alloc);
CORE_API void global_allocator_clear(void);

#endif // _CORE_ALLOC_H_
