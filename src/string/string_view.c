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
