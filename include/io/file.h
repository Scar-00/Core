#ifndef _CORE_IO_FILE_H_
#define _CORE_IO_FILE_H_

#include "../build/build.h"
#include "../string/string.h"
#include "../build/defines.h"

#include <stdbool.h>

typedef struct File *FileHandle;

typedef unsigned int FileMode_;
typedef enum FileMode {
    FILE_READ       = CORE_BIT(0),
    FILE_WRITE      = CORE_BIT(1),
    FILE_APPEND     = CORE_BIT(2),
    FILE_PLUS       = CORE_BIT(3),
    FILE_BIN        = CORE_BIT(4),
}FileMode;

CORE_API FileHandle file_open(const char *path, FileMode_ mode);
CORE_API void file_close(FileHandle self);
CORE_API void *file_raw(FileHandle self);
CORE_API String file_read(FileHandle self);
CORE_API bool file_write_raw(FileHandle self, const char *data, size_t len);
CORE_API bool file_write(FileHandle self, const StringView data);
CORE_API bool file_exists(const StringView path);

CORE_API String file_read_to_string(const char *path);

CORE_API FileHandle stdio_get(void);
CORE_API FileHandle stderr_get(void);
CORE_API FileHandle stdin_get(void);

#endif // _CORE_IO_FILE_H_
