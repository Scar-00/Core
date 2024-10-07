#include "../internal.h"
#include <string.h>
#include <stdarg.h>
#include <wchar.h>

size_t wstrlen(const wchar *ptr) {
    size_t len = 0;
    while(*ptr++) {
        len++;
    }
    return len;
}

size_t wstrcpy(wchar *dest, const wchar *src, size_t len) {
    size_t i;
    for(i = 0; i <= len; i++) {
        dest[i] = src[i];
    }
    return i;
}

bool wpartial_cmp_ptr(const wchar *self, const wchar *predicate, size_t len) {
    for(size_t i = 0; i < len; i ++) {
        if(self[i] != predicate[i]) {
            return false;
        }
    }
    return true;
}

const WString utf8_from_ascii(StringView self) {

    //mbstowcs_s(wchar_t *Dest, const char *Source, size_t MaxCount)
    (void)self;
    return (WString){};
}

static wchar string_cpy_buffer[SHORT_STRING_LENGTH] = {0};
static void wstring_transform(WString *self) {
    if(self->type == STRING_LONG) {
        size_t len = self->data.l.len;
        memcpy(string_cpy_buffer, self->data.l.ptr, len * sizeof(wchar));
        global_allocator.free(self->data.l.ptr);
        memmove(self->data.s.data, string_cpy_buffer, len * sizeof(wchar));
        self->type = STRING_SHORT;
    }else {
        memcpy(string_cpy_buffer, self->data.s.data, SHORT_STRING_LENGTH * sizeof(wchar));
        self->data.l.ptr = global_allocator.alloc((SHORT_STRING_LENGTH + 1) * sizeof(wchar));
        memcpy(self->data.l.ptr, string_cpy_buffer, SHORT_STRING_LENGTH * sizeof(wchar));
        self->data.l.len = SHORT_STRING_LENGTH;
        self->data.l.cap = SHORT_STRING_LENGTH + 1;
        self->type = STRING_LONG;
    }
    memset(string_cpy_buffer, 0, sizeof(*string_cpy_buffer));
    //printf("[info]: transformed string\n");
}

static void wstring_grow(WString *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_grow`");
    CORE_ASSERT(self->type == STRING_LONG && "error: cannot grow small `String`");
    size_t new_size = (self->data.l.cap * STRING_GROW_FACTOR);
    self->data.l.ptr = global_allocator.realloc(self->data.l.ptr, new_size * sizeof(wchar));
    self->data.l.cap = new_size;
    //printf("[info]: grew string\n");
}

CORE_API WString wstring_new() {
    return (WString){
        .type = STRING_SHORT,
        .data = {0},
    };
}

CORE_API WString wstring_new_size(size_t size) {
    WString self = {0};
    self = (WString){
        .type = size > SHORT_STRING_LENGTH ? STRING_LONG : STRING_SHORT,
    };
    if(self.type == STRING_LONG) {
        self.data.l.cap = size;
        self.data.l.ptr = global_allocator.alloc((size + 1) * sizeof(wchar));
        CORE_ASSERT(self.data.l.ptr && "error: failed to allocate `String`");
    }
    return self;
}

CORE_API WString wstring_from(const wchar *ptr) {
    CORE_ASSERT(ptr && "error: cannot pass nullptr to `string_from`");
    //FIXME: do not put the fucking nulltermination char in length ????
    size_t len = wstrlen(ptr);
    WString self = wstring_new_size(len);
    if(self.type == STRING_LONG) {
        self.data.l.len = len;
        wstrcpy(self.data.l.ptr, ptr, len);
    }else {
        self.data.s.data[23] = SHORT_STRING_LENGTH - len;
        wstrcpy(self.data.s.data, ptr, len);
    }
    return self;
}

CORE_API WString wstring_from_parts(const wchar *ptr, size_t len, size_t cap) {
    if(cap > SHORT_STRING_LENGTH) {
        WString str = {
            .type = STRING_LONG,
            .data.l.ptr = global_allocator.alloc(cap * sizeof(wchar)),
            .data.l.len = len,
            .data.l.cap = cap,
        };
        wstrcpy(str.data.l.ptr, ptr, cap);
        return str;
    }
    WString str = { .type = STRING_SHORT };
    str.data.s.data[23] = SHORT_STRING_LENGTH - len;
    wstrcpy(str.data.s.data, ptr, len);
    return str;
}

CORE_API WString wstring_format(const wchar *format, ...) {
    va_list args;
    va_start(args, format);
    WString self = wstring_vformat(format, args);
    va_end(args);
    return self;
}

CORE_API WString wstring_vformat(const wchar *fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = _vsnwprintf_s(NULL, 0, 0, fmt, args);

    WString self = wstring_new_size(size);
    if(self.type == STRING_SHORT) {
        _vsnwprintf_s(self.data.s.data, size + 1, size + 1, fmt, args_copy);
        self.data.s.data[23] = SHORT_STRING_LENGTH - size;
    }else {
        _vsnwprintf_s(self.data.l.ptr, size + 1, size + 1, fmt, args_copy);
        self.data.l.len = size;
    }

    return self;
}

CORE_API WString wstring_from_string(String const *string) {
    WString self = wstring_new_size(string_cap(string));
    mbstate_t state;
    wchar *self_ptr = (wchar*)wstring_cstr(&self);
    const char *string_ptr = string_cstr(string);
    for(size_t i = 0; i < string_len(string); i++) {
        mbrtowc(&self_ptr[i], &string_ptr[i], string_len(string), &state);
    }

    return self;
}

CORE_API WString wstring_from_view(StringView view) {
    WString self = wstring_new_size(view.len);
    mbstate_t state;
    wchar *self_ptr = (wchar*)wstring_cstr(&self);
    for(size_t i = 0; i < view.len; i++) {
        mbrtowc(&self_ptr[i], &view.data[i], view.len, &state);
    }

    return self;
}

CORE_API void wstring_destroy(WString *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_destroy`");
    if(self->type == STRING_SHORT)
        return;
    global_allocator.free(self->data.l.ptr);
}

