#include "../internal.h"

CORE_API StringView string_view_from(const char *data) {
    return (StringView){ strlen(data), data };
}

CORE_API StringView string_view_new(const char *data, size_t len) {
    return (StringView){ len, data };
}

CORE_API String string_view_into_string(StringView self) {
    return string_from_parts(self.data, self.len, self.len);
}

CORE_API StringView string_view_copy(StringView self) {
    return (StringView) {
        .len = self.len,
        .data = self.data,
    };
}

CORE_API bool string_view_contains(StringView self, StringView predicate) {
    if(predicate.len == 0) {
        return false;
    }

    for(size_t i = 0; i < self.len; i++) {
        if(partial_cmp_ptr(&self.data[i], predicate.data, predicate.len)) {
            return true;
        }
    }

    return false;
}

CORE_API WStringView wstring_view_from(const wchar *data) {
    return (WStringView){ wstrlen(data), data };
}

CORE_API WStringView wstring_view_new(const wchar *data, size_t len) {
    return (WStringView){ len, data };
}

CORE_API WStringView wstring_view_copy(WStringView self) {
    return (WStringView) {
        .len = self.len,
        .data = self.data,
    };
}

CORE_API bool wstring_view_contains(WStringView self, WStringView predicate) {
    if(predicate.len == 0) {
        return false;
    }

    for(size_t i = 0; i < self.len; i++) {
        if(wpartial_cmp_ptr(&self.data[i], predicate.data, predicate.len)) {
            return true;
        }
    }

    return false;
}
