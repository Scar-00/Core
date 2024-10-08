#include "../../internal.h"

static void astring_grow(AString *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_grow`");
    size_t new_size = (self->cap * STRING_GROW_FACTOR);
    self->ptr = arena_realloc(self->arena, self->ptr, new_size * sizeof(char));
    self->cap = new_size;
}

CORE_API AString astring_new(ArenaAllocator arena) {
    return (AString){
        .arena = arena,
        .ptr = NULL,
        .cap = 0,
        .len = 0,
    };
}

CORE_API AString astring_new_size(ArenaAllocator arena, size_t size) {
    char *ptr = arena_alloc(arena, size);
    CORE_ASSERT(ptr && "error: failed to allocate `AString`");
    return (AString){
        .arena = arena,
        .ptr = ptr,
        .cap = size,
        .len = 0,
    };
}

CORE_API AString astring_from(ArenaAllocator arena, const char *ptr) {
    CORE_ASSERT(ptr && "error: cannot pass nullptr to `astring_from`");
    size_t size = strlen(ptr);
    AString self = astring_new_size(arena, size);
    strcpy_s(self.ptr, size + 1, ptr);
    self.len = size;
    return self;
}

CORE_API AString astring_from_parts(ArenaAllocator arena, const char *ptr, size_t len, size_t cap) {
    AString self = {
        .arena = arena,
        .ptr = arena_alloc(arena, cap),
        .len = len,
        .cap = cap
    };
    strcpy_s(self.ptr, self.cap, ptr);
    return self;
}

CORE_API AString astring_format(ArenaAllocator arena, const char *format, ...) {
    va_list args;
    va_start(args, format);
    AString self = astring_vformat(arena, format, args);
    va_end(args);
    return self;
}

CORE_API AString astring_vformat(ArenaAllocator arena, const char *fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = vsnprintf(NULL, 0, fmt, args);
    AString self = astring_new_size(arena, size);
    vsnprintf(self.ptr, size + 1, fmt, args_copy);
    self.len = size;
    return self;
}

CORE_API AString astring_from_string(ArenaAllocator arena, String const *string) {
    return astring_from_parts(arena, string_cstr(string), string_len(string), string_cap(string));
}

CORE_API const char *astring_cstr(AString const *self) {
    return self->ptr;
}

CORE_API size_t astring_cap(AString const *self) {
    return self->cap;
}

CORE_API size_t astring_len(AString const *self) {
    return self->len;
}

CORE_API void astring_push(AString *self, char c) {
    if(self->len == self->cap) {
        astring_grow(self);
    }
    self->ptr[self->len - 1] = c;
    self->ptr[self->len++] = '\0';
}

CORE_API void astring_push_str(AString *self, AString other) {
    for(size_t i = 0; i < astring_len(self); i++) {
        astring_push(self, astring_cstr(&other)[i]);
    }
}

CORE_API void astring_push_ptr(AString *self, const char *ptr) {
    while(*ptr) {
        astring_push(self, *ptr++);
    }
}

CORE_API void astring_pop(AString *self) {
    CORE_ASSERT(self->len > 0 && "error: cannot pop of an empty `AString`");
    self->ptr[(--self->len) - 1] = '\0';
}

CORE_API bool astring_cmp(AString const *self, AString const *other) {
    size_t self_len = astring_len(self);
    if(self_len != astring_len(other)) {
        return false;
    }

    const char *self_ptr = astring_cstr(self);
    const char *other_ptr = astring_cstr(other);
    for(size_t i = 0; i < self_len; i++) {
        if(self_ptr[i] != other_ptr[i]) {
            return false;
        }
    }
    return true;
}

CORE_API bool astring_contains(AString const *self, AString const *predicate) {
    if(astring_len(predicate) == 0) {
        return false;
    }

    const char *self_ptr = astring_cstr(self);
    const char *predicate_ptr = astring_cstr(predicate);
    for(size_t i = 0; i < astring_len(self); i++) {
        if(partial_cmp_ptr(&self_ptr[i], predicate_ptr, astring_len(predicate))) {
            return true;
        }
    }

    return false;
}

CORE_API bool astring_contains_sv(AString const *self, StringView predicate) {
    if(predicate.len == 0) {
        return false;
    }

    const char *self_ptr = astring_cstr(self);
    for(size_t i = 0; i < astring_len(self); i++) {
        if(partial_cmp_ptr(&self_ptr[i], predicate.data, predicate.len)) {
            return true;
        }
    }

    return false;
}

CORE_API void astring_dump(AString const *self) {
    fprintf(stdout, "AString { ptr: \"%s\", len: %zu, cap: %zu }\n", self->ptr, self->len, self->cap);
}

CORE_API AString astring_copy(AString const *self) {
       return astring_from_parts(self->arena, astring_cstr(self), astring_len(self), astring_cap(self));
}

CORE_API String astring_to_string(AString const *self) {
    return string_from_parts(self->ptr, self->len, self->cap);
}