CORE_API const wchar *wstring_cstr(WString const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_cstr`");
    if(self->type == STRING_LONG) {
        return self->data.l.ptr;
    }
    return self->data.s.data;
}

CORE_API size_t wstring_cap(WString const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_cap`");
    if(self->type == STRING_SHORT) {
        return SHORT_STRING_LENGTH;
    }
    return self->data.l.cap;
}

CORE_API size_t wstring_len(WString const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_len`");
    if(self->type == STRING_SHORT) {
        return SHORT_STRING_LENGTH - self->data.s.data[23];
    }
    return self->data.l.len;
}

CORE_API void wstring_push(WString *self, wchar c) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_push`");
    if(self->type == STRING_SHORT) {
        if((SHORT_STRING_LENGTH - self->data.s.data[23]) >= SHORT_STRING_LENGTH) {
            wstring_transform(self);
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
        wstring_grow(self);
    }
    self->data.l.ptr[self->data.l.len - 1] = c;
    self->data.l.ptr[self->data.l.len++] = '\0';
    //string_dump(self);
}

CORE_API void wstring_push_str(WString *self, WString other) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_push_str`");
    for(size_t i = 0; i < wstring_len(&other) - 1; i++) {
        wstring_push(self, wstring_cstr(&other)[i]);
    }
}

CORE_API void wstring_push_ptr(WString *self, const wchar *ptr) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_push_ptr`");
    while(*ptr != '\0') {
        wstring_push(self, *ptr++);
    }
}

CORE_API void wstring_pop(WString *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_pop`");
    CORE_ASSERT(wstring_len(self) > 0 && "error: cannot pop from empty `String`");
    if(self->type == STRING_SHORT) {
        self->data.s.data[(SHORT_STRING_LENGTH - ++self->data.s.data[23]) - 1] = '\0';
    }else {
        self->data.l.ptr[(--self->data.l.len) - 1] = '\0';
        if((self->data.l.len) <= 24) {
            wstring_transform(self);
        }
    }
    //string_dump(self);
}

CORE_API bool wstring_cmp(WString const *self, WString const *other) {
    size_t self_len = wstring_len(self);
    if(self_len != wstring_len(other)) {
        return false;
    }

    const wchar *self_ptr = wstring_cstr(self);
    const wchar *other_ptr = wstring_cstr(other);
    for(size_t i = 0; i < self_len; i++) {
        if(self_ptr[i] != other_ptr[i]) {
            return false;
        }
    }
    return true;
}

CORE_API bool wstring_cmp_sv(WString const *self, WStringView other) {
    size_t self_len = wstring_len(self);
    if(self_len != other.len) {
        return false;
    }

    const wchar *self_ptr = wstring_cstr(self);
    for(size_t i = 0; i < self_len; i++) {
        if(self_ptr[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

CORE_API bool wstring_contains(WString *self, WStringView predicate) {
    if(predicate.len == 0) {
        return false;
    }

    const wchar *self_ptr = wstring_cstr(self);
    for(size_t i = 0; i < wstring_len(self); i++) {
        if(wpartial_cmp_ptr(&self_ptr[i], predicate.data, predicate.len)) {
            return true;
        }
    }

    return false;
}

CORE_API void wstring_dump(WString const *self) {
    CORE_ASSERT(self && "error: cannot pass nullptr to `string_dump`");
    fwprintf(stdout, L"String { data: \"%ws\", len: %zu, cap: %zu, type: %s }\n",
            wstring_cstr(self),
            wstring_len(self),
            wstring_cap(self),
            self->type == STRING_SHORT ? L"`short`" : L"`long`");
}

CORE_API WString wstring_copy(WString const *self) {
    return wstring_from_parts(wstring_cstr(self), wstring_len(self), wstring_cap(self));
}

CORE_API WStringView wstring_into_view(WString const *self) {
    return (WStringView){
        .len = wstring_len(self),
        .data = wstring_cstr(self),
    };
}
