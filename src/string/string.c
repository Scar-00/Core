#include "../internal.h"
#include <string.h>
#include <stdarg.h>

//  the caller needs to garuentee that both pointers are valid and cannot go oob
bool partial_cmp_ptr(const char *self, const char *predicate, size_t len) {
    for(size_t i = 0; i < len; i ++) {
        if(self[i] != predicate[i]) {
            return false;
        }
    }
    return true;
}

static char string_cpy_buffer[SHORT_STRING_LENGTH] = {0};
static void string_transform(String *self) {
    if(self->type == STRING_LONG) {
        size_t len = self->data.l.len;
        memcpy(string_cpy_buffer, self->data.l.ptr, len * sizeof(char));
        global_allocator.free(self->data.l.ptr);
        memmove(self->data.s.data, string_cpy_buffer, len * sizeof(char));
        self->type = STRING_SHORT;
    }else {
        memcpy(string_cpy_buffer, self->data.s.data, SHORT_STRING_LENGTH * sizeof(char));
        self->data.l.ptr = global_allocator.alloc((SHORT_STRING_LENGTH + 1) * sizeof(char));
        memcpy(self->data.l.ptr, string_cpy_buffer, SHORT_STRING_LENGTH * sizeof(char));
        self->data.l.len = SHORT_STRING_LENGTH;
        self->data.l.cap = SHORT_STRING_LENGTH + 1;
        self->type = STRING_LONG;
    }
    memset(string_cpy_buffer, 0, sizeof(*string_cpy_buffer));
    //printf("[info]: transformed string\n");
}

static void string_grow(String *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_grow`");
    CORE_ASSERT(self->type == STRING_LONG && "error: cannot grow small `String`");
    size_t new_size = (self->data.l.cap * STRING_GROW_FACTOR);
    self->data.l.ptr = global_allocator.realloc(self->data.l.ptr, new_size * sizeof(char));
    self->data.l.cap = new_size;
    //printf("[info]: grew string\n");
}

CORE_API String string_new() {
    return (String){
        .type = STRING_SHORT,
        .data = {0},
    };
}

CORE_API String string_new_size(size_t size) {
    String self = {0};
    self = (String){
        .type = size > SHORT_STRING_LENGTH ? STRING_LONG : STRING_SHORT,
    };
    if(self.type == STRING_LONG) {
        self.data.l.cap = size;
        self.data.l.ptr = global_allocator.alloc((size + 1) * sizeof(char));
        CORE_ASSERT(self.data.l.ptr && "error: failed to allocate `String`");
    }
    return self;
}

CORE_API String string_from(const char *ptr) {
    CORE_ASSERT(ptr && "error: cannot pass nullptr to `string_from`");
    //FIXME: do not put the fucking nulltermination char in length ????
    size_t len = strlen(ptr);
    String self = string_new_size(len);
    if(self.type == STRING_LONG) {
        self.data.l.len = len;
        strcpy(self.data.l.ptr, ptr);
    }else {
        self.data.s.data[23] = SHORT_STRING_LENGTH - len;
        strcpy(self.data.s.data, ptr);
    }
    return self;
}

CORE_API String string_from_parts(const char *ptr, size_t len, size_t cap) {
    if(cap > SHORT_STRING_LENGTH) {
        String str = {
            .type = STRING_LONG,
            .data.l.ptr = global_allocator.alloc(cap * sizeof(char)),
            .data.l.len = len,
            .data.l.cap = cap,
        };
        strcpy(str.data.l.ptr, ptr);
        return str;
    }
    String str = { .type = STRING_SHORT };
    str.data.s.data[23] = SHORT_STRING_LENGTH - len;
    strcpy(str.data.s.data, ptr);
    return str;
}

CORE_API String string_format(const char *format, ...) {
    va_list args;
    va_start(args, format);
    String self = string_vformat(format, args);
    va_end(args);
    return self;
}

CORE_API String string_vformat(const char *fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = vsnprintf(NULL, 0, fmt, args);

    String self = string_new_size(size);
    if(self.type == STRING_SHORT) {
        vsnprintf(self.data.s.data, size + 1, fmt, args_copy);
        self.data.s.data[23] = SHORT_STRING_LENGTH - size;
    }else {
        vsnprintf(self.data.l.ptr, size + 1, fmt, args_copy);
        self.data.l.len = size;
    }

    return self;
}

CORE_API void string_destroy(String *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_destroy`");
    if(self->type == STRING_SHORT)
        return;
    global_allocator.free(self->data.l.ptr);
}

