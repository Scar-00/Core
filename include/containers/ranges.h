#ifndef _CORE_CONTAINERS_RANGES_H_
#define _CORE_CONTAINERS_RANGES_H_

#include "../build/build.h"
#include "../build/defines.h"

#include <stddef.h>
#include <stdbool.h>

typedef long long int isize_t;

typedef struct URange {
    size_t start, end;
}URange;

CORE_API URange urange_new(size_t start, size_t end);
CORE_API URange urange_end(size_t end);
CORE_API void urange_extend_right(URange *self, size_t amount);
CORE_API bool urange_extend_left(URange *self, size_t amount);

typedef struct Range {
    isize_t start, end;
}Range;

CORE_API Range range_new(isize_t start, isize_t end);
CORE_API Range range_end(isize_t end);
CORE_API void range_extend_right(Range *self, isize_t amount);
CORE_API void range_extend_left(Range *self, isize_t amount);

#define range_foreach(r, item) for(__typeof__((r).start) item = (r).start; item < (r).end; item++)
#define range_foreach_incl(r, item) for(__typeof__((r).start) item = (r).start; item <= (r).end; item++)

#endif // _CORE_CONTAINERS_RANGES_H_
