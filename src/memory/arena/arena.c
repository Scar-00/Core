#include "../../internal.h"

CORE_API ArenaAllocator arena_new(size_t size) {
     ArenaAllocator self = {
        .buffer = vec_with_size(char, size),
        .current_alloc = self.buffer,
        .next = NULL,
    };
    return self;
}

CORE_API void arena_dealloc(ArenaAllocator *self) {
    if(self->next) {
        arena_dealloc(self->next);
    }
    vec_destroy(self->buffer);
}

static void arena_clear_int(ArenaAllocator self) {
    self.current_alloc = self.buffer;
    if(self.next)
        arena_clear_int(*self.next);
}

static void* arena_alloc_internal(ArenaAllocator self, size_t size) {
    if(self.next) {
        return arena_alloc_internal(*self.next, size);
    }
    if(self.current_alloc + size > self.buffer + ARENA_DEFAULT_ALLOC_SIZE) {
        self.next = default_allocator.alloc(sizeof(ArenaAllocator));
        *self.next = arena_new(ARENA_DEFAULT_ALLOC_SIZE);
        return  arena_alloc_internal(*self.next, size);
    }
    void *alloc = self.current_alloc;
    self.current_alloc += size;
    return alloc;
}

CORE_API void *arena_alloc(ArenaAllocator alloc, size_t size) {
    return arena_alloc_internal(alloc, size);
}

CORE_API void *arena_realloc(ArenaAllocator alloc, void *src, size_t new_size) {
    (void)src;
    return NULL;
}

CORE_API void arena_clear(ArenaAllocator alloc) {
    arena_clear_int(alloc);
}