CORE_API const char *string_cstr(String const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_cstr`");
    if(self->type == STRING_LONG) {
        return self->data.l.ptr;
    }
    return self->data.s.data;
}

CORE_API size_t string_cap(String const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_cap`");
    if(self->type == STRING_SHORT) {
        return SHORT_STRING_LENGTH;
    }
    return self->data.l.cap;
}

CORE_API size_t string_len(String const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_len`");
    if(self->type == STRING_SHORT) {
        return SHORT_STRING_LENGTH - self->data.s.data[23];
    }
    return self->data.l.len;
}

CORE_API void string_push(String *self, char c) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_push`");
    if(self->type == STRING_SHORT) {
        if((SHORT_STRING_LENGTH - self->data.s.data[23]) >= SHORT_STRING_LENGTH) {
            string_transform(self);
            self->data.l.ptr[self->data.l.len - 1] = c;
            self->data.l.ptr[self->data.l.len++] = '\0';
            //string_dump(self);
            return;
        }
        self->data.s.data[SHORT_STRING_LENGTH - self->data.s.data[23] - 1] = c;
        self->data.s.data[SHORT_STRING_LENGTH - self->data.s.data[23]--] = '\0';
        //string_dump(self);
        return;
    }
    if(self->data.l.len == self->data.l.cap) {
        string_grow(self);
    }
    self->data.l.ptr[self->data.l.len - 1] = c;
    self->data.l.ptr[self->data.l.len++] = '\0';
    //string_dump(self);
}

CORE_API void string_push_str(String *self, String other) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_push_str`");
    for(size_t i = 0; i < string_len(&other) - 1; i++) {
        string_push(self, string_cstr(&other)[i]);
    }
}

CORE_API void string_push_ptr(String *self, const char *ptr) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_push_ptr`");
    while(*ptr != '\0') {
        string_push(self, *ptr++);
    }
}

CORE_API void string_pop(String *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_pop`");
    CORE_ASSERT(string_len(self) > 0 && "error: cannot pop from empty `String`");
    if(self->type == STRING_SHORT) {
        self->data.s.data[(SHORT_STRING_LENGTH - ++self->data.s.data[23]) - 1] = '\0';
    }else {
        self->data.l.ptr[(--self->data.l.len) - 1] = '\0';
        if((self->data.l.len) <= 24) {
            string_transform(self);
        }
    }
    //string_dump(self);
}

CORE_API bool string_cmp(String const *self, String const *other) {
    size_t self_len = string_len(self);
    if(self_len != string_len(other)) {
        return false;
    }

    const char *self_ptr = string_cstr(self);
    const char *other_ptr = string_cstr(other);
    for(size_t i = 0; i < self_len; i++) {
        if(self_ptr[i] != other_ptr[i]) {
            return false;
        }
    }
    return true;
}

CORE_API bool string_cmp_sv(String const *self, StringView other) {
    size_t self_len = string_len(self);
    if(self_len != other.len) {
        return false;
    }

    const char *self_ptr = string_cstr(self);
    for(size_t i = 0; i < self_len; i++) {
        if(self_ptr[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

CORE_API bool string_contains(String *self, StringView predicate) {
    if(predicate.len == 0) {
        return false;
    }

    const char *self_ptr = string_cstr(self);
    for(size_t i = 0; i < string_len(self); i++) {
        if(partial_cmp_ptr(&self_ptr[i], predicate.data, predicate.len)) {
            return true;
        }
    }

    return false;
}

CORE_API void string_dump(String const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_dump`");
    fprintf(stdout, "String { data: \"%s\", len: %zu, cap: %zu, type: %s }\n",
            string_cstr(self),
            string_len(self),
            string_cap(self),
            self->type == STRING_SHORT ? "`short`" : "`long`");
}

CORE_API String string_copy(String const *self) {
    return string_from_parts(string_cstr(self), string_len(self), string_cap(self));
}

CORE_API StringView string_into_view(String const *self) {
    return (StringView){
        .len = string_len(self),
        .data = string_cstr(self),
    };
}
