#ifndef _CORE_INTERNAL_H_
#define _CORE_INTERNAL_H_

#include <core.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define SHORT_STRING_LENGTH 24

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
