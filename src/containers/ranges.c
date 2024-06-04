#include "../internal.h"

CORE_API URange urange_new(size_t start, size_t end) {
    return (URange){ start, end };
}

CORE_API URange urange_end(size_t end) {
    return (URange){ 0, end };
}

CORE_API void urange_extend_right(URange *self, size_t amount) {
    self->start += amount;
}

CORE_API bool urange_extend_left(URange *self, size_t amount) {
    if(self->start - amount < 0) {
        return false;
    }
    self->start -= amount;
    return true;
}

CORE_API Range range_new(isize_t start, isize_t end) {
    return (Range){ start, end };
}

CORE_API Range range_end(isize_t end) {
    return (Range){ 0, end };
}

CORE_API void range_extend_right(Range *self, isize_t amount) {
    self->end += amount;
}

CORE_API void range_extend_left(Range *self, isize_t amount) {
    self->end += amount;
}
