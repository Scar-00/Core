#ifndef _CORE_VECTOR_H_
#define _CORE_VECTOR_H_

#include "../build/build.h"

#include <stddef.h>
#include <stdbool.h>

struct ArrayHeader {
    size_t len;
    size_t cap;
};

CORE_API void *core_vec_create_internal(size_t capacity, size_t elem_size);
CORE_API void *core_vec_maygrow_internal(void *arr, size_t elem_size);
CORE_API void core_vec_destroy_internal(void *arr);
CORE_API void *core_vec_create_empty_internal(void);

#define vec_header(v) ((struct ArrayHeader *)(v) - 1)
#define vec_len(v) (vec_header((v))->len)
#define vec_cap(v) (vec_header((v))->cap)

#define Vec(type) type *
#define vec_new() core_vec_create_empty_internal()
#define vec_clear(v) (vec_header((v))->len = 0)
#define vec_with_size(ty, size) core_vec_create_internal((size), sizeof(ty))
#define vec_destroy(arr) (core_vec_destroy_internal(vec_header(arr)), arr = NULL)
#define vec_push(arr, value) ((arr) = core_vec_maygrow_internal((arr), sizeof(*(arr))), (arr)[vec_header((arr))->len++] = (value))
#define vec_pop(arr) (vec_header((arr))->len--)
#define vec_put(arr, index, value) {\
        if(!arr) arr = core_vec_maygrow_internal((arr), (size_t)sizeof(*(arr)));\
        if((index) >= vec_len((arr))) {\
            vec_len(arr) = index + 1;\
        }\
        (arr) = core_vec_maygrow_internal((arr), (size_t)sizeof(*(arr)));\
        (arr)[(index)] = (value);\
}

#define vec_at(arr, index) (arr)[(index)]
#define vec_iter(arr, iter) for(size_t (iter) = 0; (iter) < vec_len((arr)); (iter)++)
#define vec_foreach(arr, item) for(__typeof__(*(arr)) *item = (arr); item != (arr) + vec_len((arr)); item++)

#endif // _CORE_VECTOR_H_
