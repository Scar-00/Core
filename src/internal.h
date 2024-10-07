#ifndef _CORE_INTERNAL_H_
#define _CORE_INTERNAL_H_

#include <core.h>
#include <gfx/gfx.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define SHORT_STRING_LENGTH 24
//  the caller needs to garuentee that both pointers are valid and cannot go oob
bool partial_cmp_ptr(const char *self, const char *predicate, size_t len);
bool wpartial_cmp_ptr(const wchar *self, const wchar *predicate, size_t len);
const WString utf8_from_ascii(StringView self);
size_t wstrlen(const wchar *ptr);


#define ARENA_DEFAULT_ALLOC_SIZE 1024 * 4

#define BIT(x) 1 << (x)

typedef struct File {
    String path;
    FileMode_ mode;
    FILE *fd;
}File;

typedef struct ArrayHeader ArrayHeader;

extern Allocator global_allocator;
extern Allocator default_allocator;

#endif // _CORE_INTERNAL_H_
