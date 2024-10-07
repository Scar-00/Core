#include "../internal.h"

#define DEFAULT_INITIAL_VECTOR_SIZE 8

CORE_API void *core_vec_create_internal(size_t capacity, size_t elem_size) {
    ArrayHeader *arr = global_allocator.alloc((capacity * elem_size) + sizeof(ArrayHeader));
    memset(arr, 0, (capacity * elem_size) + sizeof(ArrayHeader));
    arr[0].len = 0;
    arr[0].cap = capacity;
    arr++;
    return arr;
}

/*static void *core_vec_create_header_present_internal(void *arr, size_t capacity, size_t elem_size) {
    ArrayHeader *tmp = global_allocator.realloc(vec_header(arr), sizeof(ArrayHeader) + capacity * elem_size);
    tmp->cap = capacity;
    tmp++;
    return tmp;
}*/


CORE_API void *core_vec_maygrow_internal(void *arr, size_t elem_size) {
    /*if(!arr) {
        if((arr - sizeof(ArrayHeader)) != NULL) {
            arr = core_vec_create_header_present_internal(arr, DEFAULT_INITIAL_VECTOR_SIZE, elem_size);
        }else {
            arr = core_vec_create_internal(DEFAULT_INITIAL_VECTOR_SIZE, elem_size);
        }
    }*/

    if(vec_len(arr) >= vec_cap(arr)) {
        vec_cap(arr) = vec_cap(arr) == 0 ? DEFAULT_INITIAL_VECTOR_SIZE : vec_cap(arr) * 2;
        ArrayHeader *tmp = global_allocator.realloc(vec_header(arr), (vec_cap(arr) * elem_size) + sizeof(ArrayHeader));
        tmp++;
        return tmp;
    }
    return arr;
}

CORE_API void core_vec_destroy_internal(void *arr) {
    global_allocator.free(arr);
}

CORE_API void *core_vec_create_empty_internal(void) {
    ArrayHeader *arr = global_allocator.alloc(sizeof(ArrayHeader));
    memset(arr, 0, sizeof(ArrayHeader));
    arr[0].len = 0;
    arr[0].cap = 0;
    arr++;
    return arr;
}

CORE_API void *core_vec_copy(void *arr, size_t elem_size) {
    ArrayHeader *other = arr;
    size_t other_cap = other[-1].cap;
    ArrayHeader *new = core_vec_create_internal(other_cap, elem_size);
    new[-1].len = other[-1].len;
    memcpy((void*)new, arr, new[-1].len * elem_size);
    return new;
}




