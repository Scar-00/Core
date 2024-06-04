#include "../internal.h"
#ifdef CORE_DEBUG

static void *debug_alloc(size_t size) {
    void *alloc = malloc(size);
    fprintf(stderr, "[INFO]: allocated `%zu` bytes at address `%p`\n", size, alloc);
    return alloc;
}

static void *debug_realloc(void *ptr, unsigned long new_size) {
    void *new = realloc(ptr, new_size);
    fprintf(stderr, "[INFO]: reallocated `%p` to `%p` location with size `%zu`\n", ptr, new, new_size);
    return new;
}

static void debug_free(void *block) {
    free(block);
    fprintf(stderr, "[INFO]: freed allocation at `%p`\n", block);
}

Allocator default_allocator = {
    .alloc = debug_alloc,
    .realloc = debug_realloc,
    .free = debug_free,
};

Allocator global_allocator = {
    .alloc = debug_alloc,
    .realloc = debug_realloc,
    .free = debug_free,
};

#else

Allocator default_allocator = {
    .alloc = malloc,
    .realloc = realloc,
    .free = free,
};

Allocator global_allocator = {
    .alloc = malloc,
    .realloc = realloc,
    .free = free,
};

#endif

CORE_API void set_global_allocator(Allocator alloc) {
    global_allocator = alloc;
}

CORE_API void clear_global_allocator() {
    global_allocator = default_allocator;
}
